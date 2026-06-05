#include "MainWindow.h"

// ==========================================
//             WŁAŚCIWY EKRAN GRY
// ==========================================
void MainWindow::setupGameUi()
{
    gameWidget = new QWidget(this);
    gameWidget->setStyleSheet("background-color: #232323;");

    // VIDEO MP4 (Teraz osadzone wewnątrz gameWidget)
    videoWidget = new QVideoWidget(gameWidget);
    videoWidget->setGeometry(gameWidget->rect());
    videoWidget->hide();

    player = new QMediaPlayer(this);
    player->setVideoOutput(videoWidget);

    auto *audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(1.0);
    player->setAudioOutput(audioOutput);

    QString videoPath = QDir(QCoreApplication::applicationDirPath())
                            .filePath("../resources/videos/spin.mp4");
    player->setSource(QUrl::fromLocalFile(videoPath));
    videoWidget->raise();

    // Dźwięk wygranej
    QString soundPath = QDir(QCoreApplication::applicationDirPath())
                            .filePath("../resources/sounds/coin_win.wav");
    winSound = new QMediaPlayer(this);
    winSoundOutput = new QAudioOutput(this);
    winSoundOutput->setVolume(1.0);
    winSound->setAudioOutput(winSoundOutput);
    winSound->setSource(QUrl::fromLocalFile(soundPath));

    // Główny layout gry: lewa strona automat, prawa panel
    QHBoxLayout *mainLayout = new QHBoxLayout(gameWidget);
    mainLayout->setContentsMargins(0, 0, 5, 115);
    mainLayout->setSpacing(40);

    // LEWA STRONA — AUTOMAT
    QLabel *machineLabel = new QLabel(gameWidget);
    QPixmap machinePixmap(":/images/machine_hq_yel.png");
    machineLabel->setPixmap(machinePixmap);
    machineLabel->setScaledContents(true);
    machineLabel->setMinimumSize(900, 700);
    mainLayout->addWidget(machineLabel, 4);

    // BEBNY
    reel1 = new QLabel(machineLabel);
    reel2 = new QLabel(machineLabel);
    reel3 = new QLabel(machineLabel);

    reel1->setFixedSize(300, 300);
    reel2->setFixedSize(300, 300);
    reel3->setFixedSize(300, 300);

    reel1->move(265, 260);
    reel2->move(505, 260);
    reel3->move(750, 260);

    reel1->setAlignment(Qt::AlignCenter);
    reel2->setAlignment(Qt::AlignCenter);
    reel3->setAlignment(Qt::AlignCenter);

    reel1->setStyleSheet("background: transparent;");
    reel2->setStyleSheet("background: transparent;");
    reel3->setStyleSheet("background: transparent;");

    reel1Effect = new QGraphicsOpacityEffect(reel1);
    reel2Effect = new QGraphicsOpacityEffect(reel2);
    reel3Effect = new QGraphicsOpacityEffect(reel3);

    reel1->setGraphicsEffect(reel1Effect);
    reel2->setGraphicsEffect(reel2Effect);
    reel3->setGraphicsEffect(reel3Effect);

    reel1Effect->setOpacity(1.0);
    reel2Effect->setOpacity(1.0);
    reel3Effect->setOpacity(1.0);

    reel1->hide();
    reel2->hide();
    reel3->hide();

    // PRAWA STRONA — PANEL
    QWidget *rightPanel = new QWidget(gameWidget);
    panelLayout = new QVBoxLayout(rightPanel);
    panelLayout->setContentsMargins(20, 80, 20, 80);
    panelLayout->setSpacing(40);

    // Credit label
    creditsLabel = new QLabel(gameWidget);
    creditsLabel->setText("Credits: 1000");
    creditsLabel->setStyleSheet(R"(
    color: white;
    background-color: rgba(0, 0, 0, 150);
    border-radius: 10px;
    padding: 8px 15px;
  )");
    creditsLabel->setFont(QFont("Arial", 18, QFont::Bold));
    creditsLabel->adjustSize();

    // BET LABEL
    betLabel = new QLabel("BET", gameWidget);
    betLabel->setAlignment(Qt::AlignCenter);
    QFont betFont;
    betFont.setPointSize(38);
    betFont.setBold(true);
    betLabel->setFont(betFont);

    // MESSAGE LABEL
    messageLabel = new QLabel(gameWidget);
    messageLabel->setStyleSheet("color: #ff6666; font-size: 20px; font-weight: bold;");
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->hide();

    // WIN LABEL
    winLabel = new QLabel(gameWidget);
    winLabel->setAlignment(Qt::AlignCenter);
    winLabel->setStyleSheet(R"(
    color: white;
    background-color: #00cc44;
    border: 4px solid #7CFC00;
    border-radius: 25px;
    font-size: 48px;
    font-weight: bold;
    padding: 30px;
  )");
    winLabel->hide();

    // Timer do komunikatów błędu
    messageTimer = new QTimer(this);
    messageTimer->setSingleShot(true);
    connect(messageTimer, &QTimer::timeout, this, [this]()
            {
    fadeWidget(messageLabel, 100, 0, 500);
    QTimer::singleShot(500, this, [this]() { messageLabel->hide(); }); });

    // BET INPUT
    betInput = new QLineEdit(gameWidget);
    betInput->setAlignment(Qt::AlignCenter);
    betInput->setPlaceholderText("SET BET");
    QFont inputFont;
    inputFont.setPointSize(28);
    betInput->setFont(inputFont);
    betInput->setFixedHeight(80);

    // SPIN BUTTON
    spinButton = new QPushButton("SPIN", gameWidget);
    spinButton->setStyleSheet(R"(
      QPushButton {
          background-color: #da90ff;
          color: black;
          border-radius: 15px;
          padding: 10px;
          border: 2px solid #da75ff;
      }
      QPushButton:hover { background-color: #daacff; }
      QPushButton:pressed { background-color: #da57ff; }
  )");
    QFont spinFont;
    spinFont.setPointSize(42);
    spinFont.setBold(true);
    spinButton->setFont(spinFont);
    spinButton->setFixedHeight(130);

    panelLayout->addStretch();
    panelLayout->addWidget(betLabel);
    panelLayout->addWidget(betInput);
    panelLayout->addWidget(messageLabel);
    panelLayout->addSpacing(30);
    panelLayout->addWidget(spinButton);
    panelLayout->addStretch();

    mainLayout->addWidget(rightPanel, 1);

    creditsLabel->raise();
    creditsLabel->show();

    // PODŁĄCZENIE AKCJI GRY
    connect(spinButton, &QPushButton::clicked, this, &MainWindow::startSpinAnimation);

    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status)
            {
    if (status == QMediaPlayer::EndOfMedia) {
      videoWidget->hide();
    } });

    connect(spinButton, &QPushButton::clicked, this, [=]()
            { QTimer::singleShot(300, this, [this]()
                                 {
      creditsLabel->setText("Credits: " + QString::number(gameLogic->getCredits()));
      creditsLabel->adjustSize();
      creditsLabel->move(width() - creditsLabel->width() - 20, 20);
      
      const auto &spin = gameLogic->getCurrentSpin();
      QLabel *reels[3] = {reel1, reel2, reel3};
      for (int i = 0; i < spin.size() && i < 3; i++) {
        QString path = QString::fromStdString(spin[i]->getPath());
        QPixmap symbolPixmap(path);
        reels[i]->setPixmap(symbolPixmap.scaled(reels[i]->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
      } }); });
}


//transition

// 2. DEDYKOWANE PRZEJŚCIE: GRA -> MENU (PO BANKRUCTWIE)
void MainWindow::transitionFromGameToMenu()
{
    // Płynnie wygaszamy elementy interfejsu gry, które są sterowane przez fade
    fadeWidget(spinButton, 100, 0, 350);
    fadeWidget(betInput, 100, 0, 350);
    fadeWidget(betLabel, 100, 0, 350);
    fadeWidget(creditsLabel, 100, 0, 350);

    // Po wygaszeniu kontrolek (350ms) czyścimy ekran, resetujemy grę i płynnie wchodzimy do menu
    QTimer::singleShot(350, this, [this]()
                       {
                           // Ukrywamy bębny i napisy z poprzedniego losowania
                           messageLabel->hide();
                           reel1->hide();
                           reel2->hide();
                           reel3->hide();

                           // Przywracamy domyślny styl wiadomości błędu zakładu
                           messageLabel->setStyleSheet("color: #ff6666; font-size: 20px; font-weight: bold;");

                           // Bezpieczny reset logiki rdzenia gry
                           delete gameLogic;
                           gameLogic = new Game();

                           // Przywracamy startowe GUI pod 1000 kredytów
                           creditsLabel->setText("Credits: " + QString::number(gameLogic->getCredits()));
                           creditsLabel->adjustSize();
                           betInput->clear();

                           // Przywracamy widoczność komponentów interfejsu gry w tle (na 1.0 opacity)
                           auto *b1 = qobject_cast<QGraphicsOpacityEffect *>(spinButton->graphicsEffect());
                           auto *b2 = qobject_cast<QGraphicsOpacityEffect *>(betInput->graphicsEffect());
                           auto *b3 = qobject_cast<QGraphicsOpacityEffect *>(betLabel->graphicsEffect());
                           auto *b4 = qobject_cast<QGraphicsOpacityEffect *>(creditsLabel->graphicsEffect());
                           if (b1)
                               b1->setOpacity(1.0);
                           if (b2)
                               b2->setOpacity(1.0);
                           if (b3)
                               b3->setOpacity(1.0);
                           if (b4)
                               b4->setOpacity(1.0);

                           spinButton->setEnabled(true);
                           betInput->setEnabled(true);

                           // Zmieniamy okno na Menu Główne
                           stackedWidget->setCurrentIndex(MenuScene);

                           // fade in
                           fadeWidget(titleMenuLabel, 0, 100, 350);
                           fadeWidget(playButton, 0, 100, 350);
                           fadeWidget(infoButton, 0, 100, 350); });
}