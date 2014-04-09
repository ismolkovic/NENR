CC = g++
CFLAGS = -Wall -o3

INCS = -I ./lib
SRC_DIR = ./lib
SRCS = $(shell find $(SRC_DIR) -name "*.cc")
OBJS = $(SRCS:.cc=.o)


MAIN = LAB3

all:	$(MAIN)

train:
	$(CC) gen.cc -o genTrainData

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCS) -o $(MAIN) $(OBJS) main.cc

.cc.o:
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@ 

delMain:
	rm $(MAIN)

clean:
	rm -rf $(MAIN) lib/*.o