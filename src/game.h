#pragma once
#include "drum.h"
#include "symbol.h"
#include <vector>

class Game
{
  int credits;
  int bet;
  Drum drum;

public:
  Game();          // no need for destructor. Game() is not using any memory operations
  void start();    // for console
  int playRound(); // for gui
  int getCredits() const;
  int getBet() const;
  void setBet(int bet_);
  const std::vector<Symbol *> &getCurrentSpin() const; // przekazanie gettera z drum.h dalej
};