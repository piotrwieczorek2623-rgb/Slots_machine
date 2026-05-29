#pragma once
#include "fruit.h"

class Cherry : public Fruit
{
public:
    Cherry();
    ~Cherry() override = default;
    std::string getPath() const override;
};