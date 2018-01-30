# Porting libjpeg to microcontrollers (without OS)

## How to porting
- Copy Libs folder to your project
- Add all the source files to the project
- Add include path

## Original code
http://www.ijg.org/

jpegsr9c.zip (http://www.ijg.org/files/jpegsr9c.zip)

## What was modified
- created `jconfig.h` based on configuration for mingw
- deleted unnecessary files:
	- sample code
	- source cods for cjpeg, djpeg, jpegtran, rdjpgcom, wrjpgcom
- deleted jmemXXX.c except for jmemansi.c
- modified jmorecfg.h
	- the definition of boolean type, TRUE and FALSE for C++ environment(g++)
	- this is not mandatory if you use gcc
- created dummy_systemcalls.c
	- dummy systen call functions
	- you may be able to use  `--specs=nosys.specs` option if available
	- without this, I got the following error:
		- unlinkr.c:(.text+0x1c): undefined reference to `_unlink'

### You may need to modify the following files depending on your system, and performance and memory usage
- dummy_systemcalls.c
- jinclude.h
- jmorecfg.h

## Note
I confirmed this code with ZYBO (Zynq, Cortex-A9) in baremetal environment in Xilinx SDK.
This code should work for other microcontrollers.
