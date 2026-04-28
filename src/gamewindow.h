#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include "gameboard.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void gameLoop();
    void onNewGameClicked();
    void onPauseClicked();

private:
    void updateStatusBar();
    void drawGame(QPainter &painter);

    GameBoard gameBoard;
    QTimer gameTimer;
    QLabel *statusLabel;
    QPushButton *pauseButton;
    QPushButton *newGameButton;

    static constexpr int CELL_SIZE = 25;
    static constexpr int BOARD_MARGIN = 10;
};

#endif // GAMEWINDOW_H
