my_target:
	@# Replace all occurances of .c with .cpp in the given text
	@echo $(subst .c,.cpp, main.c lib1.c lib2.c2.c lib3.c.c)

	# Replace last occurance of .c with .cpp in the given text
	@echo $(patsubst %.c,%.cpp, main.c lib1.c lib2.c2.c lib3.c.c)
	# Output --> main.cpp lib1.cpp lib2.c2.cpp lib3.c.c

	# Sort the list of items
	@echo $(sort main.c lib1.c lib2.c2.c lib3.c.c main.c)

	# Get the first word in the list
	@echo $(firstword main.c lib1.c lib2.c2.c lib3.c.c)

	# Get the last word in the list
	@echo $(lastword main.c lib1.c lib2.c2.c lib3.c.c)

	# Get the number of words in the list
	@echo $(words main.c lib1.c lib2.c2.c lib3.c.c)

	# Get the word at the given index
	@echo $(word 2, main.c lib1.c lib2.c2.c lib3.c.c)

	# Get the words starting from the given index
	@echo $(wordlist 2, 4, main.c lib1.c lib2.c2.c lib3.c.c)

	# Strip the leading and trailing whitespaces
	@echo $(strip main.c  lib1.c lib2.c2.c   lib3.c.c  )

	# Find string in the given text
	@echo $(findstring lib12, main.c lib1.c lib2.c2.c lib3.c.c)

list = main.c lib1.c lib2.c2.c lib3.c.c
list2 = $(foreach word, $(list), $(word).tmp)
foo = ok
my_target2:
	@for word in $(list); do \
		echo $$word; \
	done
	echo $(list2)

# -----------------------------------------
# Compiling for C++ files
# -----------------------------------------

# Set the compiler variable
CC = g++
# -Wall: Show all warnings, -g: Debugging information
CFLAGS = -Wall -g

# Get all the files with .cpp extension
files = $(wildcard *.cpp)

# Replace all .cpp with .o
var = $(files:.cpp=.o)

# -----------------------------------------
# First Method #1: Using explicit rules
# -----------------------------------------
build: main.cpp
	# Basic way of compiling #1
	$(CC) $(CFLAGS) main.cpp -o main
	
	# Using automatic variables #2
	$(CC) $(CFLAGS) $^ -o $@

	# Using wildcard #3
	$(CC) $(CFLAGS) $(files) -o main

# -----------------------------------------
# Second Method #2: Using built-in rules
# -----------------------------------------
# Using built-in rules
# The built-in rules are used when there is no rule defined for the target
# Here ... the compilation is done using the built-in rules
build2: ${var}
	@echo "Compiling using built-in rules"

# Generating the output fiel using the generated .o files from built-in rules
build_method2: build2
	$(CC) $(CFLAGS) $(var) -o main_build2


	

# Even if there is a file named clean, make clean will still run the command
.PHONY: clean
clean:
	rm -f *.tmp
	rm -f *.exe
	rm -f *.o

define info1
	@echo "This is Makefile v0.1"
endef

define info2
	@echo "Made by Enigma"
endef

.PHONY: help
help:
	@echo "This is $@ for Makefile"
	$(info1)
	$(info2)
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  my_target: Run the target"
	@echo "  my_target2: Run the target2"
	@echo "  clean: Clean the files"
	# Shows prerequisites (dependencies) of the target
	@echo "First Prerequisite: $^"
	@echo "Prerequisites: $<"
