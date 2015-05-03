QGLDEMO
-------------
The qgldemo application is a simple demonstrator of Qt's QGL functionality.
It creates a single widget with a few cubes in it, and provides a mechinism of
interacting with the widget via <ctrl>+ mouse-click (<cmd> on OSX). Essentially, it
is a small playground for me to test out new things.

The application is very much a work in progress, as I don't get as much time to 
hack away at it as I'd like.

REQUIREMENTS
-------------
To build this source you will need installed:
- Qt4.8.X
- CMake 2.8.12+

INSTRUCTIONS
-------------
- Create a build directory and navigate into it
- `ccmake path/to/my/source`
- Hit configure.
- Select your generator of choice. I personally develop on OSX and used 'unix makefiles'
- Hit generate. You may need to helpfully point CMake to your Qt installation if it is not on the PATH.
- Build using your development environment of choice. For unix users:
- `make`

KNOWN ISSUES
------------
- The mouse interactivity has a few problems with vertical motion that I haven't quite
gotten around to fixing yet. This shouldn't take too long to identify and patch up.

