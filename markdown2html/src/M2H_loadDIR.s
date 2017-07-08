	.file	"M2H_loadDIR.c"
	.comm	g_head,208,32
	.section	.rodata
.LC0:
	.string	"cat't open %s\r\n"
.LC1:
	.string	"%s\r\n"
	.text
	.globl	M2H_LoadDIR
	.type	M2H_LoadDIR, @function
M2H_LoadDIR:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	$0, -16(%rbp)
	movl	$g_config, %edi
	call	opendir
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jne	.L4
	movl	$g_config, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	jmp	.L3
.L5:
	movq	-8(%rbp), %rax
	addq	$19, %rax
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
.L4:
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	readdir
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L5
	movl	$1, %eax
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	M2H_LoadDIR, .-M2H_LoadDIR
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
