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
    titleInfoLabel = new QLabel("", infoWidget);
    titleInfoLabel->setAlignment(Qt::AlignCenter);
    titleInfoLabel->setStyleSheet("color: #e9e9e9;");

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

    //  =================================================================
    //                              TEKST
    //  =================================================================
    infoTextBrowser = new QTextBrowser(infoWidget);
    infoTextBrowser->setReadOnly(true);
    infoTextBrowser->setFrameShape(QFrame::NoFrame); // Ukrywa ramkę systemową

    // Stylizujemy tekst i tło (przezroczyste tło, żeby pasowało do okna)
    infoTextBrowser->setStyleSheet(R"(
        QTextBrowser {
            background-color: transparent;
            color: #e9e9e9;
            font-size: 20px;
        }
    )");

    // Tutaj możesz pisać swobodnie za pomocą kodu HTML!
    infoTextBrowser->setHtml(R"(
    <div style="text-align: center; font-family: sans-serif; line-height: 1.6; padding: 20px;">
        
        <h2 style="color: #da90ff; font-size: 28px; margin-bottom: 20px; font-weight: bold;">
            Symulator gry hazardowej slots machine!
        </h2>
        <p style="color: #e9e9e9; font-size: 20px; max-width: 600px; margin: 0 auto 30px auto;">
            Celem tej gry jest wylosowanie zwycięskiej kombinacji symboli i wypełnienie kieszeni pieniędzmi po brzegi!
        </p>

        <div style="margin: 30px 0;">
            <table align="center" style="border: 2px solid #da75ff; border-collapse: collapse; width: 80%; background-color: #222222; color: #e9e9e9; font-size: 18px;">
                <thead>
                    <tr style="background-color: #da75ff; color: black; font-weight: bold;">
                        <th style="padding: 12px; border: 1px solid #da75ff;">Symbol</th>
                        <th style="padding: 12px; border: 1px solid #da75ff;">Mnożnik</th>
                        <th style="padding: 12px; border: 1px solid #da75ff;">Waga</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td style="padding: 10px; border: 1px solid #333;">Cytryna</td>
                        <td style="padding: 10px; border: 1px solid #333; font-weight: bold; color: #da90ff;">X3</td>
                        <td style="padding: 10px; border: 1px solid #333;">50</td>
                    </tr>
                    <tr>
                        <td style="padding: 10px; border: 1px solid #333;">Wiśnia</td>
                        <td style="padding: 10px; border: 1px solid #333; font-weight: bold; color: #da90ff;">X4</td>
                        <td style="padding: 10px; border: 1px solid #333;">40</td>
                    </tr>
                    <tr>
                        <td style="padding: 10px; border: 1px solid #333;">Śliwka</td>
                        <td style="padding: 10px; border: 1px solid #333; font-weight: bold; color: #da90ff;">X5</td>
                        <td style="padding: 10px; border: 1px solid #333;">30</td>
                    </tr>
                    <tr>
                        <td style="padding: 10px; border: 1px solid #333;">Bomba</td>
                        <td style="padding: 10px; border: 1px solid #333; font-weight: bold; color: #da90ff;">*</td>
                        <td style="padding: 10px; border: 1px solid #333;">8</td>
                    </tr>
                    <tr>
                        <td style="padding: 10px; border: 1px solid #333;"> Siódemka</td>
                        <td style="padding: 10px; border: 1px solid #333; font-weight: bold; color: #da90ff;">X77</td>
                        <td style="padding: 10px; border: 1px solid #333;">7</td>
                    </tr>
                </tbody>
            </table>
        </div>
        
        <div style="margin-top: 40px; border-top: 1px solid #333; padding-top: 20px;">
            <span style="color: #888888; font-size: 16px; text-transform: uppercase; letter-spacing: 2px;">
                Autorzy
            </span>
            <p style="color: #da75ff; font-size: 22px; font-weight: bold; margin-top: 5px;">
                Piotr Wieczorek & Bartosz Pawłowski
            </p>
        </div>

    </div>
)");
    infoLayout->addWidget(infoTextBrowser); // Dodajemy do układu pionowego

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
                           fadeWidget(infoButton, 0, 100, 350); 
                           fadeWidget(exitGameButton, 0, 100, 350); });
}