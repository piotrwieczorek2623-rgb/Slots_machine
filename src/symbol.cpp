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

int Symbol::calculateMultiplier(const std::vector<Symbol *> &spin) const
{
  return multiplier;
}