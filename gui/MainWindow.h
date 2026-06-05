#pragma once

#include "../src/game.h"
#include <QMainWindow>
#include <QMediaPlayer>
#include <QParallelAnimationGroup>
#include <QResizeEvent>
#include <QVideoWidget>
#include <QWidget>
#include <QSoundEffect>
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
#include <QStackedWidget>

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

  QVBoxLayout *panelLayout;

  // menu
  Game *gameLogic;
  QStackedWidget *stackedWidget;

  QLabel *titleMenuLabel;
  QLabel *titleInfoLabel;
  QPushButton *playButton;
  QPushButton *infoButton;
  QPushButton *returnToMenuButton;
  QWidget *gameWidget;
  QWidget *menuWidget;
  QWidget *infoWidget;
  void setupMenuUi();
  void setupGameUi();
  void setupInfoUi();
  void transitionFromMenuToGame();
  void transitionFromGameToMenu();
  void transitionFromMenuToInfo();

public:
  MainWindow(QWidget *parent = nullptr);

protected:
  void resizeEvent(QResizeEvent *event) override; // resize
private slots:
  void onPlayClicked();
  void onInfoClicked();
};

enum ScreenIndex
{
  MenuScene = 0,
  GameScene = 1,
  InfoScene = 2
};