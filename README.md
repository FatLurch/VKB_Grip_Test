# VKB_Grip_Test

Basic Arduino Joystick interface to VKB MCG Pro grip

## Disclaimer

This information is provided without warranty. Also, if you're looking for elegant design or code, you are in the wrong place.

## Overview

This project is intended to wire an Arduino Micro directly to a VKB MCG Pro grip so that the Arduino can output the status of the grip as a Joystick using standard Windows joystick drivers.

## Circuit

The VKB grip uses a weird half-duplex UART line to transmit and recieve. In layman's terms, The TX and RX are tied together.

For my specific build I had a few Arduino Pro Micros laying around. This complicated things a bit since it uses 5V UART (The VKB grip is using 3.3V). There's a chance the UART pin on the STM32 microcontroller in the grip is 5V tolerant, but I wasn't feeling gutsy enough to try it, even if I were to dig in and verify the pin per the spec sheet. Ref: [https://community.st.com/t5/stm32-mcus-products/stm-uart-communication-voltage/td-p/288531](https://community.st.com/t5/stm32-mcus-products/stm-uart-communication-voltage/td-p/288531) 

The Micro also differentiates Serial (the USB com port) from Serial1 (Pins 0 & 1). If you're targeting this sketch for another type of board, you may need to go in and adjust these references.

My specific Arduino appeared to be outputting closer to 4V than 5V on the TX pin when measured with an oscillioscope. The two resistors act as a voltage divider to bring the TX voltage from the Arduino down to about 3.2V. The RX pin on my Arduino accepted the 3.3V signal from the VKB grip directly. 

To get the grip to "talk", an interrogation packet (a few hex characters) are required to be sent to the grip. In my code I'm simply ignoring this message when it's recieved by the Arduino (because the TX and RX UART pins are tied together).

Starting with a 3.3V Arduino should eliminate the need for the voltage divider and the voltage regulator.
![Circuit Layout](/images/VKB_Grip_Schematic_bb.png)

All 32 possible buttons are mapped. You can view their status using "Set up USB game controllers" in Windows
![Testing](/images/Test.png)

To wire to the grip itself, I used a 3 pin JST Molex PicoBlade 1.25mm style connector from this kit on Amazon: https://www.amazon.com/gp/product/B08RMQP6YP/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1

![Connector](/images/Conn.jpg)

## Sketch

** WHEN COMPILING THIS CODE ON ARDUINO MICRO CLONES BOARDS, YOU'LL LIKELY NEED TO SET THE BOARD TYPE TO ARDUINO LEONARDO **

The two analog hats are mapped both as analog outputs, and as digital "buttons" for up, down, left and right.

The math for calculating the analog positions is a bit sketchy and cxould use some work.

Currently, grip Z twist isn't incorperated because I'm not using it in my application and I don't want to build a custom PCB and mount to interface to the pogo pins in the twist adapter.

Previous breadcrumbs on decoding the grip protocol: [https://www.reddit.com/r/HotasDIY/comments/lckamn/vkb_3_wire_protocol/](https://www.reddit.com/r/HotasDIY/comments/lckamn/vkb_3_wire_protocol/)
