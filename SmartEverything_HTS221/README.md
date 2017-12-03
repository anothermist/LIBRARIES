# SmartEverything ST HTS221  Humidity Sensor
----

This is a library for the ST HTS221 component.

The HTS221 is an ultra compact sensor for relative humidity and temperature.
It includes a sensing element and a mixed signal ASIC to provide the measurement information
through digital serial interfaces.

The sensing element consists of a polymer dielectric planar capacitor structure
capable of detecting relative humidity variations and is manufactured using a dedicated ST process.

The HTS221 is available in a small top-holed cap land grid array (HLGA)
package guaranteed to operate over a temperature range from -40 °C to +120 °C.


[* ST HTS221 Home Page *](http://www.stmicroelectronics.com.cn/web/en/catalog/sense_power/FM89/SC1718/PF260067)

Host control and result reading is performed using an I2C interface, no extra pins are required.

It was principally designed to work with the SmartEverything board, but could
be easily adapt and use on every Arduino and Arduino Certified boards.

[*Library is based on the useful project done by Kris Winer*](https://github.com/kriswiner/VL6180X)

Written by Seve <seve@axelelettronica.it>.

## Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE.
* **/src** - Source files for the library (.cpp, .h).
* **library.properties** - General library properties for the Arduino package manager.


## Releases
---
#### v1.1.2 Second Release 3-Jam-2017
* Increased Humidity sensor accuracy from int to double. Modified the provided examples accordingly.

#### v1.1.1 Second Release 18-Dec-2015
* Change example to be more Arduino boards compatible and not only for ASME boards family.

#### v1.1.0 Second Release 18-Dec-2015
* Fixed Issue:<br>
    Fix Humidity example: Add Wire.begin() to initiate the Wire library and join the I2C bus.<br>
    Make the temperature reading more accurate: The HTS221 sensor is more accurate than 1 degree C.<br>

* New Features:<br>
    Add 2 new examples to be used with the Arduino plotter<br>

#### v1.0.0 First Release

## Documentation
--------------
* **[Installing an Arduino Library Guide](http://www.arduino.cc/en/Guide/Libraries#toc3)** - How to install a SmartEverything library on the Arduino IDE using the Library Manager


## License Information
-------------------

Copyright (c) Amel Technology. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

