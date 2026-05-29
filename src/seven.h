#pragma once
#include "specialsymbol.h"

class Seven : public SpecialSymbol
{
public:
    Seven();
    ~Seven() override = default;
    std::string getPath() const override;
};