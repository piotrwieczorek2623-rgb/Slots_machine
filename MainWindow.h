#pragma once

#include "src/game.h"
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QParallelAnimationGroup>
#include <QPushButton>
#include <QResizeEvent>
#include <QVideoWidget>
#include <QWidget>
#include <QSoundEffect>

class MainWindow : public QMainWindow
{
  Q_OBJECT

private:
  Game game;
  QMediaPlayer *player = nullptr;
  QVideoWidget *videoWidget = nullptr;

  QPushButton *spinButton = nullptr;
  QLineEdit *betInput = nullptr;

  QLabel *reel1 = nullptr;
  QLabel *reel2 = nullptr;
  QLabel *reel3 = nullptr;

  QGraphicsOpacityEffect *reel1Effect = nullptr;
  QGraphicsOpacityEffect *reel2Effect = nullptr;
  QGraphicsOpacityEffect *reel3Effect = nullptr;

  QLabel *creditsLabel = nullptr;
  QLabel *betLabel = nullptr;

  // dzwiek
  QMediaPlayer *winSound = nullptr;
  QAudioOutput *winSoundOutput = nullptr;

  // message
  QLabel *messageLabel = nullptr;
  QTimer *messageTimer = nullptr;

  // win
  QLabel *winLabel = nullptr;
  void showWinMessage(int prize);

  void hideReels();
  void animateReelIn(QLabel *reel, QPoint finalPos, int delay);

  void fadeWidget(QWidget *widget, int start, int end, int duration); // fade
  void startSpinAnimation();                                          // START ANIMACJI

public:
  MainWindow(QWidget *parent = nullptr);

protected:
  void resizeEvent(QResizeEvent *event) override; // resize
};
