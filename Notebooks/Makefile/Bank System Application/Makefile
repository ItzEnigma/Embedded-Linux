# Author: Enigma

CC = gcc
CFLAGS = -Wall -g
SRC = $(wildcard *.c)
TARGET = main
LIB_PATH = ./lib/

.PHONY: all clean build help
build: createLib $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -L $(LIB_PATH) -lSystem -I $(LIB_PATH)
	@echo "Project Compiled Successfully!"

createLib: System.o
	ar rcs $(LIB_PATH)libSystem.a System.o $(LIB_PATH)System.h
	@echo "Library Created Successfully!"

System.o:
	$(CC) $(CFLAGS) -c $(LIB_PATH)System.c

clean:
	@rm -f $(TARGET) $(LIB_PATH)*.a *.o
	@echo "Project Cleaned!"

help:
	@echo "Usage: make [clean|build|help]"
	@echo "clean: remove the target and object files"
	@echo "build: compile the project"
	@echo "help: show this message"