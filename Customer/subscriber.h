#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCloseEvent>

class QTcpSocket;

namespace Ui {
class Subscriber;
}

class Subscriber : public QMainWindow
{
    Q_OBJECT

public:
    explicit Subscriber(QWidget *parent = 0);
    ~Subscriber();

private:
    Ui::Subscriber *ui;
    QTcpSocket *tcp_Socket;
    QString message;
    quint16 block_size;  //Size information used to store data

    int room_id; //Room number
    int speed;  //Set wind speed, 0123, no�� low, medium and high
    int speed_now;  //Current wind speed, 0123, no low, medium and high
    double temperature_now; //Current temperature
    double temperature_init;    //initial temperature
    double temperature_set; //setting temperature
    int mode;   //0 refrigeration, 1 heating
    int state = 0;  //Turn off 0, turn on 1
    int wind_state; //0 stop air, 1 supply air
    double temperature;
    QTimer *timer = new QTimer(this);
    QJsonObject buffer;

    void initialization(QJsonObject respond);    //Air conditioning initialization

signals:
    void send_data(QJsonObject info);

private slots:
    void login(QString data);   //A slot for receiving transmitted data.
    void new_connect();
    void read_message();
    void send_message(QJsonObject info);
    void display_error(QAbstractSocket::SocketError);

    void temperature_up();  //Temperature rise
    void temperature_down();    //Temperature drop
    void speed_change();    //Change wind speed
    void state_change();  //switch
    void mode_change(); //Patterns change
    void check_out();

    void re_fresh(); //Refresh Real-time Temperature
    void moni_tor(); //Timely send current temperature
    void temperature_send();

    void close_event(QCloseEvent * event);
    void on_set_temp_clicked();
    void on_exit_clicked();
};

#endif // SUBSCRIBE_H
