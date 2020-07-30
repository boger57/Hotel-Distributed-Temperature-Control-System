#ifndef DBCONTROL_H
#define DBCONTROL_H

#include <QObject>
#include <QSqlDatabase>
#include <room.h>

#include <QVector>
#include <QSqlQuery>
#include <QDateTime>

class Dbcontrol : public QObject
{
    Q_OBJECT
public:
    explicit Dbcontrol(QObject *parent);
    //add data to database
    ~Dbcontrol();

    int host_login(QString username,QString secret);

    bool sub_login(int room_id); //房间号为room_id的从控机登陆
    bool sub_turnon(int room_id); //房间号为room_id的从控机开机
    void sub_turnoff(int room_id); //房间号为room_id的从控机关机
    void sub_cmode(int room_id,bool mode); //从控机更改模式mode=1为制冷，0为制热
    void sub_cwind_servequeue(int room_id,int requiredwindspeed); //从控机更改风速为requiredwindspeed（服务队列中）
    void sub_cwind_waitqueue(int room_id,int requiredwindspeed); //从控机更改风速为requiredwindspeed（等待队列中）
    void sub_ctemp(int room_id,double requiredtemp); //从控机更改设定温度=requiredtemp
    void sub_ctemp2(int room_id,double currenttemp); //从控机更新自身温度=currenttemp
    void sub_stopwind(int room_id); //房间号为room_id的从控机停风
    void sub_startwind(int room_id, int windspeed); //房间号为room_id的从控机送风，风速为windspeed
    void sub_quit(int room_id); //房间号为room_id的从控机退房
    void sub_scheduletimes(int room_id,int num); //从控机被调度次数+=num
    double sub_totalfee(int room_id);//获取从控机总花费double型
    void host_turnon();//打开中央空调
    void host_turnoff();//关闭中央空调
    void host_cmode(int mode);//改变中央空调模式
    void host_ctemp(double defaulttemp,double maxtemp,double mintemp);//改变中央空调的温度
    void host_crate(double feerate);//改变中央空调的计费率为feerate
    double host_initrate();//读取中央空调设置费率
    double host_inittemp();//读取中央空调缺省温度
    int host_initmode();//读取中央空调默认模式
    void adjust_on(int room_id,Submachine Info);//开机写数据库报表函数
    void adjust_off(int room_id,Submachine Info);//关机写数据库报表函数
    void adjust_others(int room_id,Submachine Info);//其他修改操作写数据库报表函数

private:
    QSqlDatabase db;
};



#endif // DBCONTROL_H
