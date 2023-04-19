/*
 * 22/04/2022
 * Temporary class for handling the incoming socket and getting the
 * configuration messages before passing it to the control class
 *
*/

#ifndef TMPSOCKET_H
#define TMPSOCKET_H
#include <QTcpSocket>
#include <QObject>
#include <QDebug>

enum DEV_TYPE:uint8_t{
    DEFLT = 0,
    LAMP = 1,
    CAMERA = 2,
    DISPLAY = 3
};

class TmpSocket: public QObject{    // temporary socket
    Q_OBJECT
public:
    TmpSocket(QTcpSocket *s);    // takes the incoming socket as argument
    ~TmpSocket();
    QTcpSocket* getSocket();    // return the socket
    uint8_t getClientType();    // return the client type as a QString
    QString getClientTypeAsQString();  // return the client type as a QString
    QString getClientName();    // return the client name as a QString

public slots:
    void onReceived();

signals:
    void configCompleted(TmpSocket *);  // notifies the  parent that all the parameters have been received

private:
    QTcpSocket *sock;
    uint8_t clientType = 0;       //
    char *clientName;            //
    uint8_t msg_len = 0;    // length of the message
    bool got_len = false;       // true if the length is received
};



#endif // TMPSOCKET_H
