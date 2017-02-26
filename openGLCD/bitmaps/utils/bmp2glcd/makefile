#
#  BMP2GLCD - BMP to GLCD image makefile
#                       
#       Author: Sami Varjo 
# file creatd : 31.7.2008
#      version: 1.0
#                                                
#   License: GPL which can be found at
#	     http://www.gnu.org/licenses/gpl.txt
#                                                
# description: makefile for compiling bmp2glcd program with the EasyBMP library. 
#

CC = g++
BILLFLGS = -Wformat=2
CFLAGS = $(BILLFLGS) -O3 -pipe -fomit-frame-pointer -funroll-all-loops -s
CINC = -I ./EasyBMP/

bmp2glcd: EasyBMP.o bmp2glcd.o 
	g++ $(CFLAGS) EasyBMP.o bmp2glcd.o -o bmp2glcd

EasyBMP.o: EasyBMP/EasyBMP.cpp EasyBMP/EasyBMP*.h
	g++ $(CFLAGS) -c EasyBMP/EasyBMP.cpp

bmp2glcd.o: bmp2glcd.cpp
	g++ $(CINC) -c bmp2glcd.cpp

clean: 
	rm -f bmp2glcd
	rm -f *.o
	rm -f *~ \#*\#

