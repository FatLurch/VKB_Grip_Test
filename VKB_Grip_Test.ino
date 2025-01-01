/*
 * Created 12/31/2024 by Fat_Lurch, built upon the SerialPassthrough sketch by Erik Nyquist
 * 
 * This sketch sets up the Arduino to communicate with a VKB MCG Pro grip and act as a Joystick in Windows
 * 
 * A 3k and a 10k resistor are used in a voltage divider to lower the output voltage of the TX pin (measured as ~4V before voltage divider)
 * 
 * ### WHEN COMPILING THIS CODE ON ARDUINO MICRO CLONES BOARDS, YOU'LL LIKELY NEED TO SET THE BOARD TYPE TO ARDUINO LEONARDO ###
 */

#include "Joystick.h"

Joystick_ Joystick;

unsigned long previousMillis = 0;

const long interval = 10; //The loop will fire after this many milliseconds
   
char hexOut[8]; 

uint8_t buffer[100] = {};

uint8_t byteIndex = 0;

//static uint8_t interrogation[] = {0xA5, 0x09, 0x11, 0x98, 0x00, 0x00, 0x00, 0xA5, 0xAB};

void setup() {
  Serial.begin(500000);
  Serial1.begin(500000);
  Joystick.begin();
  Joystick.setThrottleRange(105, 830);
  Joystick.setXAxisRange(-329, 280);
  Joystick.setYAxisRange(-130, 145);
  Joystick.setRzAxisRange(350, 1800);
  Joystick.setRyAxisRange(-2900, 2900);   //Master Mode hat y-axis
  Joystick.setRxAxisRange(-2800, 2800);   //Master Mode hat x-axis
}

void loop() {

  unsigned long currentMillis = millis();

  //This is necessary because of the way the Micro handles serial. Reference the SerialPassthrough sketch in examples > communication
  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1) (Vs Serial which is the USB interface)
    buffer[byteIndex] = Serial1.read();
    byteIndex++;
  }

  if (currentMillis - previousMillis >= interval) {
    
    previousMillis = currentMillis;

    //This is the "interrogation packet" for the VKB MCG Pro - 0xA5 0x09 0x11 0x98 0x00 0x00 0x00 0xA5 0xAB
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
      for(uint8_t i = 0; i < byteIndex; i++)
      {
        //Display of the raw hex coming from the grip. I'd manipulate one control at a time to see which bytes were reacting, then I'd focus on those bytes and try to decode them

        //sprintf(hexOut, "%02X", buffer[i]);
        //Serial.print(hexOut);
        //Serial.print(" ");
        
      }
      //Serial.print(" - ");

      int byteToTest = 22;

      /*
      //Display the individual bits in the byte of interest to see which bit starts reacting first
      Serial.print(bitRead(buffer[byteToTest], 7), BIN);
      Serial.print(bitRead(buffer[byteToTest], 6), BIN);
      Serial.print(bitRead(buffer[byteToTest], 5), BIN);
      Serial.print(bitRead(buffer[byteToTest], 4), BIN);
      Serial.print(bitRead(buffer[byteToTest], 3), BIN);
      Serial.print(bitRead(buffer[byteToTest], 2), BIN);
      Serial.print(bitRead(buffer[byteToTest], 1), BIN);
      Serial.print(bitRead(buffer[byteToTest], 0), BIN);
      Serial.print(" - ");
      */

      //Tests for each byte. 
      //I was starting with a bitmask of B00000000 and then "turning on" the most significant bit. I'd then sweep the range to see if the result was incrementing/decrementing correctly and smoothly
      //If turning on a bit caused non-linear behavior, I'd set an inversion mask bit to correct
      //At the end I'd try using an offset bit mask to get the swept range (e.g. from x to - x)
      //The bitshifts (<< and >>) were to rearrange bits to the correct spot. e.g. the first bit on a signed value needs to be the left most bit, even though that makes for a silly large number

      //                          Bit Mask    Inv Mask     offset Mask
      //int test = ((buffer[24] & B00011111 ^ B00001001) + B00000001 << 11);
      //int test2 = ((buffer[23] & B11111111 ^ B11011000) + B01100000 << 3);  //
      //int test3 = (test + test2) >> 3;// 
      
      
      //Serial.print(test);
      //Serial.print(" - ");
      //Serial.print(test2);
      //Serial.print(" - ");
      Serial.print(test3);
      
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
      Joystick.setThrottle(((buffer[30] & B00000011) << 8) + buffer[29] ^ B10011111);       //Brake Lever
      Joystick.setRzAxis(((buffer[32] & B00111111 ^ B00101011) << 5) + ((buffer[31] & B11111000 ^ B01011000) >> 3));    //Analog trigger
      Joystick.setXAxis((((buffer[28] & B00111111 ^ B00000111) << 10) + ((buffer[27] & B11110000 ^ B01100000) << 2)) * -0.015625);    //Gate Cont Hat X axis
      Joystick.setYAxis((((buffer[26] & B00111111 ^ B00110001) << 10) + ((buffer[25] & B11100000 ^ B11100000) << 2)) * -0.0078125);   //Gate Cont Hat Y Axis
      Joystick.setRyAxis((((buffer[22] & B00011111 ^ B00001110) + B00000010 << 11) + ((buffer[21] & B11111111 ^ B11001000) + B00001000 << 3)) >> 3);  //Master Mode Hat Y Axis
      Joystick.setRxAxis((((buffer[24] & B00011111 ^ B00001001) + B00000001 << 11) + ((buffer[23] & B11111111 ^ B11011000) + B01100000 << 3)) >> 3);  //Master Mode Hat X Axis

    }

    byteIndex = 0;

  }

}
