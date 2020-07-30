#include"dbcontrol.h"
#include<QDebug>
Dbcontrol::Dbcontrol(QObject *parent) :
    QObject(parent)
{

    //QSqlDatabase创建连接数据库实例,一个QSqlDatabase的实例代表了一个数据库连接。
    if(QSqlDatabase::contains("linkdata.db"))
    //判断是否已经创建数据库文件
    //连接QSQLITE类型的数据库linkdata.db
    {
        db = QSqlDatabase::database("linkdata.db");
    }
    else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:/Users/1wxyw/Desktop/linkdata.db");//database name
        bool ok = db.open();
        if(ok==true)
            qDebug()<<"success";
    }
}

Dbcontrol::~Dbcontrol()
{
    db.close();
}

void Dbcontrol::sub_quit(int room_id)
{
    QSqlQuery query(db);
    QDateTime currenttime=QDateTime::currentDateTime();
    query.prepare("update room set in_state=0, open_close =0, nowfanspeed = 0,updatetime = ? where roomid=? ");
    query.bindValue(0,currenttime);
    query.bindValue(1,room_id);
    query.exec();
}

int Dbcontrol::host_login(QString username, QString secret){
    QSqlQuery query;
    query.prepare("SELECT secret FROM user where username=?");
    query.bindValue(0,username);
    query.exec();
    if(query.next()==false)
        return -1;
    else{
        QString makeshure=query.value(0).toString();
        if(secret==makeshure){
            query.prepare("SELECT kind FROM user where username=?");
            query.bindValue(0,username);
            query.exec();
            if(query.first())
                return query.value(0).toInt();
        }
        else
            return -2;
    }
}

bool Dbcontrol::sub_turnon(int room_id)
//从控机开机请求
{
    qDebug()<<"进入从控机开机请求数据库操作函数";
    QSqlQuery query(db);
    QDateTime currenttime=QDateTime::currentDateTime();
    query.prepare("update room set open_close=1,updatetime = ?  where roomid=?");
    query.bindValue(0,currenttime);
    query.bindValue(1,room_id);
    query.exec();
}

bool Dbcontrol::sub_login(int room_id)
//查询房间登录信息，登陆成功返回ture，其他返回false
{
    qDebug()<<"进入房间登陆请求数据库操作函数";
    QSqlQuery query(db);
    query.prepare("select in_state from room where roomid=?");
    query.bindValue(0,room_id);
    query.exec();
    //获取该房间登录状态
    //判断表中是否有此房间号
    if(query.next()==false) {
        return false;
        //无此房间
    }
    else {//有此房间
        int in_state=query.value(0).toInt();
        qDebug()<<"该房间读取的登录状态为"<<in_state;
        if(in_state==0)
        //判断该房间的入住状态，未入住为0
        {
            QDateTime currenttime=QDateTime::currentDateTime();
            query.prepare("update room set in_state=1,count_schedule =0, updatetime = ?   where roomid=? ");
            //更新入住状态为1,被调度次数置为0
            query.bindValue(0,currenttime);
            query.bindValue(1,room_id);
            query.exec();
            qDebug()<<"登录成功";
            return true;
            //登陆成功
        }
        else {
            qDebug()<<"登录失败";
            return false;
            //已有入住登陆失败
        }
    }
}
void Dbcontrol::host_turnon()
{
    qDebug()<<"进入中央空调开机请求数据库操作函数";
    QSqlQuery query(db);
    query.prepare("update center_air set open_close=?");
    query.bindValue(0,1);
    query.exec();
}

void Dbcontrol::host_turnoff()
{
    qDebug()<<"进入中央空调关机请求数据库操作函数";
    QSqlQuery query(db);
    query.prepare("update center_air set open_close=?");
    query.bindValue(0,0);
    query.exec();
}

void Dbcontrol::host_cmode(int mode)
{
    qDebug()<<"进入中央空调默认模式更改请求数据库操作函数";
    QSqlQuery query(db);
    query.prepare("update center_air set defaultmode=?");
    query.bindValue(0,mode);
    query.exec();
}

void Dbcontrol::host_ctemp(double defaulttemp,double maxtemp,double mintemp)
{
    qDebug()<<"进入中央空调缺省温度更改请求数据库操作函数";
    QSqlQuery query(db);
    query.prepare("update center_air set defaulttemp=?,maxtemp=?,mintemp=?");
    query.bindValue(0,defaulttemp);
    query.bindValue(1,maxtemp);
    query.bindValue(2,mintemp);
    query.exec();
}

void Dbcontrol::host_crate(double feerate)
{
    qDebug()<<"进入中央空调计费费率更改请求数据库操作函数";
    QSqlQuery query(db);
    query.prepare("update center_air set feerate=?");
    query.bindValue(0,feerate);
    query.exec();
}

int Dbcontrol::host_initmode()
{
    qDebug()<<"进入读取中央空调默认模式数据库操作函数";
    int num;
    QSqlQuery query(db);
    query.prepare("select defaultmode from center_air");
    query.exec();
    while(query.next())
     num = query.value(0).toInt();
    return num;
}

double Dbcontrol::host_inittemp()
{
    qDebug()<<"进入读取中央空调缺省温度数据库操作函数";
    double y;
    QSqlQuery query(db);
    query.prepare("select defaulttemp from center_air");
    query.exec();
    while(query.next())
        y=query.value(0).toDouble();
    return y;
}

double Dbcontrol::host_initrate()
{
    qDebug()<<"进入读取中央空调费率数据库操作函数";
    double y;
    QSqlQuery query(db);
    query.prepare("select feerate from center_air");
    query.exec();
    while(query.next())
        y=query.value(0).toDouble();
    return y;
}

void Dbcontrol::sub_cwind_servequeue(int room_id,int requiredwindspeed)

 {
     QSqlQuery query(db);
     QDateTime currenttime=QDateTime::currentDateTime();
     query.prepare("update room set setfanspeed=?,nowfanspeed=?,updatetime = ? where roomid=?");
     query.bindValue(0,requiredwindspeed);
     query.bindValue(1,requiredwindspeed);
     query.bindValue(2,currenttime);
     query.bindValue(3,room_id);
     query.exec();
 }


void Dbcontrol::sub_cwind_waitqueue(int room_id,int requiredwindspeed)
 {
     qDebug()<<"进入等待队列中从控机修改风速请求数据库操作函数";
     QSqlQuery query(db);
     QDateTime currenttime=QDateTime::currentDateTime();
     query.prepare("update room set setfanspeed=? ,updatetime = ? where roomid=?");
     query.bindValue(0,requiredwindspeed);
     query.bindValue(1,currenttime);
     query.bindValue(2,room_id);
     query.exec();
 }
void Dbcontrol::sub_turnoff(int room_id){
     QSqlQuery query(db);
     QDateTime currenttime=QDateTime::currentDateTime();
     query.prepare("update room set open_close=?,updatetime = ? where roomid=?");
     query.bindValue(0,0);
     query.bindValue(1,currenttime);
     query.bindValue(2,room_id);
     query.exec();
}


void Dbcontrol::sub_ctemp(int room_id,double requiredtemp)
 {
     QSqlQuery query(db);
     QDateTime currenttime=QDateTime::currentDateTime();
     query.prepare("update room set settemp=?,updatetime = ? where roomid=?");

     query.bindValue(0,requiredtemp);
     query.bindValue(1,currenttime);
     query.bindValue(2,room_id);
     query.exec();

 }

void Dbcontrol::sub_cmode(int room_id,bool mode)
 {
     QSqlQuery query(db);
     QDateTime currenttime=QDateTime::currentDateTime();
     query.prepare("update room set mode=? ,updatetime = ? where roomid=?" );
     query.bindValue(0,mode);
     query.bindValue(1,currenttime);
     query.bindValue(2,room_id);
     query.exec();
 }

void Dbcontrol::sub_startwind(int room_id,int windspeed)
 {
      qDebug()<<"进入从控机送风请求数据库操作函数";
      QSqlQuery query(db);
      QDateTime currenttime=QDateTime::currentDateTime();
      query.prepare("update room set nowfanspeed=? ,updatetime = ? where roomid=? ");
      query.bindValue(0,windspeed);
      query.bindValue(1,currenttime);
      query.bindValue(2,room_id);
      query.exec();
 }

void Dbcontrol::sub_stopwind(int room_id)
 {
     qDebug()<<"进入从控机停风请求数据库操作函数";
     QSqlQuery query(db);
     QDateTime currenttime=QDateTime::currentDateTime();
     query.prepare("update room set nowfanspeed=0,updatetime = ? where roomid=? ");
     query.bindValue(0,currenttime);
     query.bindValue(1,room_id);
     query.exec();
 }

void Dbcontrol::sub_ctemp2(int room_id,double currenttemp)
{
    qDebug()<<"进入从控机定期更新请求数据库操作函数";
    QSqlQuery query(db);
    QDateTime currenttime=QDateTime::currentDateTime();
    query.prepare("update room set nowtemp=?,updatetime = ? where roomid=? ");
    query.bindValue(0,currenttemp);
    query.bindValue(1,currenttime);
    query.bindValue(2,room_id);
    query.exec();
}

double Dbcontrol::sub_totalfee(int room_id) /************************遍历**/
//获取从控机总花费
{
    qDebug()<<"进入从控机总花费获取数据库操作函数";
    double totalfee=0;
    QSqlQuery query(db);

    query.exec(QString("select fee from adjust where roomid=%1").arg(room_id));
    while(query.next())
    {
        totalfee = totalfee + query.value(0).toDouble();
    }
    return totalfee;
}

void Dbcontrol::sub_scheduletimes(int room_id, int num)
{
    qDebug()<<"进入从控机调度次数更新数据库操作函数";
    int old_num = 0;
    QSqlQuery query(db);
    query.exec(QString("select count_schedule from room where roomid=%1").arg(room_id));

    query.next();
    old_num = query.value(0).toInt();
    qDebug()<<"该房间读取的次数为"<<old_num;
    QDateTime currenttime=QDateTime::currentDateTime();
    query.prepare("update room set count_schedule = ? ,updatetime = ? where roomid=? ");
    query.bindValue(0,old_num+num);
    query.bindValue(1,currenttime);
    query.bindValue(2,room_id);
    query.exec();
}


void Dbcontrol::adjust_on(int room_id,Submachine Info)
//从控机开机操作写报表函数
{
      qDebug()<<"进入开机操作写报表函数";
      qDebug()<<Info.current_temp<<Info.set_temp<<Info.fan_speed<<Info.set_speed<<Info.now_model<<Info.use_fee;
      int old_count_use = 0;
      int old_count_all = 0;
      int old_count_temp = 0;
      QDateTime old_starttime;
      QDateTime currenttime=QDateTime::currentDateTime();
      QSqlQuery query(db);
      //判断是否为该房间adjust第一条记录，即此前是否有开过机
      query.prepare("select starttime from adjust where roomid=? order by starttime desc limit 0,1");
      query.bindValue(0,room_id);
      query.exec();
      if(query.next()==false); //若不为第一条
      else
      {
          //获得上一条记录开始时间
          old_starttime = query.value(0).toDateTime();
          //获得开关空调次数,统计详单记录数,达到目标温度次数
          query.prepare("select count_use,count_all,count_settemp from adjust where roomid=? order by starttime desc limit 0,1");
          query.bindValue(0,room_id);
          query.exec();
          query.next();
          old_count_use=query.value(0).toInt();
          old_count_all=query.value(1).toInt();
          old_count_temp=query.value(2).toInt();
          //更新上一条记录的终止时间为当前时间，更新上一条记录费用
          query.prepare("update adjust set stoptime=? ,fee=? where roomid=? and starttime=?");
          query.bindValue(0,currenttime);
          query.bindValue(1,0); //关机持续期间费用为0
          query.bindValue(2,room_id);
          query.bindValue(3,old_starttime);
          query.exec();
          /*若此前关机，改关机记录未写入终止时间，再开机时需要以上操作*/
      }

        //更新最新一条详单                   0     1    2         3       4          5        6       7            8          9         10          11
      query.prepare("insert into adjust(roomid,mode,starttime,in_state,open_close,nowtemp,settemp,nowfanspeed,setfanspeed,count_all,count_use,count_settemp) values(?,?,?,?,?,?,?,?,?,?,?,?)");
      query.bindValue(0,room_id);
      if(Info.now_model.compare("cold") == 0)
        query.bindValue(1,1);
      else
        query.bindValue(1,0);
      query.bindValue(2,currenttime);
      query.bindValue(3,1);
      query.bindValue(4,1); //状态改为开机
      query.bindValue(5,Info.current_temp);
      query.bindValue(6,Info.set_temp);
      query.bindValue(7,Info.fan_speed);
      query.bindValue(8,Info.set_speed);
      query.bindValue(9,old_count_all+1);
      query.bindValue(10,old_count_use+1);
      query.bindValue(11,old_count_temp);
      query.exec();
      /*以上操作无终止时间，需后续操作进行写入*/

      //更新adjust_temp表
      //更新上一条adjust_temp的stoptime
      query.prepare("select starttime from adjust_temp where roomid=? order by starttime desc limit 0,1");
      query.bindValue(0,room_id);
      query.exec();
      if(query.next()==false);
      else{
          QDateTime prevtime = query.value(0).toDateTime();
          query.prepare("update adjust_temp set stoptime=?  where roomid=? and starttime=?");
          query.bindValue(0,currenttime);
          query.bindValue(1,room_id);
          query.bindValue(2,prevtime);
          query.exec();

      }
      //加一条新纪录
      query.prepare("insert into adjust_temp(roomid,starttime,settemp) values(?,?,?)");
      query.bindValue(0,room_id);
      query.bindValue(1,currenttime);
      query.bindValue(2,Info.set_temp);
      query.exec();
}

void Dbcontrol::adjust_off(int room_id, Submachine Info)
//从控机关机写报表
{
    qDebug()<<"进入关机操作写报表函数";
    qDebug()<<Info.current_temp<<Info.set_temp<<Info.fan_speed<<Info.set_speed<<Info.now_model<<Info.use_fee;
    int old_count_use = 0;
    int old_count_all = 0;
    int old_count_temp = 0;
    QDateTime old_starttime;
    QDateTime currenttime=QDateTime::currentDateTime();
    QSqlQuery query(db);
    //获取前一条记录
    query.prepare("select starttime from adjust where roomid=? order by starttime desc limit 0,1");
    query.bindValue(0,room_id);
    query.exec();
    if(query.next()==false); //若不为第一条
    else
    {
        //获得上一条记录开始时间
        old_starttime = query.value(0).toDateTime();
        //获得开关空调次数,统计详单记录数,达到目标温度次数
        query.prepare("select count_use,count_all,count_settemp from adjust where roomid=? order by starttime desc limit 0,1");
        query.bindValue(0,room_id);
        query.exec();
        query.next();
        old_count_use=query.value(0).toInt();
        old_count_all=query.value(1).toInt();
        old_count_temp=query.value(2).toInt();
        //更新上一条记录的终止时间为当前时间,更新上一条记录费用
        query.prepare("update adjust set stoptime=? ,fee = ? where roomid=? and starttime=?");
        query.bindValue(0,currenttime);
        query.bindValue(1,Info.use_fee);
        query.bindValue(2,room_id);
        query.bindValue(3,old_starttime);
        query.exec();
    }

      //更新最新一条详单                   0     1    2         3       4          5        6       7            8          9
    query.prepare("insert into adjust(roomid,mode,starttime,in_state,open_close,nowtemp,settemp,nowfanspeed,setfanspeed,count_all,count_use,count_settemp) values(?,?,?,?,?,?,?,?,?,?,?,?)");
    query.bindValue(0,room_id);
    if(Info.now_model.compare("cold") == 0)
      query.bindValue(1,1);
    else
      query.bindValue(1,0);
    query.bindValue(2,currenttime);
    query.bindValue(3,1);
    query.bindValue(4,0);  //状态改为关机
    query.bindValue(5,Info.current_temp);
    query.bindValue(6,Info.set_temp);
    query.bindValue(7,Info.fan_speed);
    query.bindValue(8,Info.set_speed);
    query.bindValue(9,old_count_all+1);
    query.bindValue(10,old_count_use); //一次开关机统计一次，开机时已经+1，关机则不必再统计
    query.bindValue(11,old_count_temp);
    query.exec();


    //更新adjust_temp表
    //更新上一条adjust_temp的stoptime
    query.prepare("select starttime from adjust_temp where roomid=? order by starttime desc limit 0,1");
    query.bindValue(0,room_id);
    query.exec();
    if(query.next()==false);
    else{
        QDateTime prevtime = query.value(0).toDateTime();
        query.prepare("update adjust_temp set stoptime=?  where roomid=? and starttime=?");
        query.bindValue(0,currenttime);
        query.bindValue(1,room_id);
        query.bindValue(2,prevtime);
        query.exec();

    }
    //加一条新纪录
    query.prepare("insert into adjust_temp(roomid,starttime,settemp) values(?,?,?)");
    query.bindValue(0,room_id);
    query.bindValue(1,currenttime);
    query.bindValue(2,Info.set_temp);
    query.exec();
}

void Dbcontrol::adjust_others(int room_id, Submachine Info)
//修改其他参数写报表
{
    qDebug()<<"进入修改操作写报表函数";
    qDebug()<<Info.current_temp<<Info.set_temp<<Info.fan_speed<<Info.set_speed<<Info.now_model<<Info.use_fee;
    int old_count_use = 0;
    int old_count_all = 0;
    int old_count_temp = 0;
    QDateTime old_starttime;
    QDateTime currenttime=QDateTime::currentDateTime();
    QSqlQuery query(db);
    //获取前一条记录
    query.prepare("select starttime from adjust where roomid=? order by starttime desc limit 0,1");
    query.bindValue(0,room_id);
    query.exec();
    if(query.next()==false); //若不为第一条
    else
    {
        //获得上一条记录开始时间
        old_starttime = query.value(0).toDateTime();
        //获得开关空调次数,统计详单记录数,达到目标温度次数
        query.prepare("select count_use,count_all,count_settemp from adjust where roomid=? order by starttime desc limit 0,1");
        query.bindValue(0,room_id);
        query.exec();
        query.next();
        old_count_use=query.value(0).toInt();
        old_count_all=query.value(1).toInt();
        old_count_temp=query.value(2).toInt();
        //更新上一条记录的终止时间为当前时间，更新上一条记录费用
        query.prepare("update adjust set stoptime=?,fee = ?  where roomid=? and starttime=?");
        query.bindValue(0,currenttime);
        query.bindValue(1,Info.use_fee);
        query.bindValue(2,room_id);
        query.bindValue(3,old_starttime);
        query.exec();
    }

      //更新最新一条详单                   0     1    2         3       4          5        6       7            8          9
    query.prepare("insert into adjust(roomid,mode,starttime,in_state,open_close,nowtemp,settemp,nowfanspeed,setfanspeed,count_all,count_use,count_settemp) values(?,?,?,?,?,?,?,?,?,?,?,?)");
    query.bindValue(0,room_id);
    if(Info.now_model.compare("cold") == 0)
      query.bindValue(1,1);
    else
      query.bindValue(1,0);
    query.bindValue(2,currenttime);
    query.bindValue(3,1);
    query.bindValue(4,1);
    query.bindValue(5,Info.current_temp);
    query.bindValue(6,Info.set_temp);
    query.bindValue(7,Info.fan_speed);
    query.bindValue(8,Info.set_speed);
    query.bindValue(9,old_count_all+1);
    query.bindValue(10,old_count_use); //一次开关机统计一次,不变
    if(Info.current_temp == Info.set_temp)
        old_count_temp ++; //如果当前温度等于目标温度，则达到目标温度次数+1
    query.bindValue(11,old_count_temp);
    query.exec();

    //更新adjust_temp表
    //更新上一条adjust_temp的stoptime
    query.prepare("select starttime from adjust_temp where roomid=? order by starttime desc limit 0,1");
    query.bindValue(0,room_id);
    query.exec();
    if(query.next()==false);
    else{
        QDateTime prevtime = query.value(0).toDateTime();
        query.prepare("update adjust_temp set stoptime=?  where roomid=? and starttime=?");
        query.bindValue(0,currenttime);
        query.bindValue(1,room_id);
        query.bindValue(2,prevtime);
        query.exec();

    }
    //加一条新纪录
    query.prepare("insert into adjust_temp(roomid,starttime,settemp) values(?,?,?)");
    query.bindValue(0,room_id);
    query.bindValue(1,currenttime);
    query.bindValue(2,Info.set_temp);
    query.exec();
}
