/* 01/09/2022
 * Changes made to all the code architecture
 * BaseGui is the base class for all the GUI classes
 * dedicated to the different device types. It holds
 * the attributes and methods common to the those classes
 * in order not to repeat the same code again and again.
 * It also and mainly serves to resolve a conflict/problem
 * in a switch section somewhere in the ControlBoard class.
 *
 * Last edited: 19/04/2023
 */

#ifndef BASEGUI_H
#define BASEGUI_H
#include <QWidget>
#include <QObject>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class BaseGui: public QWidget{
    Q_OBJECT
public:
    BaseGui(QTcpSocket *, QWidget *p=NULL);
    ~BaseGui();
    void disconnectSocket();
    void setClientType(QString);
    void setClientName(QString);

signals:
    void closeSignal(QWidget *);

public slots:
    void onDisconnected();
    void onConnected();
    virtual void onReceived()=0;
    void onCloseRequested();

protected:
    void set_color(QPalette *, uint8_t);

protected:          // because of the inheritance by other classes
    QLineEdit *clientType;
    QLineEdit *clientName;
    QTcpSocket *sock;
    QPushButton *closeButton;
    static uint8_t color_counter;    // used to change color from one LampGui to another
    QVBoxLayout *layout;
};

#endif // BASEGUI_H
