CC = g++
CFLAGS = -Wall -O3 -std=c++0x

INCS = -I ./lib
SRC_DIR = ./lib
SRCS = $(shell find $(SRC_DIR) -name "*.cc")
OBJS = $(SRCS:.cc=.o)


MAIN = LAB2

all:	$(MAIN)

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCS) -o $(MAIN) $(OBJS) main.cc

.cc.o:
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@ 

cLAB2:
	rm $(MAIN)

clean:
	rm -rf $(MAIN) lib/*.o 
