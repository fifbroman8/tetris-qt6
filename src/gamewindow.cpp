#include "gamewindow.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QFont>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Tetris - Qt6");
    setStyleSheet("background-color: #1e1e1e; color: white;");

    // Central widget
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Game area - create with proper size
    QWidget *gameAreaContainer = new QWidget(this);
    gameAreaContainer->setMinimumSize(
        GameBoard::BOARD_WIDTH * CELL_SIZE + 2 * BOARD_MARGIN + 2,
        GameBoard::BOARD_HEIGHT * CELL_SIZE + 2 * BOARD_MARGIN + 2
    );
    gameAreaContainer->setStyleSheet("background-color: #0a0a0a;");
    mainLayout->addWidget(gameAreaContainer);

    // Info panel
    QWidget *infoPanel = new QWidget(this);
    QVBoxLayout *infoPanelLayout = new QVBoxLayout(infoPanel);
    infoPanelLayout->setAlignment(Qt::AlignTop);
    infoPanelLayout->setContentsMargins(10, 10, 10, 10);
    infoPanelLayout->setSpacing(10);

    // Title
    QLabel *titleLabel = new QLabel("TETRIS", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    infoPanelLayout->addWidget(titleLabel);

    // Score
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("font-size: 13px; margin-top: 10px; line-height: 1.8;");
    statusLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    infoPanelLayout->addWidget(statusLabel);

    // Next piece preview
    QLabel *nextLabel = new QLabel("Next Piece:", this);
    nextLabel->setStyleSheet("font-size: 12px; margin-top: 10px; font-weight: bold;");
    infoPanelLayout->addWidget(nextLabel);

    // Controls info
    QLabel *controlsLabel = new QLabel(
        "<b>Controls:</b><br>"
        "← → : Move<br>"
        "↑ : Rotate<br>"
        "↓ : Drop<br>"
        "Space: Fast Drop<br>"
        "P: Pause",
        this
    );
    controlsLabel->setStyleSheet("font-size: 11px; margin-top: 10px; line-height: 1.6;");
    infoPanelLayout->addWidget(controlsLabel);

    // Buttons
    newGameButton = new QPushButton("New Game", this);
    newGameButton->setMinimumHeight(40);
    newGameButton->setStyleSheet(
        "QPushButton { background-color: #0078d4; color: white; padding: 10px; border-radius: 4px; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { background-color: #1084d7; }"
        "QPushButton:pressed { background-color: #005a9e; }"
    );
    connect(newGameButton, &QPushButton::clicked, this, &GameWindow::onNewGameClicked);
    infoPanelLayout->addWidget(newGameButton);

    pauseButton = new QPushButton("Pause", this);
    pauseButton->setMinimumHeight(40);
    pauseButton->setStyleSheet(
        "QPushButton { background-color: #107c10; color: white; padding: 10px; border-radius: 4px; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { background-color: #117a11; }"
        "QPushButton:pressed { background-color: #0b5a0b; }"
    );
    connect(pauseButton, &QPushButton::clicked, this, &GameWindow::onPauseClicked);
    infoPanelLayout->addWidget(pauseButton);

    infoPanelLayout->addStretch();
    infoPanel->setMaximumWidth(210);
    infoPanel->setStyleSheet("background-color: #252526; border-left: 1px solid #3e3e42; padding: 0px;");
    mainLayout->addWidget(infoPanel);

    setCentralWidget(centralWidget);

    // Set focus for keyboard input
    setFocus();
    setFocusPolicy(Qt::StrongFocus);

    // Game loop timer
    connect(&gameTimer, &QTimer::timeout, this, &GameWindow::gameLoop);
    gameTimer.start(50); // 20 FPS

    updateStatusBar();
    resize(850, 550);
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

    // Draw game board
    drawGame(painter);
}

void GameWindow::drawGame(QPainter &painter)
{
    int startX = BOARD_MARGIN + 10;
    int startY = BOARD_MARGIN + 10;

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
        painter.setPen(QPen(Qt::red, 2));
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.drawText(
            startX,
            startY + GameBoard::BOARD_HEIGHT * CELL_SIZE / 2 - 20,
            GameBoard::BOARD_WIDTH * CELL_SIZE,
            100,
            Qt::AlignCenter,
            "GAME OVER"
        );
    }

    // Draw "Paused" if paused
    if (gameBoard.isPaused()) {
        painter.setPen(QPen(Qt::yellow, 2));
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(
            startX,
            startY + GameBoard::BOARD_HEIGHT * CELL_SIZE / 2 - 15,
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
        "<b>Score:</b> %1<br>"
        "<b>Level:</b> %2<br>"
        "<b>Lines:</b> %3"
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
