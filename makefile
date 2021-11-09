all: structrw.o
	gcc -o structrw structrw.o
structrw.o: structrw.c
	gcc -c structrw.c
run:
	./structrw $(ARGS)
clean:
	rm structrw
	rm *.o
	rm nyc_pop.data