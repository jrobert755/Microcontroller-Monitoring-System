#include "Watchdog.h"

void sendMessage(string& arduino_name){
	string message = "Arduino ";
	message += arduino_name;
	message += " stopped working";
	// replace the users 'to' and 'from' here before compiling this demo
   jwsmtp::mailer m("jrobert755@gmail.com", "webmaster@randomprojects.net", arduino_name.c_str(),
                    message.c_str(), "randomprojects.net",
                    jwsmtp::mailer::SMTP_PORT, false);

   m.username("webmaster@randomprojects.net");
   m.password("Alfred755.");
   m.send(); // send the mail
   std ::cout << m.response() << "\n";
}

void* watchdogThread(void* arguments){
	MMSServer* server = (MMSServer*)arguments;
	sleep(300);
	while(server->isRunning()){
		time_t current_time = time(NULL);
		string file_name = convertMDYtoString(current_time);
		file_name += ".csv";
		for(map<string, Arduino*>::iterator iter = known_arduinos.begin(); iter != known_arduinos.end(); iter++){
			string arduino_name = iter->first;
			if(!iter->second->isConnected()){
				sendMessage(arduino_name);
				pthread_exit(NULL);
				return NULL;
			}
			string full_file_name = arduino_name + file_name;
			struct stat buffer;
			if(lstat(full_file_name.c_str(), &buffer) == -1){
				//error out cause correct file was not found
				/*sendMessage(arduino_name);
				pthread_exit(NULL);
				return NULL;*/
				continue;
			}
			if(buffer.st_mtime - current_time >= 3600){
				//error out cause file time is outdated
				sendMessage(arduino_name);
				pthread_exit(NULL);
				return NULL;
			}
		}
		sleep(300);
	}
	pthread_exit(NULL);
	return NULL;
}
