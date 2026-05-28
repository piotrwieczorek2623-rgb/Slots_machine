#pragma once
#include "symbol.h"

class Fruit : public Symbol
{

public:
    Fruit(std::string name, int multiplier, int weight);
    ~Fruit() override = default;
};