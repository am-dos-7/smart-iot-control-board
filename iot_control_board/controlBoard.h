#ifndef CONTROLBOARD_H
#define CONTROLBOARD_H
#include <QObject>
#include "lampGui.h"
#include "tmpSocket.h"
#include "displayGui.h"
#include <QWidget>
#include <QGridLayout>
#include <QDebug>
#include <QTcpServer>
#include <QLabel>
#include <QPalette>
#include <cstring>
#include <QScrollArea>
#include <QHostAddress>
#include <QHostInfo>
#include <QSizePolicy>

#define DEFAULT_PORT 50885          /* Server default port number */

class ControlBoard: public QWidget{
    Q_OBJECT
public:
    ControlBoard(quint16 portNb = DEFAULT_PORT, QWidget *p = NULL);
    ~ControlBoard();

public slots:
    void onCloseRequested(QWidget *);            /* on close signal received*/
    void onNewConnection();                      /* when got a new client */
    void onConfigCompleted(TmpSocket *s);        // handles a tmpSocket config completeness

private:
    QGridLayout *layout;
    QTcpServer *server;
    QLabel *serverStatus;
    quint16 portNumber;
    QScrollArea *scrollBar;

    void addTmpSocket(QTcpSocket *);      // creates a TmpSocket from an incoming socket
};

#endif // CONTROLBOARD_H
