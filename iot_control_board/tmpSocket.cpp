#include "tmpSocket.h"

TmpSocket::TmpSocket(QTcpSocket *s): sock(s)
{
    connect(sock, SIGNAL(readyRead()), this, SLOT(onReceived()));
//    messageNb = 0;
}

TmpSocket::~TmpSocket()
{
    delete[] clientName;
}

void TmpSocket::onReceived()
{
    qDebug() << "got";
    if(!got_len){                       // if not already got the length
        if(sock->bytesAvailable() < 1)  // and if it is not available in the socket
            return;                     // return

        // If we reach here, the byte of the length is available in the socket
        char *tmp = new char[1];
        if(tmp == NULL){
            perror("tmp");
            exit(EXIT_FAILURE);
        }
        sock->read(tmp, 1);
        msg_len = (uint8_t) tmp[0];    // first (and only one) byte isi the length
        got_len = true;
        delete[] tmp;
    }

    // Here, the length is received
    if(sock->bytesAvailable() < msg_len)  // if msg is not available
        return ;                          // quit

    // msg received
    char *tmp = new char[1];
    if(tmp == NULL){
        perror("tmp");
        exit(EXIT_FAILURE);
    }
    sock->read(tmp, 1);
    clientType = (uint8_t) tmp[0];
    delete[] tmp;

    clientName = new char[msg_len];
    if(clientName == NULL){
        qDebug() << "Alloc error on clientName";
        exit(EXIT_FAILURE);
    }

    // alloc succeeded
    clientName[msg_len - 1] = '\0';
    sock->read(clientName, msg_len - 1); // reads the message in clientName
    qDebug() << "name: " << clientName;

    // From here all the parameters have been received
    disconnect(sock, SIGNAL(readyRead()), this, SLOT(onReceived()));    // disconnects this slot from the socket reception signal
    emit configCompleted(this);     // tells the parent of the config completeness
}

QTcpSocket* TmpSocket::getSocket()
{
    return sock;
}

uint8_t TmpSocket::getClientType()
{
    return clientType;
}

QString TmpSocket::getClientTypeAsQString()
{
    switch(clientType){
        case DEV_TYPE::LAMP: return "Lamp";
                             break;  // unreached break
        case DEV_TYPE::CAMERA: return "Camera";
                             break;

        case DEV_TYPE::DISPLAY: return "Display";
                             break;

        default: return "Unknown";
    }
}

QString TmpSocket::getClientName()
{
    QString to_return;
    to_return.append(clientName);
    return to_return;
}
