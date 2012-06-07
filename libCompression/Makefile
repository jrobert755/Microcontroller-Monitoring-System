all:
	g++ $(CFLAGS) -c bzip2/*.c
	g++ $(CFLAGS) -c -I../libFileHandler *.cpp
	ar -cvq ../$(DIR)/libCompression.a *.o
	rm *.o
