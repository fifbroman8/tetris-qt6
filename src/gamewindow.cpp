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
    setStyleSheet("background-color: #1e1e1e;");
    
    // Create main central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // LEFT SIDE: Game Board Drawing Area
    QWidget *gameAreaWidget = new QWidget();
    gameAreaWidget->setStyleSheet("background-color: #0a0a0a;");
    gameAreaWidget->setMinimumWidth(350);
    gameAreaWidget->setMinimumHeight(550);
    mainLayout->addWidget(gameAreaWidget, 3);
    
    // RIGHT SIDE: Info Panel
    QWidget *infoPanelWidget = new QWidget();
    infoPanelWidget->setStyleSheet("background-color: #252526;");
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
    statusLabel->setStyleSheet("color: #00ff00; font-size: 13px; font-family: monospace; padding: 10px 0px;");
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
    controlsLabel->setStyleSheet("color: #cccccc; font-size: 11px; line-height: 1.6; padding: 10px 0px;");
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
    
    // Connect buttons with debug output
    connect(newGameButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "New Game Button Clicked!";
        onNewGameClicked();
    });
    
    connect(pauseButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "Pause Button Clicked!";
        onPauseClicked();
    });
    
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
    
    // Draw only on the left side (game area)
    drawGame(painter);
}

void GameWindow::drawGame(QPainter &painter)
{
    // Calculate drawing position (left side of window)
    int startX = 15;
    int startY = 15;

    // Draw game board border
    painter.setPen(QPen(Qt::white, 2));
    painter.setBrush(Qt::black);
    painter.drawRect(
        startX - 1,
        startY - 1,
        GameBoard::BOARD_WIDTH * CELL_SIZE + 2,
        GameBoard::BOARD_HEIGHT * CELL_SIZE + 2
    );

    // Draw all cells
    for (int row = 0; row < GameBoard::BOARD_HEIGHT; ++row) {
        for (int col = 0; col < GameBoard::BOARD_WIDTH; ++col) {
            int x = startX + col * CELL_SIZE;
            int y = startY + row * CELL_SIZE;

            int color = gameBoard.getCellColor(row, col);

            if (color != 0) {
                // Filled cell
                painter.fillRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, QColor(color));
                painter.setPen(QPen(Qt::darkGray, 1));
                painter.drawRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2);
            } else {
                // Empty cell
                painter.setPen(QPen(Qt::darkGray, 1));
                painter.drawRect(x, y, CELL_SIZE, CELL_SIZE);
            }
        }
    }

    // Draw "Game Over" overlay
    if (gameBoard.isGameOver()) {
        painter.setBrush(QColor(0, 0, 0, 150));
        painter.setPen(Qt::NoPen);
        painter.drawRect(
            startX,
            startY,
            GameBoard::BOARD_WIDTH * CELL_SIZE,
            GameBoard::BOARD_HEIGHT * CELL_SIZE
        );
        
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

    // Draw "Paused" overlay
    if (gameBoard.isPaused() && !gameBoard.isGameOver()) {
        painter.setBrush(QColor(0, 0, 0, 150));
        painter.setPen(Qt::NoPen);
        painter.drawRect(
            startX,
            startY,
            GameBoard::BOARD_WIDTH * CELL_SIZE,
            GameBoard::BOARD_HEIGHT * CELL_SIZE
        );
        
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
    update(); // Trigger repaint
}

void GameWindow::updateStatusBar()
{
    QString status = QString(
        "Score: %1<br>"
        "Level: %2<br>"
        "Lines: %3"
    ).arg(gameBoard.getScore())
     .arg(gameBoard.getLevel())
     .arg(gameBoard.getLines());

    statusLabel->setText(status);
}

void GameWindow::onNewGameClicked()
{
    gameBoard.newGame();
    pauseButton->setText("Pause");
    updateStatusBar();
    update();
}

void GameWindow::onPauseClicked()
{
    gameBoard.togglePause();
    pauseButton->setText(gameBoard.isPaused() ? "Resume" : "Pause");
    update();
}
