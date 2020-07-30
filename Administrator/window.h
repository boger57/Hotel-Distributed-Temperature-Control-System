#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <room.h>
#include <dbcontrol.h>
#include <QInputDialog>
#include <QMessageBox>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QMap>
#include <QList>
#include <QTimer>
#include <QPair>
#include <QMultiMap>
#include <QTimer>
#include<QJsonObject>
#include<QJsonDocument>
#include <QStandardItemModel>
#include<schedule.h>


#define managerON  0
#define managerOff 1

#define managerCold  1
#define managerHeat 0
#define managerUnset 99

#define Highwind 3
#define Midwind 2
#define Lowwind 1
#define Nowind 0
#define queuelen 2
//#define center_Air_C_ON  0
//#define center_Air_C_OFF  1

//#define center_Air_C_Freeze  1
//#define center_Air_C_Heat 0
//#define center_Air_C_Unset 99

//#define High_wind_speed 3
//#define Mid_wind_speed 2
//#define Low_wind_speed 1
//#define No_wind_speed 0
//#define queuelen 2
using namespace std;
class QTcpServer;

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void re_data(); //接收消息并调用处理函数
    void wait_queue();//等待队列检查

    void update_subui(); //房间信息UI刷新

private:
    Ui::Window *ui;
    int airc_mode; //中央空调工作模式：制冷（0）/制热（1）
    int airc_state; //中央空调工作状态：开启（0）/关闭（1）/等待（2）
    double default_temp;  //设置的缺省工作温度
    double fee_coe; //设置的计费费率


    int stand;//计费标准
    QMap<int,WaitQueueObject> waiting_queue;
    QMap<int,ServeQueueObject> serve_queue;
    QMap<int,Submachine> submacinfo;//房间信息列表

    void host_off(); //将中央空调改为关闭状态
    bool init_judge(); //开机初始设定的合法性判断
    QStandardItemModel *ItemModel = new QStandardItemModel();
    //void updateShowSubmacinfo(); //房间信息UI刷新

    /***************************************网络控制**/
    QTcpServer *mainserver;  //建立服务端
    QTcpSocket *clientConnection;
    quint16 blocksize;
    QMap<int,QTcpSocket*> roomsocketmap;  //已知房间号的对应的SOCKET链接
    QList<QTcpSocket*> unknownconnection;//未知房间号的SOCKET链接

    void TCP_init();//初始化TCP设置
    void TCP_new();//新建TCP链接
    void TCP_off();//关闭TCP链接
    void connection(); //建立连接并接受消息
    void deal_data(QJsonObject subrespond);//消息处理函数
    int get_servespeed(int askspeed);
    int get_time(QString str,int windspeed);



    /****************************************数据控制**/
    Dbcontrol *dataupload;//数据库操作

    /****************************************响应处理**/
    void send_data(QJsonObject info); //发送消息
    void subw_login(int roomid); //房间号为roomid的从控机登录请求响应
    void subw_quit(int roomid); //房间号为roomid的从控机退房请求响应
    void subw_start(int roomid); //房间号为roomid的从控机启动请求响应
    void subw_stop(int roomid);  //房间号为roomid的从控机停止请求响应
    void sub_wind_waitqueue(int roomid, int windspeed);
    void sub_wind_servequeue(int roomid, int windspeed);//房间号为roomid的从控机更改风速请求响应
    void subw_temp(int roomid, double targettemp); //房间号为roomid的从控机更改温度请求响应，想要的温度为targettemp
    void subw_mode(int roomid, QString model); //房间号为roomid的从控机更改模式请求响应，想要的模式为model
    void sub_windstart(int roomid, int windspeed); //房间号为roomid的从控机送风请求响应，风速为windspeed
    void sub_windstop(int roomid); //房间号为roomid的从控机停风请求响应
    void sub_updatetemp(int roomid, double roomtemp); //房间号为roomid的从控机当前状态更新报文响应
    void sub_towait(int roomid);//通知房间号为roomid的从控机已被移出服务队列
    void sub_toserve(int roomid, int windspeed); //通知房间号为roomid的从控机已进入服务队列，风速为windspeed

};

#endif // WINDOW_H
