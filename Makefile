INC = -I /usr/local/include/boost/
SRCS = hand.cc solver.cc card.cc pull.cc deck.cc poker_hand_evaluator.cc decision_finder.cc fantasy_solver.cc set_decision_finder.cc
DRIVER = main.cc
TESTS = test_hand.cc test_decision_finder.cc
all: 
	g++ -std=c++17 -Wall -O2 $(INC) $(SRCS) $(DRIVER) -o main -lpthread

test: test_main.o
	g++ -std=c++17 -O3 test_main.o $(INC) $(SRCS) $(TESTS) -o maintest

debug:
	g++ -g -O2 -std=c++17 $(INC) $(SRCS) $(DRIVER) -o maindebug -lpthread

profile:
	g++ -pg -O3 -std=c++17 $(INC) $(SRCS) $(DRIVER) -o mainprof

clean:
	rm -f main maindebug mainprof maintest

test_main.o: test_main.cc
	g++ -std=c++17 -O3 test_main.cc -c

fantasysolver: all
	g++ -std=c++17 -O3 $(INC) $(SRCS) fantasy_simulator.cc -o fantasysolver

fantasysolverdebug:
	g++ -g -std=c++17 -O3 $(INC) $(SRCS) fantasy_simulator.cc -o fantasysolverdebug
