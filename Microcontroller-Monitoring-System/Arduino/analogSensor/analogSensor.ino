#include <Messagev2.h>
#include <SPI.h>

Message message;

char serial[] = "serial=abcdefghijklmnop";

long next_send;
long last_reading;
double* readings;
long number_of_readings;
bool currently_reading;
byte pins_to_read;
bool sent;

long ab_resistance = 101700;
byte number_of_resistors = 1;

void reset_readings(){
  for(byte i = 0; i < 6; i++){
    readings[i] = 0.0;
  }
  number_of_readings = 0;
}

void setup(){
  Serial.begin(9600);
  SPI.begin();
  
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
    for(location = 0; location < strlen(read_message); location++){
      if(read_message[location] == '=') break;
    }
    
    if(location != strlen(read_message)){
      if(strncmp(read_message, "readtime", strlen("readtime")) == 0){
        next_send = millis() + atol(&read_message[location+1])*1000;
        if(next_send != millis()){
          sent = false;
        }
      } else if(strncmp(read_message, "pinstoread", strlen("pinstoread")) == 0){
        pins_to_read = atoi(&read_message[location+1]);
        reset_readings();
        currently_reading = true;
      } else if(strncmp(read_message, "resistance", strlen("resistance")) == 0){
        //set resistance
        Serial.println("enter resistance");
        long total_resistance = atol(&read_message[location+1]);
        Serial.println(total_resistance, DEC);
        byte resistances[6];
        byte current_resistor = 0;
        for(int i = 0; i < 6; i++) resistances[i] = 0;
        total_resistance -= (45*number_of_resistors);
        //while(total_resistance > 0 && current_resistor < 6){
           long next_step = total_resistance >= ab_resistance ? ab_resistance : total_resistance;
           total_resistance -= ab_resistance;
           resistances[current_resistor] = floor((256.0 * next_step) / (ab_resistance));
           current_resistor++;
        //}
        
        for(int i = 0; i < 6; i++){
          Serial.println(resistances[i], DEC);
          digitalWrite(10, LOW);
          SPI.transfer(i);
          SPI.transfer(resistances[i]);
          digitalWrite(10, HIGH);
        }
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
        readings[i] *= (5.0/1023.0);
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
  
  if(currently_reading && (last_reading + 120000) <= millis()){
    char temp_to_send[128];
    strcpy(temp_to_send, "log;");
    for(byte i = 0; i < 6; i++){
      byte temp = 1<<i;
      if((pins_to_read&temp) == temp){
        int temp_voltage = analogRead(i);
        readings[i] += temp_voltage;
        char holder[16];
        sprintf(holder, "%d=%d", i, temp_voltage);
        strcat(temp_to_send, holder);
        strcat(temp_to_send, ";");
      }
    }
    strcat(temp_to_send, "\n");
    Serial.write(temp_to_send);
    number_of_readings++;
    last_reading = millis();
  }
}
