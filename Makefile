CC=g++
CFLAGS = 
EXEC=run
MAIN=src/main.cpp

main:$(MAIN)
	$(CC) $? $(CFLAGS) -o build/$(EXEC) 

run:
	build/$(EXEC)

clear:
	rm build/* 
