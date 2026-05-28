#pragma once
#include "symbol.h"
#include <vector>

class Drum
{

  std::vector<Symbol *>
      symbols; // Vector holds inherted symbol's classes (cherry,lemon,plum)
  std::vector<Symbol *> curr_spin;
  int drums_quantity;

  Symbol *randSymbol();

public:
  Drum(int drums_quantity);
  ~Drum();

  void spin();
  void forceWin(); // force Win is for debuging
  bool isWin() const;
  int calculateWin(int bet) const;
  int win(int bet);
  const std::vector<Symbol *> &getCurrentSpin() const; // getter
};