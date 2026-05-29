#include "cherry.h"

// Cherry przekazuje "twarde" dane do konstruktora klasy Fruit
Cherry::Cherry() : Fruit("Cherry", 4, 40) {}
std::string Cherry::getPath() const { return ":/images/Cherry.png"; };