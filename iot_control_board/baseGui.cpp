#include <baseGui.h>

uint8_t BaseGui::color_counter= 0;

BaseGui::BaseGui(QTcpSocket *s, QWidget *p): QWidget(p), sock(s){
    /* Here, the widgets used by all the child classes are declared,
     *  but not added to a layout. The add-to-layout part is handled
     *  in each class's constructor when the layout is declared.
     */
    clientType = new QLineEdit();
    clientType->setReadOnly(true);

    clientName = new QLineEdit();
    clientName->setReadOnly(true);

    connect(sock, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(sock, SIGNAL(readyRead()), this, SLOT(onReceived()));

    closeButton = new QPushButton("Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(onCloseRequested()));

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(clientType);
    hLayout->addWidget(clientName);
    hLayout->addWidget(closeButton);

    layout = new QVBoxLayout();
    layout->addLayout(hLayout);
    setLayout(layout);
    /* layout is a protected attribute which will be completed in each child-class.
     * The widgets common to all the classes are added here in BaseGui.
     * Those specific to each class will be declared an added in those classes' constructors
     */

    // set Background color
    QPalette *pal = new QPalette();
    set_color(pal, color_counter);
    color_counter++;        // update color_counter
    if(color_counter > 4)  // and keep it within the range
        color_counter = 0;

    pal->setColor(QPalette::WindowText, Qt::black);
    setAutoFillBackground(true);
    setPalette(*pal);
}

void BaseGui::disconnectSocket()
{
    sock->disconnectFromHost();
}

void BaseGui::setClientName(QString arg)
{
    clientName->setText(arg);
}

void BaseGui::setClientType(QString arg)
{
    clientType->setText(arg);
}

void BaseGui::onDisconnected()
{
    clientName->setText("Disconnected");

    emit closeSignal(this);  /* tells the control to remove this widget */
    qDebug() << "Disconnected";
}

void BaseGui::onConnected()
{
    /**/
}

void BaseGui::onCloseRequested()
{
    emit closeSignal(this);
}

void BaseGui::set_color(QPalette *p, uint8_t col)
{
    switch(col){
        case 0: p->setColor(QPalette::Window, Qt::darkCyan);
                break;
        case 1: p->setColor(QPalette::Window, Qt::darkMagenta);
                break;
        case 2: p->setColor(QPalette::Window, Qt::darkGray);
                break;
        case 3: p->setColor(QPalette::Window, Qt::darkYellow);
                break;
        case 4: p->setColor(QPalette::Window, Qt::darkGreen);
                break;
        default:    break;
    }
}

BaseGui::~BaseGui(){

}
