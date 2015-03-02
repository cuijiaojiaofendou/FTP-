objects = main2.o server.o diary.o

edit : $(objects)
	cc -o edit $(objects)
main2.o : main2.c 
	cc -c main2.c
server.o : server.c 
	cc -c server.c
diary.o: diary.c
	cc -c diary.c
clean : 
	rm edit $(objects)


