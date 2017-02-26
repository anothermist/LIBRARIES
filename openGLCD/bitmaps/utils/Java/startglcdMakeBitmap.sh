#!/bin/bash
#
# This is a shell wrapper to start up the java bitmap tool
# Normally, this would not be needed as users can easily start
# the tool from the commandline or from a GUI file explorer; however....
# the Nautilus GUI does not set the working directory properly prior
# to starting up programs. What that means is that the working directory
# is ~ (users home directory) which will not work for this tool as it
# needs to run "in place" so that it can create new bitmap header files in
# the proper location and also update the "allBitmaps.h" header file in glcd/bitmaps
#

# First cd to the proper directory
cd `dirname $0`

# now crank up the bitmap tool
java -jar glcdMakeBitmap.jar
