MillisTimer
==============

## Description

An [Arduino](https://en.wikipedia.org/wiki/Arduino)/[Wiring](https://en.wikipedia.org/wiki/Wiring_(development_platform)) library for working with millis().

## Documentation

This library provides an easy way to have recurring actions based on the millis() timer.

## Example

```cpp
#include "MillisTimer.h"

// Create a timer that fires every 1000 milliseconds.
MillisTimer timer1 = MillisTimer(1000);

// This is the function that is called when the timer expires.
void myTimerFunction(MillisTimer &mt)
{
  Serial.print("Repeat: ");
  Serial.println(mt.getRemainingRepeats());
}


void setup()
{
  Serial.begin(9600);

  timer1.setInterval(1000);
  timer1.expiredHandler(myTimerFunction);
  timer1.setRepeats(5);
  timer1.start();
}


void loop()
{
  timer1.run();

  if (!timer1.isRunning())
  {
    Serial.println("Timer finished!");
    for (;;);
  }
  
  delay(10);
}
```


