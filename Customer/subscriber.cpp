#include "subscriber.h"
#include "ui_subscriber.h"
#include "dialogue.h"
#include <QtNetwork>

Subscriber::Subscriber(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Subscriber)
{
    ui->setupUi(this);
    this->setWindowTitle("Customer");
    QString btnStyle =
            "QPushButton:pressed{"
            "background-color:rgba(100,255,100,200);"
            "border-color:rgba(255,255,255,30);"
            "border-style:inset;"
            "color:rgba(0,0,0,100);"
            "}"
            "QPushButton:hover{"
            "background-color:rgba(100,255,100,100);"
            "border-color:rgba(255,255,255,200);"
            "color:rgba(0,0,0,200);"
            "}";
    ui->exit->setStyleSheet(btnStyle);
    ui->on->setStyleSheet(btnStyle);
    ui->set_mode->setStyleSheet(btnStyle);
    ui->set_temp->setStyleSheet(btnStyle);
    ui->set_temp_2->setStyleSheet(btnStyle);
    ui->set_wind->setStyleSheet(btnStyle);

    Dialogue *dlg = new Dialogue;
    //Correlation signal and slot function
    connect(dlg, SIGNAL(login_data(QString)), this, SLOT(login(QString)));

    dlg->show();

    tcp_Socket = new QTcpSocket(this);
    connect(tcp_Socket, &QTcpSocket::readyRead, this, &Subscriber::read_message);
    connect(tcp_Socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(display_error(QAbstractSocket::SocketError)));
    connect(this,SIGNAL(send_data(QJsonObject)),this,SLOT(send_message(QJsonObject)));

    QTimer *second_timer = new QTimer(this);
    connect(second_timer,SIGNAL(timeout()),this,SLOT(re_fresh()));
    second_timer->start(1000);

    QTimer *minute_timer = new QTimer(this);
    connect(minute_timer,SIGNAL(timeout()),this,SLOT(moni_tor()));
    minute_timer->start(10000);

    timer->setSingleShot(true);
    connect(timer,SIGNAL(timeout()),this,SLOT(temperature_send()));

    ui->lcd_currenttemp->setSmallDecimalPoint(true);
    ui->lcd_goaltemp->setSmallDecimalPoint(true);
    }

Subscriber::~Subscriber()
{
    delete ui;
}

void Subscriber::login(QString data)
{
    new_connect();
    QJsonObject rectJson;
    rectJson.insert("Action", "Login");
    rectJson.insert("roomID", data);
    room_id = data.toInt();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    //temperature_now  = qrand()%11 + 20; //Random current temperature
    temperature_now = 25;
    emit send_data(rectJson);
}

void Subscriber::new_connect()
{
    block_size = 0;  // Initialization data size information is 0
    tcp_Socket->abort(); // Cancel existing connections
    tcp_Socket->connectToHost("10.128.228.93", 5555);    // Create new connections
    tcp_Socket->waitForConnected(-1);
}

void Subscriber::read_message()
{
    QDataStream in(tcp_Socket);
    in.setVersion(QDataStream::Qt_5_9); //Setting the version of the data stream, the client and server side use the same version
    in >> block_size;
    in >> message;  //Store the received data in variables
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toLocal8Bit().data());
    QJsonObject respond = jsonDoc.object();

    qDebug()<<"Recv:    "<<respond;

    if(respond.value("Action").toString()=="Login_S"){  //Successful landing
        this->show();
        ui->set_mode->setEnabled(false);
        ui->set_temp->setEnabled(false);
        ui->set_temp_2->setEnabled(false);
        ui->set_wind->setEnabled(false);
        ui->exit->setEnabled(false);
    }
    else if(respond.value("Action").toString()=="Login_F"){  //Landing Failure
        QMessageBox::warning(this, QString::fromLocal8Bit(""), QString::fromLocal8Bit("The room number is incorrect."));

    }
    else if(respond.value("Action").toString()=="Turnon_S"){    //Successful startup
        initialization(respond);

        ui->lcd_goaltemp->display(temperature_set);
        ui->lcd_currenttemp->display(temperature_now);
        ui->wind_speed->setText(QString::number(speed));
        state=1;
        if(mode == 0)
            ui->model_of_main->setText("COLD");
        else if(mode == 1)
            ui->model_of_main->setText("HOT");

        ui->set_mode->setEnabled(true);
        ui->set_temp->setEnabled(true);
        ui->set_temp_2->setEnabled(true);
        ui->set_wind->setEnabled(true);
        ui->exit->setEnabled(true);
    }
    else if(respond.value("Action").toString()=="Turnon_F"){    //Boot failure
        QMessageBox::warning(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("Central air conditioning not turned on"));
    }
    else if(respond.value("Action").toString()=="Turnoff_S"){   //Shutdown
        ui->lcd_goaltemp->display("");
        ui->lcd_currenttemp->display("");
        ui->wind_speed->setText(NULL);
        state=0;
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        temperature_now  = qrand()%11 + 20; //Random current temperature

        ui->set_mode->setEnabled(false);
        ui->set_temp->setEnabled(false);
        ui->set_temp_2->setEnabled(false);
        ui->set_wind->setEnabled(false);
        ui->exit->setEnabled(false);
    }
    else if(respond.value("Action").toString()=="Changemode_S"){    //Switching mode
        if(mode == 0) mode = 1;
        else if(mode == 1) mode = 0;

        if(mode == 0)
            ui->model_of_main->setText("COLD");
        else if(mode == 1)
            ui->model_of_main->setText("HOT");

        if(mode == 0){  //refrigeration
            if(temperature_now<temperature_set){
                QJsonObject rectJson;
                rectJson.insert("Action", "Stopwind");
                rectJson.insert("roomID", room_id);

                emit send_data(rectJson);
            }
        }
        else{   //Heating
            if(temperature_now>temperature_set){
                QJsonObject rectJson;
                rectJson.insert("Action", "Stopwind");
                rectJson.insert("roomID", room_id);

                emit send_data(rectJson);
            }
        }
    }
    else if(respond.value("Action").toString()=="Changewind_S"){    //Switching Wind Speed
        speed = respond.value("requiredwindspeed").toVariant().toInt();
        ui->wind_speed->setText(QString::number(speed));
        speed_now = speed;
        ui->wind_speed_2->setText(QString::number(speed_now));
    }
    else if(respond.value("Action").toString()=="Changetemp_S"){    //Successful switching temperature
        temperature_set = respond.value("settem").toVariant().toInt();
        temperature = temperature_set;
        ui->lcd_goaltemp->display(temperature_set);

        if(mode == 0){  //refrigeration
            if(temperature_now<temperature_set){
                QJsonObject rectJson;
                rectJson.insert("Action", "Stopwind");
                rectJson.insert("roomID", room_id);

                emit send_data(rectJson);
            }
        }
        else{   //Heating
            if(temperature_now>temperature_set){
                QJsonObject rectJson;
                rectJson.insert("Action", "Stopwind");
                rectJson.insert("roomID", room_id);

                emit send_data(rectJson);
            }
        }
    }
    else if(respond.value("Action").toString()=="Changetemp_F"){    //Failure to switch temperature
        QMessageBox::warning(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("Temperature beyond range"));
    }
    else if(respond.value("Action").toString()=="Sendtemp_S"){  //Timing temperature
         double totalfee = respond.value("totalfee").toDouble();
         ui->cost->setText(QString::number(totalfee,'f',2));
    }
    else if(respond.value("Action").toString()=="Stopwind_S" || respond.value("Action").toString()=="STOP"){  //Stop air supply
        wind_state = 0;
        speed_now = 0;
        ui->wind_speed_2->setText(QString::number(speed_now));
    }
    else if(respond.value("Action").toString()=="Startwind_S" || respond.value("Action").toString()=="START"){ //Request for air supply
        wind_state = 1;
        speed_now = speed;
        ui->wind_speed_2->setText(QString::number(speed_now));
    }
    else if(respond.value("Action").toString()=="Checkout_S"){  //Check out
        double cost = respond.value("money").toDouble();
        ui->cost->setText(QString::number(cost,'f',2));
        tcp_Socket->disconnectFromHost();
    }
}

void Subscriber::send_message(QJsonObject info)
{
    QJsonDocument rectJsonDoc;
    rectJsonDoc.setObject(info);
    QString request(rectJsonDoc.toJson(QJsonDocument::Compact));
    qDebug()<<request;

    QByteArray block;   //For temporary storage of data to be sent
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_5_6);    //Setting the version of the data stream, the client and server side use the same version
    out << (quint16)0;  //Reserve two bytes
    out << request;

    out.device()->seek(0);  //  Jump to the beginning of a data block
    out << (quint16)(block.size() - sizeof(quint16));   //Fill in size information

    qDebug()<<"Send:    "<<block;
    tcp_Socket->write(block);
    tcp_Socket->flush();
}

void Subscriber::display_error(QAbstractSocket::SocketError)
{
    qDebug() << tcp_Socket->errorString();
    QMessageBox::warning(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("Connection error! please connect again"));
    exit(0);
}

void Subscriber::temperature_up() //Temperature rise
{
    if(timer->remainingTime()>0){
        timer->stop();
        timer->start(1000);
    }
    else
        timer->start(1000);

    temperature = temperature + 1;
    QJsonObject rectJson;
    rectJson.insert("Action", "Changetemp");
    rectJson.insert("roomID", room_id);
    rectJson.insert("requiredtemp", temperature);

    buffer = rectJson;
}

void Subscriber::temperature_down()   //Temperature drop
{
    if(timer->remainingTime()>0){
        timer->stop();
        timer->start(1000);
    }
    else
        timer->start(1000);

    temperature = temperature - 1;
    QJsonObject rectJson;
    rectJson.insert("Action", "Changetemp");
    rectJson.insert("roomID", room_id);
    rectJson.insert("requiredtemp", temperature);

    buffer = rectJson;
}

void Subscriber::speed_change()   //Change wind speed
{
    QJsonObject rectJson;
    rectJson.insert("Action", "Changewind");
    rectJson.insert("roomID", room_id);

    if(speed_now==0){
        if(speed==1)
            speed=2;
        else if(speed==2)
            speed=3;
        else if(speed==3)
            speed=1;
        ui->wind_speed->setText(QString::number(speed));
    }
    else if(speed_now!=0){
        if(speed==1)
            rectJson.insert("requiredwindspeed", 2);
        else if(speed==2)
            rectJson.insert("requiredwindspeed", 3);
        else if(speed==3)
            rectJson.insert("requiredwindspeed", 1);

        emit send_data(rectJson);
    }
}

void Subscriber::state_change()   //switch
{
    QJsonObject rectJson;
    if(state==0){
        rectJson.insert("Action", "Turnon");
        rectJson.insert("roomID", room_id);
    }
    else if(state==1){
        rectJson.insert("Action", "Turnoff");
        rectJson.insert("roomID", room_id);
    }
    emit send_data(rectJson);
}

void Subscriber::mode_change()    //Change mode
{
    QJsonObject rectJson;
    rectJson.insert("Action", "Changemode");
    rectJson.insert("roomID", room_id);

    if(mode==0)
        rectJson.insert("mode", "hot");
    else if(mode==1)
        rectJson.insert("mode", "cold");

    emit send_data(rectJson);
}

void Subscriber::initialization(QJsonObject respond)   //Air conditioning initialization
{
    speed = respond.value("windspeed").toVariant().toInt(); //Default wind speed
    state = 0;  //Default shutdown
    if(respond.value("mode").toString()=="cold") //Default mode
        mode = 0;
    else if(respond.value("mode").toString()=="hot")
        mode = 1;
    temperature_set = respond.value("starttemp").toVariant().toInt();    //Default temperature
    temperature = temperature_set;
    temperature_init = temperature_now;
    wind_state = 0;
    speed_now = 0;

    int ww;
    if(mode==0 && temperature_now<=temperature_set)    //Refrigeration and currently less than set
        ww = 0;
    else if(mode==1 && temperature_now>=temperature_set)   //Heating and currently larger than set
        ww = 0;
    else if(mode==0 && temperature_now>temperature_set)    //Refrigeration and currently larger than set
        ww = 1;
    else if(mode==1 && temperature_now<temperature_set)    //Heating and currently less than set
        ww = 1;

    if(ww==1){
        QJsonObject rectJson;
        rectJson.insert("Action", "Startwind");
        rectJson.insert("roomID", room_id);
        rectJson.insert("requiredwindspeed", speed);

        emit send_data(rectJson);
    }
}

void Subscriber::re_fresh()    //Refresh Real-time Temperature
{
    if(state == 1){ //Start-up status
        if(mode==0 && speed_now==0){    //Refrigeration and currently less than set
            temperature_now = temperature_now + 0.5/60;
        }
        else if(mode==1 && speed_now==0){   //Heating and currently less than set
            temperature_now = temperature_now - 0.5/60;
        }
        else if(mode==0 && wind_state==1){    //Refrigeration and currently less than set
            if(speed_now == 1)
                temperature_now = temperature_now - 0.4/60;
            else if(speed_now == 2)
                temperature_now = temperature_now - 0.5/60;
            else
                temperature_now = temperature_now - 0.6/60;
        }
        else if(mode==1 && wind_state==1){    //Heating and currently less than set
            if(speed_now == 1)
                temperature_now = temperature_now + 0.4/60;
            else if(speed_now == 2)
                temperature_now = temperature_now + 0.5/60;
            else
                temperature_now = temperature_now + 0.6/60;
        }

        if(qAbs(temperature_now - temperature_set)<=0.01 && wind_state==1){  //Temperature reaches target and regular air supply is required to stop.
            //wind_state = 0;
            QJsonObject rectJson;
            rectJson.insert("Action", "Stopwind");
            rectJson.insert("roomID", room_id);

            emit send_data(rectJson);
        }
        if(qAbs(temperature_now - temperature_set)>=1 && wind_state==0){ //Currently beyond target 1 degree, air supply is required
            //wind_state = 1;
            QJsonObject rectJson;
            rectJson.insert("Action", "Startwind");
            rectJson.insert("roomID", room_id);
            rectJson.insert("requiredwindspeed", speed);

            emit send_data(rectJson);
        }

        ui->lcd_currenttemp->display(QString::number(temperature_now,'f',2));
    }

}

void Subscriber::moni_tor()    //Timely send current temperature
{
    if(state == 1){
        QJsonObject rectJson;
        rectJson.insert("Action", "Sendtemp");
        rectJson.insert("roomID", room_id);

        QString str = QString::number(temperature_now, 'f', 2);
        rectJson.insert("currenttemp", str);

        emit send_data(rectJson);
    }
}

void Subscriber::check_out()   //Check out
{
    QJsonObject rectJson;
    rectJson.insert("Action", "Checkout");
    rectJson.insert("roomID", room_id);

    state = 0;
    ui->on->setEnabled(false);
    ui->set_mode->setEnabled(false);
    ui->set_temp->setEnabled(false);
    ui->set_temp_2->setEnabled(false);
    ui->set_wind->setEnabled(false);
    ui->exit->setEnabled(false);
    emit send_data(rectJson);
}

void Subscriber::close_event(QCloseEvent * event)
{
    QJsonObject rectJson;
    rectJson.insert("Action", "Checkout");
    rectJson.insert("roomID", room_id);

    emit send_data(rectJson);
    exit(0);
}

void Subscriber::temperature_send()
{
    emit send_data(buffer);
}

