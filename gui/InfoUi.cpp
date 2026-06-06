#include "MainWindow.h"

// ==========================================
//                   INFO
// ==========================================
void MainWindow::setupInfoUi()
{
    infoWidget = new QWidget(this);
    infoWidget->setStyleSheet("background-color: #1a1a1a;"); // Ciemne tło info

    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);

    // pozycja
    infoLayout->setContentsMargins(40, 80, 40, 40);
    infoLayout->setSpacing(20);

    // Duży napis SLOT MACHINE
    titleInfoLabel = new QLabel("MULTIPLIERS", infoWidget);
    titleInfoLabel->setAlignment(Qt::AlignCenter);
    titleInfoLabel->setStyleSheet("color: #e9e9e9;");

    // FIXME
    //  infoLayout->addStretch();

    // przycisk return to menu
    returnToMenuButton = new QPushButton("RETURN TO MENU", menuWidget);
    returnToMenuButton->setFixedWidth(300);
    returnToMenuButton->setFixedHeight(80);
    returnToMenuButton->setStyleSheet(R"(
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

    QFont titleFont = titleInfoLabel->font();
    titleFont.setPointSize(64); // Powiększony pod pełny ekran
    titleFont.setBold(true);
    titleInfoLabel->setFont(titleFont);

    infoLayout->addWidget(titleInfoLabel);
    infoLayout->addWidget(returnToMenuButton, 0, Qt::AlignBottom | Qt::AlignLeft);
    connect(returnToMenuButton, &QPushButton::clicked, this, &MainWindow::onReturnInfoClicked);
}

void MainWindow::onInfoClicked()
{
    transitionFromMenuToInfo();
}

void MainWindow::onReturnInfoClicked()
{
    transitionFromInfoToMenu();
}

// info -> menu
void MainWindow::transitionFromInfoToMenu()
{
    // fade out elementów info
    fadeWidget(returnToMenuButton, 100, 0, 350);
    fadeWidget(titleInfoLabel, 100, 0, 350);
    playButton->setEnabled(true);

    // Po wygaszeniu kontrolek (350ms) czyścimy ekran, resetujemy grę i płynnie wchodzimy do menu
    QTimer::singleShot(350, this, [this]()
                       {
                           stackedWidget->setCurrentIndex(MenuScene);

                           // fade in
                           fadeWidget(titleMenuLabel, 0, 100, 350);
                           fadeWidget(playButton, 0, 100, 350);
                           fadeWidget(infoButton, 0, 100, 350); });
}