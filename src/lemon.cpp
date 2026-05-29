#include "lemon.h"

// Lemon przekazuje "twarde" dane do konstruktora klasy Fruit
Lemon::Lemon() : Fruit("Lemon", 3, 50) {}
std::string Lemon::getPath() const { return ":/images/Lemon.png"; };