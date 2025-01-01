# VKB_Grip_Test
Basic Arduino Joystick interface to VKB MCG Pro grip

## Disclaimer
This information is provided without warranty. Also, if you're looking for elegant design or code, you are in the wrong place.

## Overview
This project is intended to wire an Arduino Micro directly to a VKB MCG Pro grip so that the Arduino can output the status of the grip as a Joystick using standard Windows joystick drivers.

## Circuit
The VKB grip uses a weird half-duplex UART line to transmit and recieve. In layman's terms, The TX and RX are tied together.

For my specific build I had a few Arduino Pro Micros laying around. This complicated things a bit since it uses 5V UART (The VKB grip is using 3.3V). There's a chance the UART pin on the STM32 microcontroller in the grip is 5V tolerant, but I wasn't feeling gutsy enough to try it, even if I were to dig in and verify the pin per the spec sheet. Ref: [https://community.st.com/t5/stm32-mcus-products/stm-uart-communication-voltage/td-p/288531](https://community.st.com/t5/stm32-mcus-products/stm-uart-communication-voltage/td-p/288531) 

My specific Arduino appeared to be outputting closer to 4V than 5V on the TX pin when measured with an oscillioscope. The two resistors act as a voltage divider to bring the TX voltage from the Arduino down to about 3.2V. The RX pin on my Arduino accepted the 3.3V signal from the VKB grip directly. 

To get the grip to "talk", an interrogation packet (a few hex characters) are required to be sent to the grip. In my code I'm simply ignoring this message when it's recieved by the Arduino (because the TX and RX UART pins are tied together).

Starting with a 3.3V Arduino should eliminate the need for the voltage divider and the voltage regulator.
![Circuit Layout](VKB_Grip_Schematic_bb.png)

Previous breadcrumbs on decoding the grip protocol: [https://www.reddit.com/r/HotasDIY/comments/lckamn/vkb_3_wire_protocol/](https://www.reddit.com/r/HotasDIY/comments/lckamn/vkb_3_wire_protocol/)
