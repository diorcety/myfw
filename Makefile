# Copyright (C) 2010 Ubixum, Inc. 
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA



# change location of fx2libdir if needed
SOURCES=fw.c device.c utils.c
A51_SOURCES=dscr.a51
BASENAME=firmware


include ../../3rd/fx2lib/lib/fx2.mk

test.o: test.cpp
	i686-w64-mingw32-g++ -std=c++0x -O0 -fno-omit-frame-pointer -g -c test.cpp
	
USB.o: USB.cpp USB.h
	i686-w64-mingw32-g++ -std=c++0x -O0 -fno-omit-frame-pointer -g -c USB.cpp
	
test: test.o USB.o
	i686-w64-mingw32-g++ -o test.exe test.o USB.o -lusb-1.0


