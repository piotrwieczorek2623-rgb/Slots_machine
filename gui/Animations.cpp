#include "MainWindow.h"

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
}); });
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
