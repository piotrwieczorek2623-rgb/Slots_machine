#pragma once
#include "symbol.h"

class SpecialSymbol : public Symbol
{
public:
    SpecialSymbol(std::string name, int multiplier, int weight);
    int calculateMultiplier(const std::vector<Symbol *> &spin) const override;
    ~SpecialSymbol() override = default;
};