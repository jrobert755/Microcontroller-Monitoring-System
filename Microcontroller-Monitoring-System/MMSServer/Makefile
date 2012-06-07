include ../MakefileIncludes/*

DEPENDENCIES = $(subDirlibCompressionFiles) $(subDirlibMMSFiles) \
				$(subDirlibFileHandlerFiles) $(subDirlibSettingsFiles)

../$(DIR)/MMSServer: *.cpp *.h
	g++ $(CFLAGS) -pthread -I../libFileHandler -I../libCompression -I../libSettings -I../libMMS *.cpp -L../$(DIR) -lCompression -lFileHandler -lMMS -lSettings -o ../$(DIR)/MMSServer
