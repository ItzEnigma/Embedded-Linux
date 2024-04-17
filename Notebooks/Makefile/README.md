# Contents

- [Contents](#contents)
  - [Makefile](#makefile)
    - [How it works?](#how-it-works)
    - [Why use Make?](#why-use-make)
  - [Syntax](#syntax)
    - [`set of rules:`](#set-of-rules)
    - [`Variables:`](#variables)
      - [Automatic Variables](#automatic-variables)
    - [Functions](#functions)
      - [Built-in Functions](#built-in-functions)
    - [Loops](#loops)
    - [`.Phony` Targets](#phony-targets)
    - [Include Files](#include-files)
  - [Compilation](#compilation)
  - [Making Libraries](#making-libraries)

---

## Makefile

**`Make`** is a **build automation tool** _(scripting tool)_ that builds **executable programs** and **libraries** from **source code** by reading files called _`makefiles`_ which specify how to derive the target program **using `commands`**. Used to help decide **which parts of a large program need to be recompiled**. In the vast majority of cases, C or C++ files are compiled. Other languages typically have their own tools that serve a similar purpose as Make.

Popular **_C/C++_** alternative build systems are `SCons`, `CMake`, `Bazel`, and `Ninja`. Some IDEs like `Microsoft Visual Studio` have their own built in build tools. For Java, there's Ant, Maven, and Gradle. Other languages like Go, Rust, and TypeScript have their own build tools.

**Interpreted languages** like `Python`, `Ruby`, and `raw Javascript` don't require an analogue to Makefiles. The goal of Makefiles is to **compile** whatever files need to be compiled, based on what files have changed. But when files in interpreted languages change, nothing needs to get recompiled. When the program runs, the most recent version of the file is used.

### How it works?

Make tool parses the `makefile` to determine which files need to be updated and issues commands to update them. _(It is a bit like a shell script with some extra syntax to make it easier for resolving dependencies.)_

**Types of Make:**

- **GNU Make:** The most widely used version of Make.
- **BSD Make:** The version of Make that comes with BSD Unix systems.
- **CMake:** A cross-platform build system that generates Makefiles.

### Why use Make?

- **Automation:** Make can automatically build your project from source files.
- **Dependency Management:** Make can automatically determine which files need to be recompiled.
- **Portability:** Make is available on most Unix-like systems.
- **Speed:** Make can build large projects quickly by only recompiling files that have changed.
- **Flexibility:** Make can be used to build any type of project, not just C or C++ programs.
- **Customization:** Make can be customized to build your project in any way you want. (e.g. `make test`, `make install`, `make clean`)

---

## Syntax

> :heavy_exclamation_mark: **Makefiles must be indented using `TABs` and not `spaces` or make will fail.**
>
> :grey_exclamation: Makefiles can access **`Environment Variables`**, **`Shell Commands`** and **`External Scripts/Binaries`**.

### `set of rules:`

Each rule has a `target`, `dependencies`, and a `commands`.

- **Target:** The filename _(executable/object file/a name)_ that the rule builds. _(typically, there is only one per rule)_
- **Dependencies:** The files that the target depends on. _(if any of these files change, the target will be rebuilt)_
- **Commands:** Series of steps typically used to **make the target(s)**. These need to start with a tab character, not spaces.

```make
# Two rules: display and all

# target: dependencies
#     command1
#     command2
#     command3
display:
    echo "Hello, World!"
all: display
     echo "Done!"
```

---

### `Variables:`

Variables can be defined at the top of the makefile and used throughout the file. _(e.g. `CC = gcc`, `CFLAGS = -Wall -Wextra -g`)_ **(Variables can only be strings.)**

```make
msg = Compiling
CC = gcc
CFLAGS = -Wall -Wextra -g
msg += main.c
display:
    # ${} or $() can be used to reference variables
    echo ${msg}
    $(CC) $(CFLAGS) main.c -o main.o
```

> :grey_exclamation: **Disable Echoing Commands:** by using **`@`** (e.g. `@echo "Hello, World!"`)

#### Automatic Variables

- **`$@`** - The filename representing the target.
- **`$<`** - The filename of the first prerequisite.
- **`$^`** - The names of all the prerequisites, separated by spaces, discard duplicates.
- **`$+`** - Similar to `$^`, but includes duplicates.

```make
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

# Functions
define info1
  @echo "This is Makefile v0.1"
endef

define info2
  @echo "Made by Enigma"
endef
```

---

### Functions

Functions are mainly just for text processing. Call functions with `$(fn, arguments)` ... The result of the function’s processing is substituted into the makefile at the point of the call, just as a variable might be substituted.

:bulb: You can define a function using `define` and `endef` keywords.

```make
# Define a function
define test
  # $0 is the function name
  # $1 is the first argument passed to the function
  # $@ is the target
  echo "${1} Hello World2 ${@}"
endef

all:
  # Call the function using $(call function_name, arguments) if you want to pass arguments
  $(call test,"argument1")
  echo "Hello World"
```

> :grey_exclamation: You can use `$1` or `${1}` both are valid, and note that you have to use **call** to call your function.

#### Built-in Functions

Used to perform various operations such as **text processing**, **file operations**, **flow control**, ...etc.

- **`$(shell command)`** - Run a shell command and return its output.
- **`$(wildcard pattern)`** - Return a list of files that match a pattern.
- **`$(foreach var, list, text)`** - Iterate over a list and return text.
- **`$(if condition, then, else)`** - Return then if condition is true, else otherwise.
- **`$(filter pattern, list)`** - Return a list of items that match a pattern.
- **`$(subst find, replace, text)`** - Replace **all occurances** of `find` with `replace` in given **text**.
- **`$(patsubst pattern, replace, text)`** - Replace **last occurance** of the `pattern` with `replace` in given **text**.
- **`$(sort list)`** - Sort a list of items _(alphabetically)_ **removing duplicates**.
- **`$(firstword list)`** - Return the first word in a list.
- **`$(lastword list)`** - Return the last word in a list.
- **`$(words list)`** - Return the number of words in a list.
- **`$(word index, list)`** - Return the word at the given index in a list.
- **`$(wordlist start, end, list)`** - Return the words starting from the given index in a list.
- **`$(strip text)`** - Strip the **leading**, **trailing** whitespaces and remove **extra spaces** between words.
- **`$(findstring find, text)`** - Find a string in the given text.

```make

my_target:
  # Replace all occurances of .c with .cpp in the given text
  echo $(subst .c,.cpp, main.c lib1.c lib2.c2.c lib3.c.c)

  # Replace last occurance of .c with .cpp in the given text
  echo $(patsubst %.c,%.cpp, main.c lib1.c lib2.c2.c lib3.c.c)
  # Output --> main.cpp lib1.cpp lib2.c2.cpp lib3.c.c

  # Sort the list of items removing duplicates
  echo $(sort main.c lib1.c lib2.c2.c lib3.c.c main.c)
  # Output --> lib1.c lib2.c2.c lib3.c.c main.c

  # Get the first word in the list
  echo $(firstword main.c lib1.c lib2.c2.c lib3.c.c)

  # Get the last word in the list
  echo $(lastword main.c lib1.c lib2.c2.c lib3.c.c)

  # Get the number of words in the list
  echo $(words main.c lib1.c lib2.c2.c lib3.c.c)

  # Get the word at the given index
  echo $(word 2, main.c lib1.c lib2.c2.c lib3.c.c)

  # Get the words starting from the given index
  echo $(wordlist 2, 4, main.c lib1.c lib2.c2.c lib3.c.c)

  # Strip the leading, trailing whitespaces and remove extra spaces between words
  echo $(strip main.c  lib1.c lib2.c2.c   lib3.c.c  )
  # Output --> main.c lib1.c lib2.c2.c lib3.c.c

  # Find string in the given text
  echo $(findstring lib12, main.c lib1.c lib2.c2.c lib3.c.c)
```

> :exclamation: Becareful of spaces between arguments as they add up to the output.
>
> :exclamation: Note that **`# Comments`** show in the output.

### Loops

You can use bash-script loops and foreach loops in Makefiles.

```make
list = main.c lib1.c lib2.c2.c lib3.c.c
list2 = $(foreach word, $(list), $(word).tmp)
my_target2:
  @for word in $(list); do \
    echo $$word; \
  done
  echo $(list2)
```

### `.Phony` Targets

adding `.PHONY` to a target will prevent Make from confusing the phony target with a filename

```make
# Even if there is a file named clean, make clean will still run the command
.PHONY: clean
clean:
    rm -f *.o
    rm -f *.exe
```

### Include Files

You can include other makefiles using the `include` directive.

```make
include other.mk
```

## Compilation

There are **built-in rules** for compiling C and C++ files. You can use these rules to compile your source files. _(these will be used if you don't specify your own rules)_ and generate object files.

:grey_exclamation: **Note that the following are combination of different methods to compile C++ files.**

```make
# Set the compiler variable to compile for C++ files
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
# The built-in rules are used when there is no rule defined for the target
# Here ... the compilation is done using the built-in rules
build2: ${var}
  @echo "Compiling using built-in rules"

# Generating the output fiel using the generated .o files from built-in rules
build_method2: build2
$(CC)   $(CFLAGS) $(var) -o main_build2
```

## Making Libraries

You can also create libraries using Makefiles. Here is an example of how to create a static library.

```make
# Set the compiler variable to compile for C++ files
CC = g++
# -Wall: Show all warnings, -g: Debugging information
CFLAGS = -Wall -g

# [r - replace], [c - create], [s - index ( This option updates the symbol table in library)]
# Note: that the built-in rule for creating the object files is used.
createLib: file1.o file2.o file3.o
  ar rcs libmyLib.a file1.o file2.o file3.o file1.h file2.h file3.h

build: createLib
# -I --> Include directory
# -L --> Library directory
# -l --> Library name (l replaces lib and .a extension)
  $(CC) $(CFLAGS) -I . main.cpp -o main -L . -lmyLib
```
