#pragma once
#include <string>
#include <vector>

class Symbol
{
  int multiplier;
  std::string name;
  int weight;

public:
  Symbol(std::string name_, int multiplier, int weight);
  virtual ~Symbol() = default;
  std::string getName() const;
  int getMultiplier() const;
  int getWeight() const;
  virtual int calculateMultiplier(const std::vector<Symbol *> &spin) const;
};