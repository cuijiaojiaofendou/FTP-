objects = main.o conn_ect.o client.o

edit:$(objects)
	cc -o edit $(objects)
main.o:main.c
	cc -c main.c
conn_ect.o:conn_ect.c
	cc -c conn_ect.c
client.o:client.c
	cc -c client.c
clean:
	rm edit $(objects)
