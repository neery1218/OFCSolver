INC = -I ../include/
SRCS = hand.cc solver.cc pull.cc deck.cc fast_poker_hand_evaluator.cc decision_finder.cc action_enumerator.cc card.cc completed_hand.cc optimal_hand.cc advanced_decision_finder.cc advanced_solver.cc
DRIVER = main.cc
ACTIONDRIVER = actionmain.cc
CXX = g++
CFLAGS = -std=c++2a -Wall -fno-omit-frame-pointer -O3 -fconcepts
LIBS = -lpthread

solver: $(SRCS) $(DRIVER)
	$(CXX) $(CFLAGS) $(INC) $^ -o $@ $(LIBS)

action: $(SRCS) $(ACTIONDRIVER)
	$(CXX) $(CFLAGS) $(INC) $^ -o $@ $(LIBS)

clean:
	rm -f solver *.o
