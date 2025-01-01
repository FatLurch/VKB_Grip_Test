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

const long interval = 10; //The loop will fire after this many milliseconds

char msg[74];
char hexOut[10];
//char binOut[8];

uint8_t buffer[100] = {};

uint8_t byteIndex = 0;

//static uint8_t interrogation[] = {0xA5, 0x09, 0x11, 0x98, 0x00, 0x00, 0x00, 0xA5, 0xAB};

byte flipByte(byte c){
  char r=0;
  for(byte i = 0; i < 8; i++){
    r <<= 1;
    r |= c & 1;
    c >>= 1;
  }
  return r;
}

void setup() {
  Serial.begin(500000);
  Serial1.begin(500000);
  Joystick.begin();
  Joystick.setXAxisRange(-329, 280);
  Joystick.setYAxisRange(-130, 145);
  Joystick.setRxAxisRange(90, 830);
  Joystick.setRyAxisRange(350, 1800);
}

void loop() {

  unsigned long currentMillis = millis();
  /*
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }
  */

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    //Serial.write(Serial1.read());   // read it and send it out Serial (USB)
    buffer[byteIndex] = Serial1.read();
    byteIndex++;
  }

  if (currentMillis - previousMillis >= interval) {
    
    previousMillis = currentMillis;
    //Serial1.write("Test");

    //This is the "interrogation packed" for the VKB MCG Pro - 0xA5 0x09 0x11 0x98 0x00 0x00 0x00 0xA5 0xAB
    
    Serial1.write(0xA5);
    Serial1.write(0x09);
    Serial1.write(0x11);
    Serial1.write(0x98);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0xA5);
    Serial1.write(0xAB);

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
      //Serial.print(buffer[38], BIN);

      int byteToTest = 28;

      Serial.print(bitRead(buffer[byteToTest], 7), BIN);
      Serial.print(bitRead(buffer[byteToTest], 6), BIN);
      Serial.print(bitRead(buffer[byteToTest], 5), BIN);
      Serial.print(bitRead(buffer[byteToTest], 4), BIN);
      Serial.print(bitRead(buffer[byteToTest], 3), BIN);
      Serial.print(bitRead(buffer[byteToTest], 2), BIN);
      Serial.print(bitRead(buffer[byteToTest], 1), BIN);
      Serial.print(bitRead(buffer[byteToTest], 0), BIN);
      Serial.print(" - ");
      //Serial.print(bitRead(buffer[byteToTest], 4), BIN);
      /*
      Serial.print(buffer[byteToTest], DEC);
      Serial.print(" - ");
      Serial.print(flipByte(buffer[byteToTest]), DEC);
      Serial.print(" - ");
      Serial.print(bitRead(buffer[byteToTest], 1), BIN);
      Serial.print(" - ");
      Serial.print(bitRead(buffer[byteToTest], 0), BIN);
      Serial.print(" - ");
      */
      
      //int test = buffer[byteToTest] & 3;  //Bitmask, these right-most bits are the most significant bits for the brake analog in byte 30
      //int test = (map(((buffer[30] & B00000011) << 8) + buffer[29] ^ B10011111, 80, 830, 0, 1023));  //Bitmask, these right-most bits are the most significant bits for the brake analog in byte 30
      //Joystick.setRxAxis(((buffer[30] & B00000011) << 8) + buffer[29] ^ B10011111);

      //int test = ((buffer[28] & B00111111 ^ B00000111) << 10);  //Working baseline
      //int test2 = ((buffer[27] & B11110000 ^ B01100000) << 2);
      //int test3 = (test + test2)*-0.015625;
      
      /*
      Serial.print(test);
      Serial.print(" - ");
      Serial.print(test2);
      Serial.print(" - ");
      Serial.print(test3);
      */
      Serial.println("");

      


      // Digital outputs
      Joystick.setButton(0, bitRead(buffer[40], 7));    //Forward Trigger
      Joystick.setButton(1, bitRead(buffer[38], 5));    //1st stage of two stage trigger
      Joystick.setButton(2, !bitRead(buffer[38], 6));   //2nd stage of two stage trigger
      Joystick.setButton(3, bitRead(buffer[39], 7));    //Grey round button to the right of PC hat
      Joystick.setButton(4, !bitRead(buffer[39], 5));   //Grey round button to the right of MANVR hat
      Joystick.setButton(5, bitRead(buffer[39], 6));    //Red round button to the left of MANVR hat
      Joystick.setButton(6, bitRead(buffer[38], 7));    //Black ring finger button below reset hat
      Joystick.setButton(7, bitRead(buffer[40], 4));    //Reset hat center click
      Joystick.setButton(8, !bitRead(buffer[40], 2));   //Reset hat left click
      Joystick.setButton(9, !bitRead(buffer[40], 0));   //Reset hat back click
      Joystick.setButton(10, bitRead(buffer[40], 3));   //Reset hat right click
      Joystick.setButton(11, !bitRead(buffer[40], 1));  //Reset hat forward click
      Joystick.setButton(12, bitRead(buffer[38], 4));   //PC hat center click
      Joystick.setButton(13, bitRead(buffer[38], 3));   //PC hat left click
      Joystick.setButton(14, !bitRead(buffer[38], 1));  //PC hat down click
      Joystick.setButton(15, bitRead(buffer[38], 2));   //PC hat right click
      Joystick.setButton(16, bitRead(buffer[38], 0));   //PC hat up click
      Joystick.setButton(17, bitRead(buffer[40], 5));   //Gate Cont hat click
      Joystick.setButton(18, bitRead(buffer[41], 7));   //Gate Cont hat left
      Joystick.setButton(19, bitRead(buffer[41], 4));   //Gate Cont hat down
      Joystick.setButton(20, !bitRead(buffer[41], 5));  //Gate Cont hat right
      Joystick.setButton(21, !bitRead(buffer[41], 6));  //Gate Cont hat right
      Joystick.setButton(22, !bitRead(buffer[40], 6));  //Master mode hat click
      Joystick.setButton(23, bitRead(buffer[41], 3));   //Master mode hat left
      Joystick.setButton(24, bitRead(buffer[41], 0));   //Master mode hat down
      Joystick.setButton(25, !bitRead(buffer[41], 1));  //Master mode hat right
      Joystick.setButton(26, bitRead(buffer[41], 2));   //Master mode hat right
      Joystick.setButton(27, !bitRead(buffer[39], 4));  //DC hat click
      Joystick.setButton(28, !bitRead(buffer[39], 0));  //DC hat back
      Joystick.setButton(29, !bitRead(buffer[39], 3));  //DC hat down
      Joystick.setButton(30, bitRead(buffer[39], 1));   //DC hat down
      Joystick.setButton(31, !bitRead(buffer[39], 2));  //DC hat up

      // Analog outputs
      Joystick.setRxAxis(((buffer[30] & B00000011) << 8) + buffer[29] ^ B10011111); 
      Joystick.setRyAxis(((buffer[32] & B00111111 ^ B00101011) << 5) + ((buffer[31] & B11111000 ^ B01011000) >> 3)); 
      Joystick.setXAxis((((buffer[28] & B00111111 ^ B00000111) << 10) + ((buffer[27] & B11110000 ^ B01100000) << 2)) * -0.015625);
      Joystick.setYAxis((((buffer[26] & B00111111 ^ B00110001) << 10) + ((buffer[25] & B11100000 ^ B11100000) << 2)) * -0.0078125);

      /*
      int test = ((buffer[28] & B00111111 ^ B00000111) << 10);  //Working baseline
      int test2 = ((buffer[27] & B11110000 ^ B01100000) << 2);
      int test3 = (test + test2)*-0.015625;
      */

    }

    byteIndex = 0;

  }

}
