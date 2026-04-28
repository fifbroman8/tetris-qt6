#include "gamewindow.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QDebug>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Tetris - Qt6");
    setGeometry(100, 100, 900, 600);
    
    // Create main central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // LEFT SIDE: Game Board Area
    QWidget *gameAreaWidget = new QWidget();
    gameAreaWidget->setStyleSheet("background-color: #0a0a0a;");
    gameAreaWidget->setMinimumWidth(300);
    gameAreaWidget->setMinimumHeight(550);
    mainLayout->addWidget(gameAreaWidget, 3);
    
    // RIGHT SIDE: Info Panel
    QWidget *infoPanelWidget = new QWidget();
    infoPanelWidget->setStyleSheet("background-color: #252526; border-left: 1px solid #3e3e42;");
    infoPanelWidget->setMinimumWidth(250);
    QVBoxLayout *infoPanelLayout = new QVBoxLayout(infoPanelWidget);
    infoPanelLayout->setContentsMargins(15, 15, 15, 15);
    infoPanelLayout->setSpacing(8);
    
    // Title
    QLabel *titleLabel = new QLabel("TETRIS");
    QFont titleFont("Arial", 22, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #ffffff; padding: 10px 0px;");
    infoPanelLayout->addWidget(titleLabel);
    
    // Score Display
    statusLabel = new QLabel();
    statusLabel->setStyleSheet("color: #ffffff; font-size: 13px; line-height: 1.8; padding: 10px 0px;");
    statusLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    infoPanelLayout->addWidget(statusLabel);
    
    // Next Piece Label
    QLabel *nextLabel = new QLabel("Next Piece:");
    nextLabel->setStyleSheet("color: #ffffff; font-size: 12px; font-weight: bold; padding: 10px 0px;");
    infoPanelLayout->addWidget(nextLabel);
    
    // Controls Label
    QLabel *controlsLabel = new QLabel(
        "<b>Controls:</b><br>"
        "← → : Move<br>"
        "↑ : Rotate<br>"
        "↓ : Drop<br>"
        "Space: Fast Drop<br>"
        "P: Pause"
    );
    controlsLabel->setStyleSheet("color: #ffffff; font-size: 11px; line-height: 1.6; padding: 10px 0px;");
    infoPanelLayout->addWidget(controlsLabel);
    
    // Add stretch
    infoPanelLayout->addStretch(1);
    
    // NEW GAME BUTTON
    newGameButton = new QPushButton("New Game");
    newGameButton->setMinimumHeight(45);
    newGameButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #0078d4;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 4px;"
        "  font-weight: bold;"
        "  font-size: 14px;"
        "  padding: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #1084d7;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #005a9e;"
        "}"
    );
    infoPanelLayout->addWidget(newGameButton);
    
    // PAUSE BUTTON
    pauseButton = new QPushButton("Pause");
    pauseButton->setMinimumHeight(45);
    pauseButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #107c10;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 4px;"
        "  font-weight: bold;"
        "  font-size: 14px;"
        "  padding: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #117a11;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #0b5a0b;"
        "}"
    );
    infoPanelLayout->addWidget(pauseButton);
    
    mainLayout->addWidget(infoPanelWidget, 1);
    
    // Connect buttons
    connect(newGameButton, &QPushButton::clicked, this, &GameWindow::onNewGameClicked);
    connect(pauseButton, &QPushButton::clicked, this, &GameWindow::onPauseClicked);
    
    // Set focus
    setFocus();
    setFocusPolicy(Qt::StrongFocus);
    
    // Start game loop
    connect(&gameTimer, &QTimer::timeout, this, &GameWindow::gameLoop);
    gameTimer.start(50); // 20 FPS
    
    updateStatusBar();
}

GameWindow::~GameWindow()
{
    gameTimer.stop();
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }

    switch (event->key()) {
    case Qt::Key_Left:
        gameBoard.moveLeft();
        break;
    case Qt::Key_Right:
        gameBoard.moveRight();
        break;
    case Qt::Key_Down:
        gameBoard.moveDown();
        break;
    case Qt::Key_Up:
        gameBoard.rotatePiece();
        break;
    case Qt::Key_Space:
        gameBoard.dropDown();
        break;
    case Qt::Key_P:
        gameBoard.togglePause();
        pauseButton->setText(gameBoard.isPaused() ? "Resume" : "Pause");
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void GameWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    drawGame(painter);
}

void GameWindow::drawGame(QPainter &painter)
{
    int startX = 15;
    int startY = 15;

    // Draw border
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRect(
        startX - 1,
        startY - 1,
        GameBoard::BOARD_WIDTH * CELL_SIZE + 2,
        GameBoard::BOARD_HEIGHT * CELL_SIZE + 2
    );

    // Draw cells
    for (int row = 0; row < GameBoard::BOARD_HEIGHT; ++row) {
        for (int col = 0; col < GameBoard::BOARD_WIDTH; ++col) {
            int x = startX + col * CELL_SIZE;
            int y = startY + row * CELL_SIZE;

            int color = gameBoard.getCellColor(row, col);

            if (color != 0) {
                painter.fillRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, QColor(color));
                painter.setPen(QPen(Qt::darkGray, 1));
                painter.drawRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2);
            } else {
                painter.setPen(QPen(Qt::darkGray, 1));
                painter.drawRect(x, y, CELL_SIZE, CELL_SIZE);
            }
        }
    }

    // Draw "Game Over" if needed
    if (gameBoard.isGameOver()) {
        painter.setPen(QPen(Qt::red, 3));
        painter.setFont(QFont("Arial", 28, QFont::Bold));
        painter.drawText(
            startX,
            startY + GameBoard::BOARD_HEIGHT * CELL_SIZE / 2 - 30,
            GameBoard::BOARD_WIDTH * CELL_SIZE,
            100,
            Qt::AlignCenter,
            "GAME OVER"
        );
    }

    // Draw "Paused" if paused
    if (gameBoard.isPaused()) {
        painter.setPen(QPen(Qt::yellow, 3));
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.drawText(
            startX,
            startY + GameBoard::BOARD_HEIGHT * CELL_SIZE / 2 - 25,
            GameBoard::BOARD_WIDTH * CELL_SIZE,
            100,
            Qt::AlignCenter,
            "PAUSED"
        );
    }
}

void GameWindow::gameLoop()
{
    gameBoard.update();
    updateStatusBar();
    update();
}

void GameWindow::updateStatusBar()
{
    QString status = QString(
        "<span style='color: #ffffff;'><b>Score:</b> %1<br>"
        "<b>Level:</b> %2<br>"
        "<b>Lines:</b> %3</span>"
    ).arg(gameBoard.getScore())
     .arg(gameBoard.getLevel())
     .arg(gameBoard.getLines());

    statusLabel->setText(status);
}

void GameWindow::onNewGameClicked()
{
    qDebug() << "New Game Button Clicked!";
    gameBoard.newGame();
    pauseButton->setText("Pause");
    updateStatusBar();
    update();
}

void GameWindow::onPauseClicked()
{
    qDebug() << "Pause Button Clicked!";
    gameBoard.togglePause();
    pauseButton->setText(gameBoard.isPaused() ? "Resume" : "Pause");
    update();
}
