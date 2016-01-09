# arduino_pingpong
arduino-based pingpong, which uses MAX7219 8x8 LED matrix and four buttons

Matrix pins are connected to pins 10 to 12:
  DIN (Data in) - 12
  CS - 11
  CLK - 10
  
Buttons are connected to pins 4 to 7.

You will need LedControl library to use it. https://github.com/wayoda/LedControl
