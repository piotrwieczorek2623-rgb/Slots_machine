#include "MainWindow.h"

// ==========================================
//                MENU GŁÓWNE
// ==========================================
void MainWindow::setupMenuUi()
{
    menuWidget = new QWidget(this);
    menuWidget->setStyleSheet("background-color: #1a1a1a;"); // Ciemne tło menu

    QVBoxLayout *menuLayout = new QVBoxLayout(menuWidget);

    // 1. Duży napis SLOT MACHINE
    titleMenuLabel = new QLabel("SLOT MACHINE", menuWidget);
    titleMenuLabel->setAlignment(Qt::AlignHCenter);
    titleMenuLabel->setStyleSheet("color: #da90ff;"); // Fioletowy, neonowy kolor

    QFont titleFont = titleMenuLabel->font();
    titleFont.setPointSize(64); // Powiększony pod pełny ekran
    titleFont.setBold(true);
    titleMenuLabel->setFont(titleFont);

    // Przycisk play pod napisem
    playButton = new QPushButton("PLAY", menuWidget);
    playButton->setFixedWidth(300);
    playButton->setFixedHeight(80);
    playButton->setStyleSheet(R"(
      QPushButton {
          background-color: #da90ff;
          color: black
          border-radius: 20px;
          border: 3px solid #da75ff;
          font-size: 28px;
          font-weight: bold;
      }
      QPushButton:hover { background-color: #daacff; }
      QPushButton:pressed { background-color: #da57ff; }
  )");
    // przycisk info
    infoButton = new QPushButton("INFO", menuWidget);
    infoButton->setFixedWidth(300);
    infoButton->setFixedHeight(80);
    infoButton->setStyleSheet(R"(
      QPushButton {
          background-color: #da90ff;
          color: black;
          border-radius: 20px;
          border: 3px solid #da75ff;
          font-size: 28px;
          font-weight: bold;
      }
      QPushButton:hover { background-color: #daacff; }
      QPushButton:pressed { background-color: #da57ff; }
  )");

    // przycisk EXIT GAME
    exitGameButton = new QPushButton("EXIT GAME", menuWidget);
    exitGameButton->setFixedWidth(300);
    exitGameButton->setFixedHeight(80);
    exitGameButton->setStyleSheet(R"(
      QPushButton {
          background-color: #da90ff;
          color: black;
          border-radius: 20px;
          border: 3px solid #da75ff;
          font-size: 28px;
          font-weight: bold;
      }
      QPushButton:hover { background-color: #daacff; }
      QPushButton:pressed { background-color: #da57ff; }
  )");

    // Pozycjonowanie na środku ekranu
    menuLayout->addStretch();
    menuLayout->addWidget(titleMenuLabel);
    menuLayout->addSpacing(40);
    menuLayout->addWidget(playButton, 0, Qt::AlignCenter);
    menuLayout->addWidget(infoButton, 0, Qt::AlignCenter);
    menuLayout->addWidget(exitGameButton, 0, Qt::AlignCenter);
    menuLayout->addStretch();

    // Podłączenie kliknięcia play -> przejście do gry
    connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
    // podlaczenie przycisku info -> scena info
    connect(infoButton, &QPushButton::clicked, this, &MainWindow::onInfoClicked);
    // podlaczenie przycisku exitGame
    connect(exitGameButton, &QPushButton::clicked, this, &QWidget::close);
}

// MENU -> GRA
void MainWindow::transitionFromMenuToGame()
{
    // Blokujemy przycisk na czas animacji
    playButton->setEnabled(false);

    // Wygaszamy napisy i przyciski w menu
    fadeWidget(titleMenuLabel, 100, 0, 350);
    fadeWidget(playButton, 100, 0, 350);
    fadeWidget(infoButton, 100, 0, 350);

    // Po zakończeniu animacji (350ms) przełączamy okno i dbamy o czystość stanów
    QTimer::singleShot(350, this, [this]()
                       {
        stackedWidget->setCurrentIndex(GameScene); // Zmiana na ekran gry

        if (creditsLabel) {
            creditsLabel->move(width() - creditsLabel->width() - 20, 20);
        }

        // Przywracamy bazową przezroczystość komponentom menu, 
        // aby były gotowe na kolejny powrót do menu w przyszłości
        auto *titleEffect = qobject_cast<QGraphicsOpacityEffect *>(titleMenuLabel->graphicsEffect());
        auto *playEffect = qobject_cast<QGraphicsOpacityEffect *>(playButton->graphicsEffect());
        if (titleEffect) titleEffect->setOpacity(1.0);
        if (playEffect) playEffect->setOpacity(1.0);
        
        playButton->setEnabled(true); 
        //fade przyciskow
        fadeWidget(spinButton, 0, 100, 350);
        fadeWidget(betInput, 0, 100, 350);
        fadeWidget(betLabel, 0, 100, 350);
        fadeWidget(creditsLabel, 0, 100, 350); });
}

// MENU->INFO
void MainWindow::transitionFromMenuToInfo()
{
    // Blokujemy przycisk na czas animacji
    playButton->setEnabled(false);

    // Wygaszamy napisy i przyciski w menu
    fadeWidget(titleMenuLabel, 100, 0, 350);
    fadeWidget(playButton, 100, 0, 350);
    fadeWidget(infoButton, 100, 0, 350);

    // Po zakończeniu animacji (350ms) przełączamy okno i dbamy o czystość stanów
    QTimer::singleShot(350, this, [this]()
                       {
                           fadeWidget(titleInfoLabel, 0, 100, 350);
                           fadeWidget(returnToMenuButton, 0, 100, 350);
                           stackedWidget->setCurrentIndex(InfoScene); // Zmiana na ekran gry
                       });
}

void MainWindow::onPlayClicked()
{
    transitionFromMenuToGame();
}
