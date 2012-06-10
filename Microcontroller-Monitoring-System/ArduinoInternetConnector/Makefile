include ../MakefileIncludes/*

DEPENDENCIES = $(subDirlibMMSFiles)

../$(DIR)/ArduinoInternetConnector: *.cpp $(DEPENDENCIES)
	g++ $(CFLAGS) -pthread  -I../libMMS *.cpp -L../$(DIR) -lMMS -o ../$(DIR)/ArduinoInternetConnector
	
