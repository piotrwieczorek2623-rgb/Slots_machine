#include "symbol.h"

Symbol::Symbol(std::string name_, int multiplier_, int weight_)
{
  name = name_;
  multiplier = multiplier_;
  weight = weight_;
};

std::string Symbol ::getName() const { return name; };
int Symbol ::getMultiplier() const { return multiplier; };
int Symbol ::getWeight() const { return weight; };
//domyslna funkcja dla symboli ktore zachowują sie standardowo(owoce i siodemka)
//czyli jesli w klasach nic sie nie nadpisuje to uzywa standardowej definicji
//a jesli sie nadpisuje to korzysta z definicji konkretnej klasy
int Symbol::calculateMultiplier(const std::vector<Symbol *> &spin) const{ return multiplier;}