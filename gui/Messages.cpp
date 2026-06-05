#include "MainWindow.h"

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
