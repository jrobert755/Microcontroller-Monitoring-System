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
				libFileHandler.a libjwsmtp.a libMMS.a libSettings.a)

all: MMSServer ArduinoInternetConnector

MMSServer: $(DIR)/MMSServer

ArduinoInternetConnector: $(DIR)/ArduinoInternetConnector

libraries: $(DEPENDENCIESALL)

$(DEPENDENCIESALL): | $(DIR)

$(DIR):
	mkdir -p $(DIR)
	
$(DIR)/MMSServer: $(DEPENDENCIESALL) MMSServer/*.cpp MMSServer/*.h;
	$(MAKE) -C MMSServer

$(DIR)/ArduinoInternetConnector: $(DEPENDENCIESALL) $(AICDEPENDENCIES)
	$(MAKE) -C ArduinoInternetConnector

$(DIR)/libCompression.a: $(libCompressionFiles); 
	$(MAKE) -C libCompression
	
$(DIR)/libFileHandler.a: $(libFileHandlerFiles); 
	$(MAKE) -C libFileHandler
	
$(DIR)/libjwsmtp.a: $(libjwsmtpFiles);
	$(MAKE) -C libjwsmtp
	
$(DIR)/libMMS.a: $(libMMSFiles);
	$(MAKE) -C libMMS
	
$(DIR)/libSettings.a: $(libSettingsFiles)
	$(MAKE) -C libSettings

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
	

