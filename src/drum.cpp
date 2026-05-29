#include "drum.h"
#include "specialsymbol.h"
#include "lemon.h"
#include "cherry.h"
#include "plum.h"
#include "bomb.h"
#include "seven.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Drum::Drum(int drums_quantity_)
{
  drums_quantity = drums_quantity_;

  symbols.push_back(new Lemon());
  symbols.push_back(new Cherry());
  symbols.push_back(new Plum());
  symbols.push_back(new Bomb());
  symbols.push_back(new Seven());
  srand(time(NULL));
}

Drum::~Drum()
{
  for (int i = 0; i < symbols.size(); i++)
  {
    Symbol *s = symbols[i];
    delete s;
  }
  symbols.clear();
}

// rand z wagami
Symbol *Drum::randSymbol()
{
  int totalWeight = 0;

  for (Symbol *symbol : symbols)
  {
    totalWeight += symbol->getWeight();
  }

  int randomValue = rand() % totalWeight;

  int currentWeight = 0;

  for (Symbol *symbol : symbols)
  {
    currentWeight += symbol->getWeight();

    if (randomValue < currentWeight)
    {
      return symbol;
    }
  }

  return symbols[0];
}

// fills curr_spin vector with random symbols
void Drum::spin()
{
  curr_spin.clear();
  for (size_t i = 0; i < drums_quantity; i++)
  {
    curr_spin.push_back(randSymbol());
    std::cout << curr_spin[i]->getName() << " ";
  }
  std::cout << std::endl;
}

void Drum::forceWin()
{
  curr_spin.clear();
  curr_spin.push_back(symbols[1]);
  curr_spin.push_back(symbols[1]);
  curr_spin.push_back(symbols[1]);
}

bool Drum::isWin() const
{
  // protection in case size of vector is <2
  if (curr_spin.size() < 2)
    return false;
  // bomba przypadek
  for (size_t i = 0; i < curr_spin.size(); i++)
  {
    if (curr_spin[i]->getName() == "Bomb")
      return true;
  }
  // comparing name of symbols (size_t is return type of size() function)
  for (size_t i = 0; i < curr_spin.size() - 1; i++)
  {
    if (curr_spin[i]->getName() != curr_spin[i + 1]->getName())
      return false;
  }
  return true;
}

// win dla roznych symboli
int Drum::calculateWin(int bet) const
{
  if (!isWin())
  {
    return 0;
  }

  for (Symbol *symbol : curr_spin)
  {
    if (symbol->getName() == "Bomb")
    {
      return bet * symbol->calculateMultiplier(curr_spin);
    }
  }

  return (bet * curr_spin[0]->calculateMultiplier(curr_spin));
}
// int Drum::calculateWin(int bet) const
// {
//   if (!isWin())
//     return 0;
//   return (bet * curr_spin[0]->getMultiplier());
// }

int Drum::win(int bet)
{
  if (!isWin())
    return 0;
  return calculateWin(bet);
}

const std::vector<Symbol *> &Drum::getCurrentSpin() const { return curr_spin; }