INC = -I /usr/local/include/boost/
SRCS = main.cc hand.cc solver.cc card.cc pull.cc deck.cc
all: 
	g++ -std=c++17 $(INC) $(SRCS) -o main

debug:
	g++ -g -std=c++17 $(INC) $(SRCS) -o maindebug

clean:
	rm -f main maindebug 
