M2H: M2H_main.o M2H_loadDIR.o M2H_convert.o
	gcc $^ -o $@

M2H_loadDIR.o: M2H_loadDIR.c
	gcc -c $^

M2H_convert.o: M2H_convert.c
	gcc -c $^
	
M2H_main.o: M2H_main.c
	gcc -c $^

compile:
	sudo chmod -Rf 777 ../../MD
	rm -rf ../../html
	./M2H
	sudo chmod -Rf 777 ../../html
	
rm:
	-rm M2H *.o
