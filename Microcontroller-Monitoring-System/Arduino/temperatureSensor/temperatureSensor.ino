#include <Adafruit_MAX31855.h>
#include <Messagev2.h>
#include <SPI.h>

Message message;
Adafruit_MAX31855 thermocouple(13, 10, 12);

char serial[] = "serial=bcdefghijklmnopq";

long next_send;
long last_reading;
double readings;
long number_of_readings;
bool currently_reading;
bool sent;
char degrees_type;

void reset_readings(){
  readings = 0;
  number_of_readings = 0;
}

void setup(){
  Serial.begin(9600);
  
  readings = 0;
  number_of_readings = 0;
  currently_reading = false;
  last_reading = 0;
  sent = false;
  next_send = 0;
  
  delay(2000);
  char to_send[150];
  strcpy(to_send, "update;");
  strcat(to_send, serial);
  strcat(to_send, "\n");
  message.SerialWrite(to_send);
}

void loop(){
  message.SerialRead();
  if(message.isMessageComplete()){
    const char* read_message = message.getMessage();
    long location;
    for(location = 0; location < strlen(read_message); location++){
      if(read_message[location] == '=') break;
    }
    
    if(location != strlen(read_message)){
      if(strncmp(read_message, "readtime", strlen("readtime")) == 0){
        next_send = millis() + atol(&read_message[location+1])*1000;
        if(next_send != millis()){
          sent = false;
        }
      } else if(strncmp(read_message, "degrees", strlen("degrees")) == 0){
        degrees_type = read_message[location+1];
      }
    }
  }
  
  if(next_send != 0 && !sent && millis() >= next_send){
    //send new readings over serial
    char to_send[150];
    strcpy(to_send, "newreading;");
    strcat(to_send, serial);
    strcat(to_send, "&reading=");
    char holder[16];
    readings = readings/number_of_readings;
    fmtDouble(readings, 3, holder, 16);
    strcat(to_send, holder);
    strcat(to_send, "\n");
    message.SerialWrite(to_send);
    strcpy(to_send, "update;");
    strcat(to_send, serial);
    strcat(to_send, "\n");
    message.SerialWrite(to_send);
    sent = true;
    currently_reading = false;
  }
  
  if(currently_reading && (last_reading + 10000) >= millis()){
    double temp;
    if(degrees_type == 'f' || degrees_type == 'F'){
      temp = thermocouple.readFahrenheit();
      if(isnan(temp)){
        //something wrong
        while(true){}
      }
    } else if(degrees_type == 'c' || degrees_type == 'C'){
      double temp = thermocouple.readCelsius();
      if(isnan(temp)){
        while(true){}
      }
    } else{
      degrees_type = 'C';
      double temp = thermocouple.readCelsius();
      if(isnan(temp)){
        while(true){}
      }
    }
    number_of_readings++;
    readings += temp;
  }
}
