INC = -I /usr/local/include/boost/
SRCS = hand.cc solver.cc card.cc pull.cc deck.cc poker_hand_evaluator.cc
DRIVER = main.cc
TESTS = test_hand.cc
all: 
	g++ -std=c++17 -O3 $(INC) $(SRCS) $(DRIVER) -o main

test: testmain
	g++ -std=c++17 -O3 test_main.o $(INC) $(SRCS) $(TESTS) -o maintest

debug:
	g++ -g -std=c++17 $(INC) $(SRCS) $(DRIVER) -o maindebug

profile:
	g++ -pg -O3 -std=c++17 $(INC) $(SRCS) $(DRIVER) -o mainprof

clean:
	rm -f main maindebug mainprof maintest

testmain:
	g++ -std=c++17 -O3 test_main.cc -c

fantasysolver: all
	g++ -std=c++17 -O3 $(INC) $(SRCS) fantasy_simulator.cc -o fantasysolver

fantasysolverdebug:
	g++ -g -std=c++17 -O3 $(INC) $(SRCS) fantasy_simulator.cc -o fantasysolverdebug
