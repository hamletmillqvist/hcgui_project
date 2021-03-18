# Description
HCGUI is a hobby project where I try to make my own Console User Interface (CUI for short).
This CUI is planned to act more like a GUI than a console application hence the GUI name.

# Building

## - Automatic
The project root directory contains scripts for performing the making automatically.

**NOTE: At the current stage I there's only a powershell variant!**

## - Manually
The project uses CMAKE in order to compile. All build files are located in `/build`,
therefore make sure that your console has changed directory to this directory before building
if you wish to perform that manually.

If you wish to generate the makefile yourself, simply CD into `/build` and run:

    cmake .

If this succeeded then the makefile has been generated successfully. Now simply run:

    make
    
*I personally use MinGW in order to compile, if you're ALSO using that then you need to run the following commands **instead**!*

    cmake . -G "MinGW Makefiles"
    mingw32-make.exe
    
# Usage

TBA
