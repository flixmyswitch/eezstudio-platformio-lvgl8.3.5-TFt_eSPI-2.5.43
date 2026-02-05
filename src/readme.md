How I MADE MY EEZ PROJECT

Below i will record the main point of how i have built my EEZ Studio project


Not being a programmer I found this tough, not the softwares fault but the user.  I have pushed on through weeks of code not working, complie errors and the 
TFT screen going into like a constant rebooting loop. I even had the MD5 error where i had to reset the ESP chip using PYTHON.  I will have to make a video upon how I reset the ESP32.  Check the videos I may have already made it.


MY VIDEOS

I am making videos the record my progress.  I think that if I have found it hard other will also.  These videos are recording my learning and may have errors within them. Always checked for
a pinned comment to see for any corrections OR a part2 of that video correcting it if its a major error.


WHAT IS MY GOAL

My main goal for using EEZ Studio is to be able to get DATA both numbers and Strings out of it and into Arduino or Platformio.  Once I am able to do these two simple operations I will be able
to build my project


MY PROJECT

I want to be able to make a nice UI screen to control 3 stepper motors, like the small ones found on many 3D printers, but you could control anything once we get our ESP32 or Arduino set up.

These steppers will drive a 3 axis camera slider.  Controlling Linear travel, pan and tilt.

The screen will allow the user to input, length of the slider, max speed and some other info to contomise the slider.

The main input screen with have touch buttons to allow the IN MID & OUT points to be stored.  These points will record the linera position, Pan & tilt angles.



I have had great help through the Discord channel  here is the link: https://discord.gg/dhYMnCB

They have a donatation system to help support them   https://liberapay.com/eez-open/donate  or via Directly via Paypal using this email address info@envox.hr


The reason why simulator can't execute C++ code maybe isn't obvious. This would require C++ compiler which would produce machine/native code and Studio would need to somehow run that 
machine/native code. Theoretically this is possible, but compiling step would slow down simulation startup time significantly. And even then, some of it couldn't  know how to compile
- for example, the code that starts and stops the motor in your case, which can't be run on PC, only on your device.