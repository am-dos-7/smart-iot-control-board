#include <controlBoard.h>

ControlBoard::ControlBoard(quint16 portNb , QWidget *p): QWidget(p), portNumber(portNb)
{
    serverStatus = new QLabel();

    layout = new QGridLayout;
    layout->addWidget(serverStatus);

    server = new QTcpServer();
    if(server->listen(QHostAddress::Any, portNumber)){
        serverStatus->setText("<b>Server started at <i>" + server->serverAddress().toString() +
                              "<i> on port <i>" + QString::number(server->serverPort())+ "<i><b>!");
        connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));        // slot to be called on a new connection
    }
    else{
        serverStatus->setText("Server failed to start up!");
    }

    // set windowd color
    QPalette *pal = new QPalette();
    pal->setColor(QPalette::Window, Qt::black);
    pal->setColor(QPalette::WindowText, Qt::white);
    setAutoFillBackground(true);
    setPalette(*pal);

    setLayout(layout);
//    setWindowTitle("IoT Monitor");
    setWindowTitle("IoT ViewBoard");

//    scrollBar = new QScrollArea();
//    scrollBar->setWidget(this);
}

void ControlBoard::onCloseRequested(QWidget *w)
{
//    qDebug() << "Close requested";

    layout->removeWidget(w);    /* remove the widget from the layout display*/
    LampGui *c_soc = (LampGui*) w;
    c_soc->disconnectSocket();
    w->deleteLater();          /* and delete it*/
}

void ControlBoard::onNewConnection()
{
    qDebug() << "New connection";
    QTcpSocket *sock = server->nextPendingConnection(); // gets the incoming connection socket from server
    addTmpSocket(sock);                                // and creates a TmpSocket with it
}

void ControlBoard::onConfigCompleted(TmpSocket *s)
{
    BaseGui *cSock;
    switch(s->getClientType()){
        case DEV_TYPE::LAMP:
                    qDebug() << "ClientType: Lamp";
                    cSock = new LampGui(s->getSocket());
                    cSock->setClientType(s->getClientTypeAsQString());   // gets clientType from s to csock
                    cSock->setClientName(s->getClientName());
                    connect(cSock, SIGNAL(closeSignal(QWidget*)), this, SLOT(onCloseRequested(QWidget*)));  // connects the close signal

                    layout->addWidget(cSock);
                    break;
        case DEV_TYPE::DISPLAY:
                    qDebug() << "ClientType: Display";
                    cSock = new DisplayGui(s->getSocket());
                    cSock->setClientType(s->getClientTypeAsQString());   // gets clientType from s to csock
                    cSock->setClientName(s->getClientName());
                    connect(cSock, SIGNAL(closeSignal(QWidget*)), this, SLOT(onCloseRequested(QWidget*)));  // connects the close signal

                    layout->addWidget(cSock);
                    break;

        default: // Just disconnect the client; device type not supported
                    break;
    }

    delete s;       // delete the object s; no longer needed
}

void ControlBoard::addTmpSocket(QTcpSocket *s)
{
    TmpSocket *tmp = new TmpSocket(s);
    connect(tmp, SIGNAL(configCompleted(TmpSocket*)), this, SLOT(onConfigCompleted(TmpSocket*))); // onConfigComplete will be called when the socket config is terminated
}


ControlBoard::~ControlBoard()
{

}
