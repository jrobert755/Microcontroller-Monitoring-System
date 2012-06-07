all:
	g++ $(CFLAGS) -c -I./ *.cpp *.cc
	ar -cvq ../$(DIR)/libSerial.a *.o
	rm *.o
