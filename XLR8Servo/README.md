# XLR8Servo
## Accelerated Servo Library for Arduino

Arduino's Servo library causes your servos to jitter.
Alorium Technology's XLR8Servo Library doesn't jitter.

With Arduino's Servo library you lose the analogWrite() (PWM) functionality on pins 9 and 10.

With Alorium Technology's XLR8Servo Library you can still use analogWrite() (PWM) functionality on pins 9 and 10.

Arduino's Servo library uses the microcontroller's only 16bit timer.

Alorium Technology's XLR8Servo Library leaves the 16bit timer available for other uses.

This library allows an Alorium Technology XLR8 board (a drop-in replacement for Arduino Uno found at www.aloriumtech.com) to control RC (hobby) servo motors.

It's a drop in replacement for the standard Arduino servo library, but it uses Alorium Technology's XLR8 acceleration hardware to drive the servos so the timing is more precise, doesn't jitter, doesn't depend on interrupt handling, and doesn't cause PWM functionality to be lost.

If you have an XLR8 board, this library is better in every way.

This library is compatible with XLR8 board package 1.9.0 and up. For older board packages or for the XLR8 Legacy board image, use XLR8Servo library version 1.1.0.

As of library version 1.2.0 the "write" and "writeMicroseconds" functions include an optional speed parameter. If used, the servo will move gradually from its current position to the target position at a speed between 1 & 15, 15 being the fastest. This is ignored by earlier version of the servo XB, so the library is forwards and backwards compatible with versions that do and do not recognize the speed parameter.

Using this library is the same as using Arduino's servo library which is very nicely described at http://arduino.cc/en/Reference/Servo

