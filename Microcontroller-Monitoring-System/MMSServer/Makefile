include ../MakefileIncludes/*

DEPENDENCIES = $(subDirlibCompressionFiles) $(subDirlibMMSFiles) \
				$(subDirlibFileHandlerFiles) $(subDirlibSettingsFiles) \
				$(subDirlibjwsmtpFiles)

../$(DIR)/MMSServer: *.cpp *.h $(DEPENDENCIES)
	g++ $(CFLAGS) -pthread -I../libFileHandler -I../libCompression -I../libjwsmtp -I../libSettings -I../libMMS *.cpp -L../$(DIR) -lCompression -lFileHandler -ljwsmtp -lMMS -lSettings -o ../$(DIR)/MMSServer
