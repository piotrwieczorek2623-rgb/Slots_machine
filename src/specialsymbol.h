#pragma once
#include "symbol.h"

class SpecialSymbol : public Symbol
{
public:
    SpecialSymbol(std::string name, int multiplier, int weight);
    ~SpecialSymbol() override = default;
};