#include "MainWindow.h"

#include <QAudioOutput>
#include <QCoreApplication>
#include <QDir>
#include <QEasingCurve>
#include <QFont>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QShortcut>
#include <QTimer>
#include <QVBoxLayout>
#include <QFile>
#include <QSoundEffect>
#include <QStackedWidget>

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

  // Dodajemy ekrany do managera (Menu = 0, Gra = 1)
  stackedWidget->addWidget(menuWidget);
  stackedWidget->addWidget(gameWidget);

  // Startujemy od Menu Głównego
  stackedWidget->setCurrentIndex(0);

  // Wyjście z aplikacji za pomocą klawisza ESCAPE
  new QShortcut(QKeySequence(Qt::Key_Escape), this, [this]()
                { close(); });

  // WYMUSZENIE TRYBU PEŁNOEKRANOWEGO DLA CAŁEGO OKNA
  showFullScreen();
}

// ==========================================
//                MENU GŁÓWNE
// ==========================================
void MainWindow::setupMenuUi()
{
  menuWidget = new QWidget(this);
  menuWidget->setStyleSheet("background-color: #1a1a1a;"); // Ciemne tło menu

  QVBoxLayout *menuLayout = new QVBoxLayout(menuWidget);

  // 1. Duży napis SLOT MACHINE
  titleLabel = new QLabel("SLOT MACHINE", menuWidget);
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setStyleSheet("color: #da90ff;"); // Fioletowy, neonowy kolor

  QFont titleFont = titleLabel->font();
  titleFont.setPointSize(64); // Powiększony pod pełny ekran
  titleFont.setBold(true);
  titleLabel->setFont(titleFont);

  // 2. Przycisk play pod napisem
  playButton = new QPushButton("PLAY", menuWidget);
  playButton->setFixedWidth(300);
  playButton->setFixedHeight(80);
  playButton->setStyleSheet(R"(
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

  // Pozycjonowanie na środku ekranu
  menuLayout->addStretch();
  menuLayout->addWidget(titleLabel);
  menuLayout->addSpacing(40);
  menuLayout->addWidget(playButton, 0, Qt::AlignCenter);
  menuLayout->addWidget(infoButton, 0, Qt::AlignCenter);
  menuLayout->addStretch();

  // Podłączenie kliknięcia play -> przejście do gry
  connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
}

// ==========================================
//                   INFO
// ==========================================
void MainWindow::setupInfoUi()
{
  infoWidget = new QWidget(this);
  infoWidget->setStyleSheet("background-color: #1a1a1a;"); // Ciemne tło info

  QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);

  // 1. Duży napis SLOT MACHINE
  titleLabel = new QLabel("MULTIPLIERS", infoWidget);
  titleLabel->setAlignment(Qt::AlignLeft);
  titleLabel->setStyleSheet("color: #d5d5d5;");

  QFont titleFont = titleLabel->font();
  titleFont.setPointSize(64); // Powiększony pod pełny ekran
  titleFont.setBold(true);
  titleLabel->setFont(titleFont);
}

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

void MainWindow::onPlayClicked()
{
  transitionFromMenuToGame();
}

void MainWindow::startSpinAnimation()
{
  int bet = betInput->text().toInt();
  if (bet > gameLogic->getCredits() || bet <= 0)
  {
    messageTimer->stop();
    messageLabel->setGraphicsEffect(nullptr);
    messageLabel->setText("INVALID BET");
    messageLabel->show();
    messageLabel->raise();
    messageTimer->start(3000);
    return;
  }

  if (messageTimer)
    messageTimer->stop();
  messageLabel->hide();

  gameLogic->setBet(bet);
  int prize = gameLogic->playRound();

  hideReels();
  spinButton->setEnabled(false);
  betInput->setEnabled(false);

  fadeWidget(spinButton, 100, 0, 300);
  fadeWidget(betInput, 100, 0, 300);
  fadeWidget(betLabel, 100, 0, 300);
  fadeWidget(creditsLabel, 100, 0, 300);

  QTimer::singleShot(300, this, [this]()
                     {
    player->stop();
    player->setPosition(0);
    player->play();

    QTimer::singleShot(80, this, [this]() {
      videoWidget->show();
      videoWidget->raise();
    }); });

  QTimer::singleShot(4000, this, [this, prize]()
                     {
    videoWidget->hide();

    QPoint reelPositions[3] = {QPoint(210, 265), QPoint(485, 265), QPoint(760, 265)};
    animateReelIn(reel1, reelPositions[0], 0);
    animateReelIn(reel2, reelPositions[1], 150);
    animateReelIn(reel3, reelPositions[2], 300);

    if (prize > 0)
    {
      showWinMessage(prize);
      winSound->stop();
      winSound->setPosition(0);
      winSound->play();
    }

    QTimer::singleShot(300, this, [this]()
                       {
                         // =====SPRAWDZANIE BANKRUCTWA=====
                         if (gameLogic->getCredits() <= 0)
                         {
                           // Wyświetlamy wielki czerwony komunikat w miejscu wiadomości
                           messageTimer->stop();
                           messageLabel->setGraphicsEffect(nullptr); // Reset fade, jeśli był
                           messageLabel->setStyleSheet("color: #ff3333; font-size: 50px; font-weight: bold;");
                           messageLabel->setText("KONIEC GRY");
                           messageLabel->show();
                           messageLabel->raise();

                           // DYNAMICZNE TWORZENIE ŻÓŁTEGO PRZYCISKU "POWRÓT DO MENU"
                           // Tworzymy go wewnątrz panelu bocznego (rightPanel) lub bezpośrednio na gameWidget
                           QPushButton *returnButton = new QPushButton("POWRÓT DO MENU", gameWidget);
                           returnButton->setStyleSheet(R"(
      QPushButton {
          background-color: #ffd700; /* Złoty / Żółty kolor */
          color: #896302;
          border-radius: 15px;
          font-size: 24px;
          font-weight: bold;
          border: 3px solid #b8860b;
          padding: 10px;
      }
      QPushButton:hover {
          background-color: #ffea70;
      }
      QPushButton:pressed {
          background-color: #cca300;
      }
  )");

                           // Pozycjonujemy go na środku prawego panelu, dokładnie pod napisem BANKRUT
                           returnButton->setFixedHeight(130);

                           panelLayout->addWidget(returnButton);
  returnButton->show();

  // Łączymy kliknięcie żółtego przycisku z akcją powrotu
  connect(returnButton, &QPushButton::clicked, this, [this, returnButton]()
          {
    

    //fadeout przycisku i napisu
    fadeWidget(returnButton, 100, 0, 350);
    // 1. Usuwamy przycisk z układu panelu, aby nie blokował miejsca przy kolejnej grze
    panelLayout->removeWidget(returnButton);
    

    // 2. Wywołujemy dedykowaną funkcję przejścia do menu
    transitionFromGameToMenu();

    // 3. Bezpiecznie kasujemy obiekt przycisku z pamięci
    returnButton->deleteLater(); });

  return; // Przerywamy funkcję, NIE włączamy standardowego GUI poniżej!
}





      spinButton->setEnabled(true);
      betInput->setEnabled(true);

      fadeWidget(spinButton, 0, 100, 300);
      fadeWidget(betInput, 0, 100, 300);
      fadeWidget(betLabel, 0, 100, 300);
      fadeWidget(creditsLabel, 0, 100, 300);
});
});
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
  QMainWindow::resizeEvent(event);

  if (videoWidget && stackedWidget->currentWidget())
    videoWidget->setGeometry(stackedWidget->currentWidget()->rect());

  if (creditsLabel)
  {
    creditsLabel->move(width() - creditsLabel->width() - 20, 20);
  }
}

void MainWindow::fadeWidget(QWidget *widget, int start, int end, int duration)
{
  if (!widget)
    return;

  auto *effect = qobject_cast<QGraphicsOpacityEffect *>(widget->graphicsEffect());
  if (!effect)
  {
    effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);
  }

  effect->setOpacity(start / 100.0);

  auto *animation = new QPropertyAnimation(effect, "opacity", widget);
  animation->setDuration(duration);
  animation->setStartValue(start / 100.0);
  animation->setEndValue(end / 100.0);
  animation->setEasingCurve(QEasingCurve::InOutQuad);
  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::hideReels()
{
  QLabel *reels[3] = {reel1, reel2, reel3};
  QGraphicsOpacityEffect *effects[3] = {reel1Effect, reel2Effect, reel3Effect};

  for (int i = 0; i < 3; i++)
  {
    if (!reels[i] || !reels[i]->isVisible())
      continue;

    auto *animation = new QPropertyAnimation(effects[i], "opacity", this);
    animation->setDuration(250);
    animation->setStartValue(effects[i]->opacity());
    animation->setEndValue(0.0);

    connect(animation, &QPropertyAnimation::finished, reels[i], [reel = reels[i]]()
            { reel->hide(); });
    animation->start(QAbstractAnimation::DeleteWhenStopped);
  }
}

void MainWindow::animateReelIn(QLabel *reel, QPoint finalPos, int delay)
{
  if (!reel)
    return;

  reel->move(finalPos.x(), finalPos.y() - 120);
  reel->show();
  reel->raise();

  QGraphicsOpacityEffect *effect = nullptr;
  if (reel == reel1)
    effect = reel1Effect;
  else if (reel == reel2)
    effect = reel2Effect;
  else if (reel == reel3)
    effect = reel3Effect;

  if (!effect)
    return;
  effect->setOpacity(0.0);

  auto *group = new QParallelAnimationGroup(reel);

  auto *moveAnim = new QPropertyAnimation(reel, "pos");
  moveAnim->setDuration(450);
  moveAnim->setStartValue(QPoint(finalPos.x(), finalPos.y() - 120));
  moveAnim->setEndValue(finalPos);
  moveAnim->setEasingCurve(QEasingCurve::OutBack);

  auto *fadeAnim = new QPropertyAnimation(effect, "opacity");
  fadeAnim->setDuration(450);
  fadeAnim->setStartValue(0.0);
  fadeAnim->setEndValue(1.0);

  group->addAnimation(moveAnim);
  group->addAnimation(fadeAnim);

  QTimer::singleShot(delay, this, [group]()
                     { group->start(QAbstractAnimation::DeleteWhenStopped); });
}

void MainWindow::showWinMessage(int prize)
{
  winLabel->setText("WIN! " + QString::number(prize) + "zl");
  winLabel->adjustSize();

  int x = (stackedWidget->currentWidget()->width() - winLabel->width()) / 2;
  int y = (stackedWidget->currentWidget()->height() - winLabel->height()) / 2;

  winLabel->move(x, y - 80);
  winLabel->show();
  winLabel->raise();

  fadeWidget(winLabel, 0, 100, 300);

  auto *moveAnim = new QPropertyAnimation(winLabel, "pos", this);
  moveAnim->setDuration(450);
  moveAnim->setStartValue(QPoint(x, y - 80));
  moveAnim->setEndValue(QPoint(x, y));
  moveAnim->setEasingCurve(QEasingCurve::OutBack);
  moveAnim->start(QAbstractAnimation::DeleteWhenStopped);

  QTimer::singleShot(2500, this, [this]()
                     {
    fadeWidget(winLabel, 100, 0, 500);
    QTimer::singleShot(500, this, [this]() { winLabel->hide(); }); });
}

// 1. DEDYKOWANE PRZEJŚCIE: MENU -> GRA
void MainWindow::transitionFromMenuToGame()
{
  // Blokujemy przycisk na czas animacji
  playButton->setEnabled(false);

  // Wygaszamy napisy i przyciski w menu
  fadeWidget(titleLabel, 100, 0, 350);
  fadeWidget(playButton, 100, 0, 350);
  fadeWidget(infoButton, 100, 0, 350);

  // Po zakończeniu animacji (350ms) przełączamy okno i dbamy o czystość stanów
  QTimer::singleShot(350, this, [this]()
                     {
        stackedWidget->setCurrentIndex(1); // Zmiana na ekran gry

        if (creditsLabel) {
            creditsLabel->move(width() - creditsLabel->width() - 20, 20);
        }

        // Przywracamy bazową przezroczystość komponentom menu, 
        // aby były gotowe na kolejny powrót do menu w przyszłości
        auto *titleEffect = qobject_cast<QGraphicsOpacityEffect *>(titleLabel->graphicsEffect());
        auto *playEffect = qobject_cast<QGraphicsOpacityEffect *>(playButton->graphicsEffect());
        if (titleEffect) titleEffect->setOpacity(1.0);
        if (playEffect) playEffect->setOpacity(1.0);
        
        playButton->setEnabled(true); 
        //fade przyciskow
        fadeWidget(spinButton, 0, 100, 350);
        fadeWidget(betInput, 0, 100, 350);
        fadeWidget(betLabel, 0, 100, 350);
        fadeWidget(creditsLabel, 0, 100, 350);
      });
}

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
        if (b1) b1->setOpacity(1.0);
        if (b2) b2->setOpacity(1.0);
        if (b3) b3->setOpacity(1.0);
        if (b4) b4->setOpacity(1.0);

        spinButton->setEnabled(true);
        betInput->setEnabled(true);

        // Zmieniamy okno na Menu Główne
        stackedWidget->setCurrentIndex(0); 
                    
        //fade in
        fadeWidget(titleLabel, 0, 100, 350);
        fadeWidget(playButton, 0, 100, 350);
        fadeWidget(infoButton, 0, 100, 350);
      
      });
}