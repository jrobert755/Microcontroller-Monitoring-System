#include <Messagev2.h>

Message message;

char serial[] = "serial=abcdefghijklmnop";

long next_send;
long last_reading;
double* readings;
long number_of_readings;
bool currently_reading;
byte pins_to_read;
bool sent;

void reset_readings(){
  for(byte i = 0; i < 6; i++){
    readings[i] = 0;
  }
  number_of_readings = 0;
}

void setup(){
  Serial.begin(9600);
  
  readings = (double*)malloc(sizeof(double) * 6);
  currently_reading = false;
  pins_to_read = 0;
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
    Serial.println("Got message");
    for(location = 0; location < strlen(read_message); location++){
      if(read_message[location] == '=') break;
    }
    
    if(location != strlen(read_message)){
      if(strncmp(read_message, "readtime", strlen("readtime")) == 0){
        long temp_to_add = atol(&read_message[location+1]);
        next_send = millis() + atol(&read_message[location+1])*1000;
        Serial.println(next_send, DEC);
        Serial.println(millis(), DEC);
        Serial.println(temp_to_add, DEC);
        if(next_send != millis()){
          sent = false;
        }
      } else if(strncmp(read_message, "pinstoread", strlen("pinstoread")) == 0){
        pins_to_read = atoi(&read_message[location+1]);
        reset_readings();
        currently_reading = true;
        Serial.println(pins_to_read);
      } else if(strncmp(read_message, "resistance", strlen("resistance")) == 0){
        //set resistance
      }
    }
  }
  
  if(next_send != 0 && !sent && millis() >= next_send){
    //send new readings over serial
    char to_send[150];
    for(byte i = 0; i < 6; i++){
      byte temp = 1<<i;
      if((pins_to_read&temp) == temp){
        readings[i] = readings[i]/number_of_readings;
        strcpy(to_send, "newreading;");
        strcat(to_send, serial);
        strcat(to_send, "&reading=");
        char holder[16];
        fmtDouble(readings[i], 3, holder, 16);
        strcat(to_send, holder);
        sprintf(holder, "%d", i);
        strcat(to_send, "&pin=");
        strcat(to_send, holder);
        strcat(to_send, "\n");
        message.SerialWrite(to_send);
      }
    }
    strcpy(to_send, "update;");
    strcat(to_send, serial);
    strcat(to_send, "\n");
    message.SerialWrite(to_send);
    sent = true;
    currently_reading = false;
  }
  
  if(currently_reading && (last_reading + 10000) >= millis()){
    for(byte i = 0; i < 6; i++){
      byte temp = 1<<i;
      if((pins_to_read&temp) == temp){
        int temp_voltage = analogRead(i);
        readings[i] += (5.0 * temp_voltage)/1024.0;
      }
    }
    number_of_readings++;
  }
}
