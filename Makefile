CC=g++
CFLAGS = -std=c++11
EXEC=mp3spider
MAIN=src/main.cpp

main:$(MAIN)
	$(CC) $? $(CFLAGS) -o build/$(EXEC) 

run:
	build/$(EXEC)

clear:
	rm build/* 

uninstall:
	sudo rm -f /usr/local/bin/mp3spider

install:
	sudo cp build/$(EXEC) /usr/local/bin/mp3spider
