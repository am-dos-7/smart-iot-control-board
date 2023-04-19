#include <displayGui.h>

DisplayGui::DisplayGui(QTcpSocket *s, QWidget *p): BaseGui(s, p){

    QHBoxLayout * actionHLayout = new QHBoxLayout();
    textZone = new QLineEdit("Type text");
    sendButton = new QPushButton("Send");
    connect(sendButton, SIGNAL(clicked()), this, SLOT(onSendClicked()));
    actionHLayout->addWidget(textZone);
    actionHLayout->addWidget(sendButton);

    layout->addLayout(actionHLayout);
}

void DisplayGui::onReceived(){
    /* Nothing to do; so far a display object just sends data */
}

void DisplayGui::onSendClicked(){
    qDebug() << sock->write(textZone->text().toStdString().c_str()); // send text
    char tmp[2];
    tmp[0] = '\n';
    tmp[1] = '\0';
    qDebug() << sock->write(tmp);                           // and send '\n' as termination character
}

DisplayGui::~DisplayGui(){
    /* Nothing to do */
}
