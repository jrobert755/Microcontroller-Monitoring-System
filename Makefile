include MakefileIncludes/*

DEBUG = false
CFLAGS = -Wall

ifeq ($(DEBUG), false)
	DIR = linuxRelease
else
	DIR = linuxDebug
	CFLAGS += -ggdb
endif

export DIR
export CFLAGS

#DEPENDENCIESALL = Release/libCompression.a Release/libFileHandler.a \
#				Release/libMMS.a Release/libSettings.a
DEPENDENCIESALL = $(addprefix $(DIR)/,libCompression.a \
				libFileHandler.a libMMS.a libSettings.a)

all: MMSServer ArduinoInternetConnector

MMSServer: $(DIR)/MMSServer

ArduinoInternetConnector: $(DIR)/ArduinoInternetConnector

libraries: $(DEPENDENCIESALL)

$(DEPENDENCIESALL): | $(DIR)

$(DIR):
	mkdir -p $(DIR)
	
$(DIR)/MMSServer: $(DEPENDENCIESALL);
	make -C MMSServer

$(DIR)/ArduinoInternetConnector: $(DEPENDENCIESALL) $(DIR)/libSerial.a;
	make -C ArduinoInternetConnector

$(DIR)/libCompression.a: $(libCompressionFiles); 
	make -C libCompression
	
$(DIR)/libFileHandler.a: $(libFileHandlerFiles); 
	make -C libFileHandler
	
$(DIR)/libMMS.a: $(libMMSFiles);
	make -C libMMS
	
$(DIR)/libSettings.a: $(libSettingsFiles)
	make -C libSettings
	
$(DIR)/libSerial.a: $(libSerialFiles)
	make -C libSerial
	
DELETEDIR = false
ifeq ($(DELETEDIR), false)
clean: cleanwodir
else
clean: cleanwdir
endif

cleanwodir:
	rm -f $(DIR)/*.a
	
cleanwdir:
	rm -rf $(DIR)
	
.PHONY: all MMSServer ArduinoInternetConnector libraries
	

