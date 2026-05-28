#pragma once
#include "specialsymbol.h"

class Bomb : public SpecialSymbol
{
public:
    Bomb();
    ~Bomb() override = default;
};