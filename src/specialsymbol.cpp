#include "specialsymbol.h"

SpecialSymbol::SpecialSymbol(std::string name, int multiplier, int weight)
    : Symbol(name, multiplier, weight) {}

int SpecialSymbol::calculateMultiplier(const std::vector<Symbol *> &spin) const
{
    int bombCount = 0;
    int fruitMultiplierSum = 0;
    int lastFruitMultiplier = 0;
    int sevenCount = 0;

    for (Symbol *symbol : spin)
    {
        if (symbol->getName() == "Seven")
            sevenCount++;
    }
    if (sevenCount == 3)
        return spin[0]->getMultiplier();

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