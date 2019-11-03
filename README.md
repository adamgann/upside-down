# upside-down
Halloween 2019: The wall of lights from Stranger Things

---
In this costume an Arduino Uno, several shift registers (74HC595), and a handful of LEDs were used to create the wall from the first season of Stranger Things. The result can be seen [in this video](https://www.youtube.com/watch?v=mKRO_VgZO74) and the build process is discussed in [this blog post]([in-depth blog post](http://adamgannon.com/2019/11/03/an-arduino-powered-stranger-things-costume/).


All design files are available to use (see terms in LICENSE). A warning this was hacked together fairly quickly, so don't expect a graceful board layout or bug-free software. Please cite this repository or my blog.

---
## Files

### arduino/
Contains Arudino sketches. Partly based on examples from [Janis Rove](https://github.com/janisrove/Arduino-74HC595-shift-registers) and the folks at [Adafruit](https://learn.adafruit.com/adafruit-arduino-lesson-4-eight-leds/brightness-control).

The sketch `check_leds.ino` cycles through each letter of the alphabet to verify wiring is correct.

The sketch `main.ino` runs through the light sequence.

### eagle/
Contains schematic and design files for the PCB. The project name is `shift_register_board`. Huge thanks to the [Sparkfun Eagle library](https://github.com/sparkfun/SparkFun-Eagle-Libraries) for footprints.

### graphics/
Files for the silkscreen logo added to the board. I followed the Eagle BMP import process discussed [in this tutorial](https://learn.sparkfun.com/tutorials/importing-custom-images-into-eagle/method-3-import-bmp).
