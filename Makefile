CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lcurses

.PHONY: build run pack clean

build:
	$(CC) $(CFLAGS) *.c -o Minesweeper $(LDFLAGS) 

run: build
	./Minesweeper

pack: clean
	zip -r $(GRUPA)_$(NUME)_$(PRENUME)_tema3.zip *

clean:
	rm -rf Minesweeper *.zip
	clear

