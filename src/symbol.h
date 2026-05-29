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
  virtual std::string getPath() const = 0;
  virtual ~Symbol() = default;
  std::string getName() const;
  int getMultiplier() const;
  int getWeight() const;
  //nie dodaje =0 na koncu aby byla standardowa definicja funkcji
  virtual int calculateMultiplier(const std::vector<Symbol *> &spin) const;
};