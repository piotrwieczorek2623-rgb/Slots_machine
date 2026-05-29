#pragma once
#include "specialsymbol.h"

class Bomb : public SpecialSymbol
{
public:
    Bomb();
    ~Bomb() override = default;
    std::string getPath() const override;
    int calculateMultiplier(const std::vector<Symbol *> &spin) const override;
};
