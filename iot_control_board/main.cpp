#include <QApplication>
#include <QScrollArea>
#include <controlBoard.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ControlBoard *board = new ControlBoard;
    board->show();

    return app.exec();
}
