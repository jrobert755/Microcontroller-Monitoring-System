include ../MakefileIncludes/*

DEPENDENCIES = $(subDirlibMMSFiles) $(subDirlibSerialFiles)

../$(DIR)/ArduinoInternetConnector: *.cpp $(DEPENDENCIES)
	g++ -Wall -I../libSerial -I../libMMS *.cpp -L../$(DIR) -lMMS -lSerial -o ../$(DIR)/ArduinoInternetConnector
	
