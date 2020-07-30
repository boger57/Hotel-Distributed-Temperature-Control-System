#include "window.h"

int Window::get_servespeed(int askspeed){
    qDebug()<<"findservespeed";
    qDebug()<<askspeed<<"askspeed";
    int loserspeed = 10;
    long long losertime = 100000000;
    int findid = -2;
    int findsmall = 0;
    QMap<int,ServeQueueObject>::Iterator it = serve_queue.begin();
    while(it!=serve_queue.end()){
        ServeQueueObject item = it.value();
        int roomid = it.key();
//        QJsonObject protocol = get<0>(item);
        time_t waittime = item.starttime;
        Submachine curroom = submacinfo[roomid];
        qDebug()<< "curroom.speed"<<curroom.fan_speed;
        qDebug()<< "askspeed"<<askspeed;
        if (curroom.fan_speed < askspeed){ //判断如果风速小于当前的风速，则替换
            if (loserspeed > curroom.fan_speed){
                findsmall = 1;
                loserspeed = curroom.fan_speed;
                findid = roomid;
            }
        }
        if (findsmall == 0){//如果风速相等，比较服务时间，服务时间越小则可替换
            if ((curroom.fan_speed == askspeed)){
                    findid = -1;
            }
        }
        it++;
    }
    return findid;
   //返回-2说明没有比他小的，返回-1说明只有相等的，否则返回房间号
}
int Window::get_time(QString str,int windspeed){ //找到等待/服务开始时间最靠前的并返回place
    long long earlytime = 100000000000;
    int roomid = -1;
    if (str == "wait"){
        QMap<int,WaitQueueObject>::Iterator it = waiting_queue.begin();
        while(it != waiting_queue.end()){
            WaitQueueObject tp = it.value();
            time_t starttime  = tp.starttime;
            if (earlytime > starttime){
                earlytime = starttime;
                roomid = it.key();
            }
            it++;
         }
    }
    else {
        if (windspeed != -2){
            QMap<int,ServeQueueObject>::Iterator it = serve_queue.begin();
            while(it != serve_queue.end()){
                ServeQueueObject tp = it.value();
                time_t starttime  = tp.starttime;
                if (earlytime > starttime & windspeed == tp.windspeed){
                    earlytime = starttime;
                    roomid = it.key();
                }
                it++;
             }
        }
        else{
            QMap<int,ServeQueueObject>::Iterator it = serve_queue.begin();
            while(it != serve_queue.end()){
                ServeQueueObject tp = it.value();
                time_t starttime  = tp.starttime;
                if (earlytime > starttime){
                    earlytime = starttime;
                    roomid = it.key();
                }
                it++;
             }
        }

   }
    return roomid;
}

/*10s触发一次
 1.判断serve队列是否小于3,小于3进行调度，调度时选取开始等待时间最前的请求
 2.判断key为"equality"的请求是否到达2分钟，到达则将开始服务时间最前的请求挤下来
*/
void Window::wait_queue(){//放在定时器中
    qDebug()<<"定时检查队列";
    qDebug()<<"serve_queue"<<serve_queue.size();
    QMap<int,ServeQueueObject>::Iterator it2 = serve_queue.begin();
    while(it2 != serve_queue.end()){
         qDebug()<<"在服务队列中的房间"<<it2.key();
        it2++;
    }
    qDebug()<<"waiting_queue"<<waiting_queue.size();
    QMap<int,WaitQueueObject>::Iterator it1 = waiting_queue.begin();
    while(it1 != waiting_queue.end()){
        qDebug()<<"在等待队列中的房间"<<it1.key();
        it1++;
    }
    if (serve_queue.size() < queuelen){
        qDebug()<<"计入定时器，且队列少于2，不需要调度";
        if(waiting_queue.size()>0){
            int roomid = this->get_time("wait",-1);
            qDebug()<<"roomid:"<<roomid;
            qDebug()<<"waiting队列里有";
            WaitQueueObject fromwait = waiting_queue[roomid];
//            QJsonObject protocol = get<1>(tpfromwait);  //协议内容
            QString reason = fromwait.reason;
            waiting_queue.remove(roomid);
            time_t servetime = time(NULL);
            int windspeed = fromwait.windspeed;
            ServeQueueObject toserve(fromwait.roomid,fromwait.windspeed,fromwait.starttime,fromwait.temp);
            serve_queue.insert(roomid,toserve);
            this->sub_windstart(roomid,windspeed);

        }

    }
    QMap<int, WaitQueueObject>::Iterator it = waiting_queue.begin();
    QMap<int,WaitQueueObject>::Iterator preit;
    while(it != waiting_queue.end()){
        qDebug()<<"遍历等待队列，找2分钟相等";
        time_t curtime =  time(NULL);
        WaitQueueObject tp = it.value();
        QString reason = tp.reason;
        time_t starttime  = tp.starttime;
        int windspeed = tp.windspeed;
        int roomid = tp.roomid;
        int findid;
        qDebug()<<reason;
        qDebug()<<(curtime - starttime);
        if (((reason == "equality") && (curtime - starttime) > 10)||((reason == "small") && (curtime - starttime) > 30)){
            qDebug()<<"进入相等";//如果找到满足条件的，将他它从waiting队列中取出，替换服务队列服务时间最长的
            if ((reason == "equality") && (curtime - starttime) > 10){
                findid = this->get_time("serve",windspeed);
            }
            else{
                findid = this->get_time("serve",-2);
            }
            if (findid != -1){
                ServeQueueObject fromserve = serve_queue[findid];
                time_t waittime = time(NULL);
                QString reason = "iskicked";
                WaitQueueObject towait(fromserve.roomid,fromserve.windspeed,waittime,fromserve.temp,reason);
                waiting_queue.insert(findid,towait);
                serve_queue.remove(findid);
                this->sub_towait(findid);
                preit = it;
                it++;
                waiting_queue.erase(preit);
                time_t servetime = time(NULL);
                ServeQueueObject toserve(roomid,windspeed,starttime,submacinfo[roomid].set_temp);
                serve_queue.insert(roomid,toserve);
                this->sub_windstart(roomid,windspeed);//处理新请求
                qDebug()<<"还是好的";
            }
            else{//没有相等的
                 QString reasontowait = "small";
                 waiting_queue[roomid].reason = reasontowait;
                 it++;
            }
        }
        else{
            it++;
        }

    }
}
/*
如果服务队列大于3,则新请求需要判断当前等待队列中是否有该房间号的等待请求，
如果发送的是请求送风，（认为请求送风只可能有一个），则根据优先级调度
如果发送的是更改风速，如果请求送风在等待队列，直接替换，如果在服务队列,则根据优先级修改
*/
void Window::deal_data(QJsonObject subrespond)
{
    int windspeed;
    double roomtemp;
    double targettemp;
    QString targetmodel;
    int roomid = subrespond.value("roomID").toVariant().toInt();
    if(subrespond["Action"].toString()=="Login")
    {
        this->subw_login(roomid);
    }
    else if(subrespond["Action"].toString()=="Turnon")
    {
        this->subw_start(roomid);
    }
    else if(subrespond["Action"].toString()=="Turnoff")
    {
        this->subw_stop(roomid);
    }
    else if(subrespond["Action"].toString()=="Changewind")
    {
        qDebug()<<"接收到一个更改风速请求";
        windspeed = subrespond.value("requiredwindspeed").toVariant().toInt();
        if(serve_queue.contains(roomid)){//如果该房间在服务队列中服务
            serve_queue[roomid].windspeed = windspeed;
            time_t servetime = time(nullptr);
            serve_queue[roomid].starttime = servetime;
            this->sub_wind_servequeue(roomid,windspeed);
        }
        else if (waiting_queue.contains(roomid)){
            QString reason;
            int findid = this->get_servespeed(windspeed);
            if(findid == -2 || findid == -1){
                if(findid == -2)reason = "small";
                else reason = "equality";
                waiting_queue[roomid].reason = reason;
                waiting_queue[roomid].windspeed = windspeed;
                time_t waittime = time(NULL);
                waiting_queue[roomid].starttime = waittime;
                waiting_queue[roomid].roomid = roomid;
                waiting_queue[roomid].temp = submacinfo[roomid].set_temp;
                this->sub_wind_waitqueue(roomid,windspeed);
            }
            else {
                reason = "iskicked";
                ServeQueueObject qfromserve = serve_queue[findid];
//                QJsonObject protocol = get<0>(tpfromserve);
                time_t waittime = time(NULL);
                WaitQueueObject towait(qfromserve.roomid,qfromserve.windspeed,waittime,qfromserve.temp,reason);
//                tuple<QString,QJsonObject,time_t> tptowait = make_tuple(reason,protocol,waittime);
                waiting_queue.insert(towait.roomid,towait);
                this->sub_towait(findid);
                serve_queue.remove(findid);

                time_t servetime = time(NULL);
                ServeQueueObject toserve(roomid,windspeed,servetime,submacinfo[roomid].set_temp);
                serve_queue.insert(roomid,toserve);
                this->sub_windstart(roomid,windspeed);
            }

        }
    }
    else if(subrespond["Action"].toString()=="Stopwind")
    {
        qDebug()<<"接收到一个停风请求";
        //QString reason = "";
        serve_queue.remove(roomid);//从服务队列里移除
        this->sub_windstop(roomid);
    }
    else if(subrespond["Action"].toString()=="Startwind"){
        qDebug()<<"接收到一个送风请求";
        windspeed = subrespond.value("requiredwindspeed").toVariant().toInt();
        if(!serve_queue.contains(roomid)){
            if(serve_queue.size() < queuelen){//如果队列小于3，可以直接进入服务队列
                qDebug()<<"队列小于1，直接进入服务队列";
                time_t servetime = time(NULL);
                ServeQueueObject toserve(roomid,windspeed,servetime,submacinfo[roomid].set_temp);
                serve_queue.insert(roomid,toserve);
                this->sub_windstart(roomid,windspeed);
            }
            else{

                int findid = this->get_servespeed(windspeed);
                if (findid == -2){//没有比它风速小的,进入等待队列，无限期等待
                    QString reason = "small";
                    qDebug()<<"没有比他小的";
                    time_t waittime = time(NULL);
                    if(waiting_queue.contains(roomid)){
                        waittime = waiting_queue[roomid].starttime;
                    }
                    WaitQueueObject towait(roomid,windspeed,waittime,submacinfo[roomid].set_temp,reason);
                    waiting_queue.insert(roomid,towait);
                }
                else if (findid == -1){//有和它风速相等的，进入等待队列，等待两分钟
                    QString reason = "equality";
                    qDebug()<<"有和他一样的";
                    time_t waittime = time(NULL);
                    if(waiting_queue.contains(roomid)){
                        waittime = waiting_queue[roomid].starttime;
                    }
                    WaitQueueObject towait(roomid,windspeed,waittime,submacinfo[roomid].set_temp,reason);
                    waiting_queue.insert(roomid,towait);
                }
                else{//有比它小的,从服务队列中取出，计费，放入等待队列，将它放入服务队列
                    QString reason = "iskicked";
                    qDebug()<<"接收到一个停风请求";
                    qDebug()<<"有比他小的";

                    ServeQueueObject fromserve = serve_queue[findid];
//                    QJsonObject protocol = get<0>(tpfromserve);
                    time_t waittime = time(NULL);
                    WaitQueueObject towait(fromserve.roomid,fromserve.windspeed,waittime,fromserve.temp,reason);
                    waiting_queue.insert(towait.roomid,towait);
                    this->sub_towait(findid);
    //                serve_queue.remove(findid);

                    time_t servetime = time(NULL);
                    ServeQueueObject toserve(roomid,windspeed,servetime,submacinfo[roomid].set_temp);
                    serve_queue.insert(roomid,toserve);
                    serve_queue.remove(findid);
                    this->sub_windstart(roomid,windspeed);
                    if(waiting_queue.contains(roomid)){
                        waiting_queue.remove(roomid);
                    }
                    }
                }
        }
    }  //调度

    else if(subrespond["Action"].toString()=="Changemode")
    {
        targetmodel = subrespond.value("mode").toVariant().toString();
        this->subw_mode(roomid,targetmodel);
    }
    else if(subrespond["Action"].toString()=="Changetemp")
    {
        targettemp = subrespond.value("requiredtemp").toVariant().toDouble();
        this->subw_temp(roomid,targettemp);
    }
    else if(subrespond["Action"].toString()=="Sendtemp")
    {
        roomtemp = subrespond.value("currenttemp").toVariant().toDouble();
        this->sub_updatetemp(roomid,roomtemp);
    }
    else if(subrespond["Action"].toString()=="Checkout")
    {
        this->subw_quit(roomid);
    }

}

