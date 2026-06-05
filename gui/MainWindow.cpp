#include "MainWindow.h"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  // Inicjalizacja logiki gry
  gameLogic = new Game();

  // Główny manager ekranów
  stackedWidget = new QStackedWidget(this);
  setCentralWidget(stackedWidget);

  // Budujemy oba widoki
  setupMenuUi();
  setupGameUi();
  setupInfoUi();

  // Dodajemy ekrany do managera (Menu = 0, Gra = 1, info=2)
  stackedWidget->addWidget(menuWidget);
  stackedWidget->addWidget(gameWidget);
  stackedWidget->addWidget(infoWidget);

  // Startujemy od Menu Głównego
  stackedWidget->setCurrentIndex(MenuScene);

  // Wyjście z aplikacji za pomocą klawisza ESCAPE
  new QShortcut(QKeySequence(Qt::Key_Escape), this, [this]()
                { close(); });

  // WYMUSZENIE TRYBU PEŁNOEKRANOWEGO DLA CAŁEGO OKNA
  showFullScreen();
}




