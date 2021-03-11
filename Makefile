#
# CSCI 315 Operating Systems Design
# Author: L. Felipe Perrone
# Date: 2011-04-21
# Copyright (c) 2011 Bucknell University
#
# Permission is hereby granted, free of charge, to any individual or
# institution obtaining a copy of this software and associated
# documentation files (the "Software"), to use, copy, modify, and
# distribute without restriction, provided that this copyright and
# permission notice is maintained, intact, in all copies and supporting
# documentation.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL BUCKNELL UNIVERSITY BE LIABLE FOR ANY CLAIM, DAMAGES
# OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
# OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# compiler
CC = gcc

CFLAGS = -g -Wall -lpthread -lm

EXE = file_stat read_dir fdump hexdump.o traverse sender receiver

all: $(EXE)

hexdump.o: hexdump.c hexdump.h
	$(CC) $(CFLAGS) -c hexdump.c

fdump: hexdump.o fdump.c
	$(CC) $(CFLAGS) fdump.c hexdump.o -o fdump

file_stat: file_stat.o
	$(CC) $(CFLAGS) -o file_stat file_stat.o

file_stat.o: file_stat.c
	$(CC) -c $(CFLAGS) file_stat.c

read_dir: read_dir.o
	$(CC) -o read_dir read_dir.o

read_dir.o: read_dir.c
	$(CC) -c $(CFLAGS) read_dir.c

traverse: read_dir.h traverse.o
	$(CC) $(CFLAGS) -o traverse traverse.o

traverse.o: traverse.c
	$(CC) -c $(CFLAGS) traverse.c

sender: sender.c
	$(CC) $(CFLAGS) sender.c -o sender

receiver: receiver.c
	$(CC) $(CFLAGS) receiver.c -o receiver 

clean:
	/bin/rm -f *~ *.o core $(EXE)
