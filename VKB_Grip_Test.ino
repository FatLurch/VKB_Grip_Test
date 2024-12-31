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

unsigned long previousMillis = 0;

const long interval = 1000; 

void setup() {
  Serial.begin(500000);
  Serial1.begin(500000);
}

void loop() {

  unsigned long currentMillis = millis();
  
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
  }

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //Serial1.write("Test");

    //This is the "interrogation packed" for the VKB Gunfighter
    Serial1.write(0xA5);
    Serial1.write(0x09);
    Serial1.write(0x11);
    Serial1.write(0x98);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0xA5);
    Serial1.write(0xAB);
    
  }

}
