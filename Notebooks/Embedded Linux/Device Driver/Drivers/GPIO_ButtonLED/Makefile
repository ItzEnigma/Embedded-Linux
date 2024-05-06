# Build the main program and change ownership to root
#
# Usage: make
#        make clean

# Define the compiler
CC = g++

# Define the compiler flags
CFLAGS = -Wall -Wextra -Werror -std=c++11

# Define the source files
SRC = GPIO_ButtonLED.cpp

# Define the executable file
EXE = GPIO_ButtonLED

all: $(SRC)
	$(CC) $(CFLAGS) -o $(EXE) $(SRC)
	# Change the ownership of the executable to root
	sudo chown root:root $(EXE)

clean:
	rm -f $(OBJ) $(EXE)