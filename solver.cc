#include "solver.h"

using namespace std;

Solver::Solver() {}

Decision Solver::solve(Hand &myHand, Pull &myPull, vector<Hand> otherHands, 
    vector<Decision> decisions, GameType type) {
  return decisions[0];
}

ostream& operator<<(ostream& os, const Placement &p) {
  string pos = "top";
  if (p.position == Position::middle) { pos = "middle"; }
  if (p.position == Position::bottom) { pos = "bottom"; }
  if (p.position == Position::dead) { pos = "dead"; }

  os << p.card << " " << pos;
  return os;
}

ostream& operator<<(ostream& os, const Decision &d) {
  os << d.placements.at(0) << endl << d.placements.at(1)  << endl << d.placements.at(2);
  return os;
}
