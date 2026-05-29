#include "bomb.h"

// Plum przekazuje "twarde" dane do konstruktora klasy Fruit
Bomb::Bomb() : SpecialSymbol("Bomb", 0, 8) {}
std::string Bomb::getPath() const { return ":/images/Bomb.png"; };

int Bomb::calculateMultiplier(const std::vector<Symbol *> &spin) const
{
    int bombCount = 0;
    int fruitMultiplierSum = 0;
    int lastFruitMultiplier = 0;

    for (Symbol *symbol : spin)
    {
        if (symbol->getName() == "Bomb")
        {
            bombCount++;
        }
        else
        {
            if (symbol->getName() != "Seven")
            {
                fruitMultiplierSum += symbol->getMultiplier();
                lastFruitMultiplier = symbol->getMultiplier();
            }
        }
    }

    if (bombCount == 1)
    {
        return fruitMultiplierSum;
    }

    if (bombCount == 2)
    {
        return lastFruitMultiplier / 2;
    }

    if (bombCount == 3)
    {
        return -2;
    }

    return 0;
}