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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

  // CENTRAL
  new QShortcut(QKeySequence(Qt::Key_Escape), this, [this]()
                { close(); });

  QWidget *central = new QWidget(this);
  central->setStyleSheet("background-color: #232323;");
  setCentralWidget(central);

  // VIDEO MP4
  videoWidget = new QVideoWidget(central);
  videoWidget->setGeometry(central->rect());
  videoWidget->hide();

  player = new QMediaPlayer(this);
  player->setVideoOutput(videoWidget);

  // dzwiek animacji

  auto *audioOutput = new QAudioOutput(this);
  audioOutput->setVolume(1.0); // 0.0 – 1.0 GŁOŚCNOŚĆ

  player->setAudioOutput(audioOutput);
  player->setVideoOutput(videoWidget);

  // MP4 z resources NIE działa dobrze → użyj pliku z dysku
  // player->setSource(QUrl::fromLocalFile(":/videos/animation_spin_machine.mp4"));
  QString videoPath = QDir(QCoreApplication::applicationDirPath())
                          .filePath("../resources/videos/spin.mp4");

  player->setSource(QUrl::fromLocalFile(videoPath));

  videoWidget->raise();

  // dzwiek wygranej (ZMIENIONY NA QMediaPlayer)
  QString soundPath =
      QDir(QCoreApplication::applicationDirPath())
          .filePath("../resources/sounds/coin_win.wav");

  winSound = new QMediaPlayer(this);
  winSoundOutput = new QAudioOutput(this);

  winSoundOutput->setVolume(1.0); // głośność 0.0 - 1.0
  winSound->setAudioOutput(winSoundOutput);
  winSound->setSource(QUrl::fromLocalFile(soundPath));

  // FULLSCREEN
  showFullScreen();

  // Główny layout: lewa strona automat, prawa panel
  QHBoxLayout *mainLayout = new QHBoxLayout(central);
  mainLayout->setContentsMargins(0, 0, 5, 115);
  mainLayout->setSpacing(40);

  // =========================
  // LEWA STRONA — AUTOMAT
  // =========================

  QLabel *machineLabel = new QLabel(this);
  QPixmap machinePixmap(":/images/machine_hq_yel.png");

  machineLabel->setPixmap(machinePixmap);
  machineLabel->setScaledContents(true);
  machineLabel->setMinimumSize(900, 700);

  mainLayout->addWidget(machineLabel, 4);

  //===============================
  //========OWOCE NA BEBNACH=======
  //===============================

  reel1 = new QLabel(machineLabel);
  reel2 = new QLabel(machineLabel);
  reel3 = new QLabel(machineLabel);

  reel1->setFixedSize(300, 300);
  reel2->setFixedSize(300, 300);
  reel3->setFixedSize(300, 300);

  // windows symbol layout
  //  reel1->move(165, 225);
  //  reel2->move(405, 225);
  //  reel3->move(650, 225);

  // wyglada jakby ten układ wogole nie mial znaczenia xd

  // linux symbol layout
  reel1->move(265, 260);
  reel2->move(505, 260);
  reel3->move(750, 260);

  reel1->setAlignment(Qt::AlignCenter);
  reel2->setAlignment(Qt::AlignCenter);
  reel3->setAlignment(Qt::AlignCenter);

  reel1->setStyleSheet("background: transparent;");
  reel2->setStyleSheet("background: transparent;");
  reel3->setStyleSheet("background: transparent;");

  reel1->setAttribute(Qt::WA_TranslucentBackground);
  reel2->setAttribute(Qt::WA_TranslucentBackground);
  reel3->setAttribute(Qt::WA_TranslucentBackground);

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

  // =========================
  // PRAWA STRONA — PANEL
  // =========================

  QWidget *rightPanel = new QWidget(this);
  QVBoxLayout *panelLayout = new QVBoxLayout(rightPanel);

  panelLayout->setContentsMargins(20, 80, 20, 80);
  panelLayout->setSpacing(40);
  // Credit label
  creditsLabel = new QLabel(central);
  creditsLabel->setText("Credits: 1000"); // startowa wartość
  creditsLabel->setStyleSheet(R"(
    color: white;
    background-color: rgba(0, 0, 0, 150);
    border-radius: 10px;
    padding: 8px 15px;
)");
  creditsLabel->setFont(QFont("Arial", 18, QFont::Bold));
  creditsLabel->adjustSize();
  // creditsLabel->move(width() - creditsLabel->width() - 20, 20);
  creditsLabel->move(width() - creditsLabel->width() - 20, 20);
  creditsLabel->raise();

  // BET LABEL
  betLabel = new QLabel("BET", this);
  betLabel->setAlignment(Qt::AlignCenter);

  QFont betFont;
  betFont.setPointSize(38);
  betFont.setBold(true);
  betLabel->setFont(betFont);

  // MESSAGE LABEL
  messageLabel = new QLabel(this);

  messageLabel->setStyleSheet(R"(
    color: #ff6666;
    font-size: 20px;
    font-weight: bold;
)");

  messageLabel->setAlignment(Qt::AlignCenter);
  messageLabel->hide();

  // win label
  winLabel = new QLabel(central);
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

  // timer do message
  messageTimer = new QTimer(this);
  messageTimer->setSingleShot(true);

  connect(messageTimer, &QTimer::timeout, this, [this]()
          {

    fadeWidget(messageLabel, 100, 0, 500);

    QTimer::singleShot(500, this, [this]() {
        messageLabel->hide();
    }); });

  // BET INPUT
  betInput = new QLineEdit(this);
  betInput->setAlignment(Qt::AlignCenter);
  betInput->setPlaceholderText("SET BET");

  QFont inputFont;
  inputFont.setPointSize(28);
  betInput->setFont(inputFont);
  betInput->setFixedHeight(80);

  // PushButton
  spinButton = new QPushButton("SPIN", this);
  spinButton->setStyleSheet(R"(
      QPushButton {
          background-color: #da90ff;
          color: black;
          border-radius: 15px;
          padding: 10px;
            border: 2px solid #da75ff;
      }

      QPushButton:hover {
          background-color: #daacff;
      }

      QPushButton:pressed {
          background-color: #da57ff;
      }
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

  // DZIAŁANIE
  // Kliknięcie SPIN odpala animację
  connect(spinButton, &QPushButton::clicked, this,
          &MainWindow::startSpinAnimation);

  // Koniec filmu przywraca GUI
  connect(player, &QMediaPlayer::mediaStatusChanged, this,
          [this](QMediaPlayer::MediaStatus status)
          {
            if (status == QMediaPlayer::EndOfMedia)
            {
              videoWidget->hide();
            }
          });

  connect(spinButton, &QPushButton::clicked, this, [=]()
          { QTimer::singleShot(300, this, [this]()
                               {
                                 creditsLabel->setText("Credits: " + QString::number(game.getCredits()));
                                 creditsLabel->adjustSize();
                                 const auto &spin = game.getCurrentSpin();
                                 QLabel *reels[3] = {reel1, reel2, reel3};
                                 for (int i = 0; i < spin.size() && i < 3; i++)
                                 {
                                   QString path = QString::fromStdString(spin[i]->getPath());
                                       //":/images/" + QString::fromStdString(spin[i]->getName()) + ".png";

                                   QPixmap symbolPixmap(path);
                                   reels[i]->setPixmap(symbolPixmap.scaled(
                                       reels[i]->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
                                 } }); });
}

// START ANIMACJI
void MainWindow::startSpinAnimation()
{

  // Ma sie wyjebac gdy zly bet
  int bet = betInput->text().toInt();
  if (bet > game.getCredits() || bet <= 0)
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

  game.setBet(bet);
  int prize = game.playRound();

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

                       QPoint reelPositions[3] = {QPoint(210, 265), QPoint(485, 265),
                                                  QPoint(760, 265)};

                       animateReelIn(reel1, reelPositions[0], 0);
                       animateReelIn(reel2, reelPositions[1], 150);
                       animateReelIn(reel3, reelPositions[2], 300);

                       if (prize > 0)
                       {
                         showWinMessage(prize);

                         // Zatrzymujemy, przewijamy do 0 milisekund i odpalamy
                         winSound->stop();
                         winSound->setPosition(0);
                         winSound->play();
                       }

                       QTimer::singleShot(300, this, [this]()
                                          {
        spinButton->setEnabled(true);
        betInput->setEnabled(true);

        fadeWidget(spinButton, 0, 100, 300);
        fadeWidget(betInput, 0, 100, 300);
        fadeWidget(betLabel, 0, 100, 300);
        fadeWidget(creditsLabel, 0, 100, 300); }); });
}

// RESIZE
void MainWindow::resizeEvent(QResizeEvent *event)
{
  QMainWindow::resizeEvent(event);

  if (videoWidget && centralWidget())
    videoWidget->setGeometry(centralWidget()->rect());
  if (creditsLabel)
  {
    creditsLabel->move(width() - creditsLabel->width() - 20, 20);
  }
}

// FADE (linuxowy)
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

// show win
void MainWindow::showWinMessage(int prize)
{
  winLabel->setText("WIN! " + QString::number(prize) + "zl");
  winLabel->adjustSize();

  int x = (centralWidget()->width() - winLabel->width()) / 2;
  int y = (centralWidget()->height() - winLabel->height()) / 2;

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

    QTimer::singleShot(500, this, [this]() {
      winLabel->hide();
    }); });
}