DICE is a Discrete Integrated Circuit Emulator. It emulates computer systems
that lack any type of CPU, consisting only of discrete logic components.


1) Usage

By default, the DICE executable will launch in GUI mode, which allows
the user to configure the emulator and load a circuit. Additionally, DICE
can be run from the command line, which by default will launch the emulator
in fullscreen mode without the GUI and start a game immediately. When launching
from the command line, press the Exit key to exit the emulator. 

Launching DICE from the command line can be accomplished by navigating to the
directory where DICE is installed and typing:

dice gamename [parameters]

Where "gamename" is the name of the game to be run:

antiaircraft
attack
breakout
cleansweep
crashnscore
crossfire
gotcha
jetfighter
hiway
indy4
pinpong
pong
pongdoubles
quadrapong
rebound
sharkjaws
spacerace
steeplechase
stuntcycle
tvbasketball
wipeout

And parameters is any optional combination of the following:

-window : Start the emulator in a window instead of fullscreen mode.



ROM files should be placed in a subfolder named "roms" in the directory where 
the DICE executable is located. Please do not attempt to contact the DICE team 
to request ROM files.



2) Compiling

DICE makes use of some C++11 features, so GCC 4.7 or newer is needed to compile.
The SDL headers are also required, available at http://www.libsdl.org.

Currently DICE supports Windows (using MinGW to compile), Linux, and OS X (preliminarily).



3) Contact Information

Project Page: http://sourceforge.net/projects/dice/
Email: dice.emulator@gmail.com



4) License

Copyright (C) 2008-2014 DICE Team

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

