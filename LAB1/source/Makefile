# define compiler to use
CC = g++-4.8

# define any compile-time flags
CFLAGS = -Wall -o3 -std=c++11

# define any directories containing header files other than /usr/include
INCLUDES = -I ./inc/
SRC_DIR = ./lib/

# define the C++ source files
SRCS = $(shell find $(SRC_DIR) -name "*.cc")
OBJS = $(SRCS:.cc=.o)

OBJS1 = ./lib/domain.o ./lib/fuzzyset.o ./lib/func.o ./lib/control.o ./lib/rules.o

# define the executable file 
MAIN = LAB1
CONTROL = fuzzyControl

all:	$(MAIN) $(CONTROL)

$(CONTROL): $(OBJS1)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(CONTROL) $(OBJS1) fuzzyCtr.cc

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) main.cc

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@ 

clean:
	rm -rf lib/*.o $(MAIN) $(CONTROL)
