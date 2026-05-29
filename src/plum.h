#pragma once
#include "fruit.h"

class Plum : public Fruit
{
public:
    Plum();
    ~Plum() override = default;
    std::string getPath() const override;
};