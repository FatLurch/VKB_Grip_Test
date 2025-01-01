/*
  SerialPassthrough sketch

  Some boards, like the Arduino 101, the MKR1000, Zero, or the Micro, have one
  hardware serial port attached to Digital pins 0-1, and a separate USB serial
  port attached to the IDE Serial Monitor. This means that the "serial
  passthrough" which is possible with the Arduino UNO (commonly used to interact
  with devices/shields that require configuration via serial AT commands) will
  not work by default.

  This sketch allows you to emulate the serial passthrough behaviour. Any text
  you type in the IDE Serial monitor will be written out to the serial port on
  Digital pins 0 and 1, and vice-versa.

  On the 101, MKR1000, Zero, and Micro, "Serial" refers to the USB Serial port
  attached to the Serial Monitor, and "Serial1" refers to the hardware serial
  port attached to pins 0 and 1. This sketch will emulate Serial passthrough
  using those two Serial ports on the boards mentioned above, but you can change
  these names to connect any two serial ports on a board that has multiple ports.

  created 23 May 2016
  by Erik Nyquist
*/

/*
 * Created 12/31/2024 by Erik Kofahl, built upon the SerialPassthrough sketch by Erik Nyquist
 * 
 * This is a basic test to try to interrogate a VKB grip using a 1-wire Serial interface
 * 
 * A 3k and a 10k resistor are used in a voltage divider to lower the output voltage of the TX pin (measured as ~4V before voltage divider)
 * 
 */

#include "Joystick.h"

Joystick_ Joystick;

unsigned long previousMillis = 0;

const long interval = 50; //The loop will fire after this many milliseconds

char msg[74];
char hexOut[10];

uint8_t buffer[100] = {};

uint8_t byteIndex = 0;

//static uint8_t interrogation[] = {0xA5, 0x09, 0x11, 0x98, 0x00, 0x00, 0x00, 0xA5, 0xAB};

void setup() {
  Serial.begin(500000);
  Serial1.begin(500000);
  Joystick.begin();
}

void loop() {

  unsigned long currentMillis = millis();

  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    //Serial.write(Serial1.read());   // read it and send it out Serial (USB)
    //sprintf(msg,"%02X",buffer[byteIndex] = Serial1.read());
    buffer[byteIndex] = Serial1.read();
    //Serial.write(msg);
    //Serial.write(" ");
    //buffer[byteIndex] = msg;
    byteIndex++;
  }

  if (currentMillis - previousMillis >= interval) {
    
    previousMillis = currentMillis;
    //Serial1.write("Test");

    //This is the "interrogation packed" for the VKB Gunfighter - 0xA5 0x09 0x11 0x98 0x00 0x00 0x00 0xA5 0xAB
    
    Serial1.write(0xA5);
    Serial1.write(0x09);
    Serial1.write(0x11);
    Serial1.write(0x98);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0xA5);
    Serial1.write(0xAB);
    


    //Serial.println("");



    

    if(byteIndex == 43) //i.e. if it's a good packet
    {
      for(int i = 0; i < byteIndex; i++)
      {
        //Serial.print(buffer[i], HEX);
        sprintf(hexOut, "%02X", buffer[i]);
        Serial.print(hexOut);
        Serial.print(" ");
        
      }
      Serial.print(" - ");
      Serial.print(buffer[40], BIN);
      Serial.print(" - ");
      Serial.print(bitRead(buffer[40], 7), BIN);
      Serial.println("");
      Joystick.setButton(0, bitRead(buffer[40], 7));
      /*
      Serial.print(" -- ");
      Serial.print(byteIndex);
      Serial.print(" bytes");
      Serial.print(" -- ");
      Serial.print("Good packet");
      */
    }

    byteIndex = 0;

    

  }

}
