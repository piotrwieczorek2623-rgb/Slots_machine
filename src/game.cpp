#include "game.h"
#include <iostream>

Game::Game() : drum(3), credits(1000) {}

int Game::getCredits() const { return credits; }
int Game::getBet() const { return bet; }
void Game::setBet(int bet_) { bet = bet_; }

int Game::playRound()
{
  if (bet > credits) // allow only possible bets
    return -999;

  // credits -= bet;
  drum.spin();

  int prize = drum.win(bet);

  if (prize > 0)
    credits += prize;
  else if (prize < 0)
  {
    credits += prize; // prize jest ujemny, więc odejmuje
  }
  else
  {
    credits -= bet;
  }

  return prize;
}

const std::vector<Symbol *> &Game::getCurrentSpin() const
{
  return drum.getCurrentSpin();
}

void Game::start()
{
  int bet;

  std::cout << "=== SLOT MACHINE ===\n";

  while (credits > 0)
  {
    std::cout << "\ncredits: " << credits << " zl\n";
    std::cout << "Bet: ";
    std::cin >> bet;
    credits -= bet;

    if (bet <= 0 || bet > credits)
    {
      std::cout << "Wrong bet\n";
      continue;
    }

    drum.spin();

    int prize = drum.win(bet);

    if (prize > 0)
    {
      credits += prize;
      std::cout << "WIN: " << prize << " zl\n";
    }
    else
    {
      std::cout << "Lose: " << bet << " zl\n";
    }
  }

  std::cout << "\nGame over. No money left.\n";
}