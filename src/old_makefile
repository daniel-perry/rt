# Daniel Perry
# makefile for my raytracer (rt) started in cs6620
CC = g++
DEBUG = -Wall -g -Wno-deprecated
CFLAGS = -Wall -O4 -Wno-deprecated


rt : rt.cc glib
	$(CC) -o rt $(CFLAGS) rt.cc gl/*.o

debug : rt.cc glib
	$(CC) -o rt $(DEBUG) rt.cc gl/*.o
	echo "DON'T FORGET TO CHANGE FLAGS IN gl!!!"

glib : 
	$(MAKE) -C gl

.PHONY : clean
clean : 
	$(MAKE) -C gl clean
	rm rt
 
