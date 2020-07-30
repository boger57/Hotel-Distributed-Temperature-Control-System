#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QDateTime>
class QueueObject{
public:
    int roomid;
    int windspeed;
    time_t starttime;
    float temp;

    QueueObject(){}
    QueueObject(int roomid,int windspeed,time_t starttime,float temp){
        this->roomid = roomid;
        this->windspeed = windspeed;
        this->starttime = starttime;
        this->temp = temp;
    }
};
class WaitQueueObject:public QueueObject{
public:
    QString reason;
    WaitQueueObject(){}
    WaitQueueObject(int roomid,int windspeed,time_t starttime,float temp,QString reason):QueueObject(roomid,windspeed,starttime,temp),
        reason(reason){}
    void changereason(QString reason){
        this->reason = reason;
    }
    WaitQueueObject(const WaitQueueObject& C)
       {
        this->reason = C.reason;
        this->roomid = C.roomid;
        this->starttime = C.starttime;
        this->roomid = C.roomid;
        this->windspeed = C.windspeed;

       }
};
class ServeQueueObject:public QueueObject{
public:
    ServeQueueObject(){}
    ServeQueueObject(int roomid,int windspeed,time_t starttime,float temp):QueueObject(roomid,windspeed,starttime,temp){}
     ServeQueueObject(const  ServeQueueObject& C)
       {
        this->roomid = C.roomid;
        this->starttime = C.starttime;
        this->roomid = C.roomid;
        this->windspeed = C.windspeed;

       }
};
#endif // SCHEDULE_H
