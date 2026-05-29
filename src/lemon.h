#pragma once
#include "fruit.h"

class Lemon : public Fruit
{
public:
    Lemon();
    ~Lemon() override = default;
    std::string getPath() const override;
};