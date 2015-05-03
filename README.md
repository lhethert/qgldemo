QGLDEMO
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

