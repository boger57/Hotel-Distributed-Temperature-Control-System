#include "window.h"
#include "ui_window.h"


void  Window::subw_login(int roomid)//从控机登录请求响应
{
    qDebug()<<"进入登录请求响应函数";
    QJsonObject  message;
    if(dataupload->sub_login(roomid)) //在数据库中未找到该房间并修改状态为登录，计费置0
    {
        message.insert("Action","Login_S");
        message.insert("roomID",roomid);
    }
    else //该房间已登陆过,登陆失败
    {
        message.insert("Action","Login_F");
        message.insert("roomID",roomid);
    }
    qDebug()<<"回复登录请求的报文"<<message;
    this->send_data(message);
}

void  Window::subw_quit(int roomid)//从控机退房请求响应
{
    qDebug()<<"进入退房请求响应函数";
    QMap<int,Submachine>::iterator ptr;
    double sum_fee;
    if((ptr=submacinfo.find(roomid))!=submacinfo.end())//如果该从控机未关机，在列表中可以找到
    {
        if(submacinfo.value(roomid).fan_speed != Nowind) //如果未停风
        {
            sum_fee = ptr.value().count_free(fee_coe); //计算上次计费时间到当前的费用
            ptr.value().fan_speed = Nowind;  //修改清单中对应值
            dataupload->sub_quit(roomid);//数据库修改状态为退房
            dataupload->adjust_off(roomid,submacinfo.value(roomid)); //写关机报表
            submacinfo.remove(roomid);  //从房间列表和所有队列中移除
            serve_queue.remove(roomid);
        }
        else
        {
            dataupload->adjust_off(roomid,submacinfo.value(roomid)); //写关机报表
            submacinfo.remove(roomid);  //从房间列表和所有队列中移除
            if(waiting_queue.contains(roomid))
            {
                waiting_queue.remove(roomid);//如果在等待队列中则移除
            }
            dataupload->sub_quit(roomid);//数据库状态修改为退房
        }
    }
    else //该从控机已关机，从列表中已移除
    {
        dataupload->sub_quit(roomid);//数据库修改状态为退房
    }
    qDebug()<<"房间队列长度"<<submacinfo.size();
    //从数据库中读取该房间累计总费用发送，然后费用清零
    sum_fee = dataupload->sub_totalfee(roomid); //从数据库获取处理后最终的总花费
    QJsonObject message;
    message.insert("Action","Checkout_S");
    message.insert("roomID",roomid);
    message.insert("money",sum_fee);
    qDebug()<<"回复退房请求的报文"<<message;
    this->send_data(message);

    QMap<int,QTcpSocket*>::iterator i;
    if((i=roomsocketmap.find(roomid)) !=roomsocketmap.end()) //在Socket列表中找到该房间对应的Socket
    {
        i.value()->disconnectFromHost(); //断开其连接
        roomsocketmap.remove(roomid);
    }
    qDebug()<<"roomsocketmap的长度"<<roomsocketmap.size();
}

void  Window::subw_start(int roomid ) //从控机启动请求响应
{
    qDebug()<<"进入开机请求响应函数";
    QMap<int,Submachine>::iterator ptr;
    if((ptr=submacinfo.find(roomid))==submacinfo.end())
    {  //房间列表中未找到该房间
       submacinfo.insert(roomid,Submachine(default_temp,airc_mode,roomid));  //该房间号对应从控机启动，设置为初始化
       dataupload->sub_turnon(roomid); //修改数据库为开机状态
       dataupload->sub_ctemp(roomid,default_temp); //重新开机后为中央空调初始化的状态，修改缺省温度
       dataupload->sub_cmode(roomid,airc_mode); //默认模式为中央空调设置模式
       dataupload->sub_cwind_waitqueue(roomid,1); //请求风速为默认1档
       dataupload->adjust_on(roomid,submacinfo.value(roomid)); //写开机报表
       /***********************
        * 开机时从控机未给当前温度？
        **********************/
    }
    QJsonObject message;
    message.insert("Action","Turnon_S");
    message.insert("roomID",roomid);
    message.insert("windspeed",1);
    if(airc_mode == 1)
        message.insert("mode","cold");
    else
        message.insert("mode","hot");
    message.insert("starttemp",default_temp);
    qDebug()<<"工作中从控机列表长度"<<submacinfo.size();
    qDebug()<<"回复开机请求的报文"<<message;
    this->send_data(message);
}

void  Window::subw_stop(int roomid)  //从控机停止请求响应
{
    qDebug()<<"进入关机请求响应函数";
    double get_fee;
    if(submacinfo.value(roomid).fan_speed != Nowind) //如果该房间未停风
    {
        QMap<int,Submachine>::iterator ptr;
        if((ptr=submacinfo.find(roomid))!=submacinfo.end())
        {  //找到该房间
            get_fee = ptr.value().count_free(fee_coe);//计算这段期间费用
            ptr.value().fan_speed = Nowind;  //修改清单中对应值
            dataupload->sub_turnoff(roomid);//数据库修改为关机状态
            dataupload->adjust_off(roomid,submacinfo.value(roomid)); //写关机报表
            submacinfo.remove(roomid);  //从房间列表和所有队列中移除
            serve_queue.remove(roomid);
        }
    }
    else //该房间已停风
    {
        QMap<int,Submachine>::iterator ptr;
        if((ptr=submacinfo.find(roomid))!=submacinfo.end())
        {  //找到该房间
            get_fee = ptr.value().count_free(fee_coe);//计算这段期间费用
            dataupload->sub_turnoff(roomid);//数据库修改为关机状态
            dataupload->adjust_off(roomid,submacinfo.value(roomid)); //写关机报表
            submacinfo.remove(roomid);  //从房间列表和所有队列中移除
            if(waiting_queue.contains(roomid))
            {
                waiting_queue.remove(roomid);//如果在等待队列中则移除
            }
        }
    }
    QJsonObject message;
    message.insert("Action","Turnoff_S");
    message.insert("roomID",roomid);
    qDebug()<<"回复关机请求的报文"<<message;
    this->send_data(message);
}

void  Window::sub_wind_waitqueue(int roomid,int windspeed) //等待队列从控机更改风速请求响应
{
    qDebug()<<"进入等待队列更改风速请求响应函数";
    QMap<int,Submachine>::iterator ptr;
    if((ptr=submacinfo.find(roomid))!=submacinfo.end()){  //在列表中找到该房间
        ptr.value().set_speed=windspeed;  //修改清单中目标风速对应值
        dataupload->sub_cwind_waitqueue(roomid,windspeed);//修改数据库状态，仅修改目标风速
        dataupload->adjust_others(roomid,submacinfo.value(roomid)); //写操作报表
    }
    QJsonObject message;
    message.insert("Action","Changewind_S");
    message.insert("roomID",roomid);
   message.insert("requiredwindspeed",windspeed);
    qDebug()<<"回复等待队列更改风速请求的报文"<<message;
    this->send_data(message);
}

void  Window::sub_wind_servequeue(int roomid,int windspeed) //服务队列从控机更改风速请求响应
{
    qDebug()<<"进入服务队列更改风速请求响应函数";
    double get_fee;
    QMap<int,Submachine>::iterator ptr;
    if((ptr=submacinfo.find(roomid))!=submacinfo.end()){  //在列表中找到该房间
        get_fee = ptr.value().count_free(fee_coe); //计算期间费用
        ptr.value().fee_counttime = QDateTime::currentDateTime(); //更新计费时间段
        ptr.value().set_speed=windspeed;  //修改清单中目标风速对应值
        ptr.value().fan_speed=windspeed;//修改清单中当前风速
        dataupload->sub_cwind_servequeue(roomid,windspeed); //修改数据库状态，修改当前和目标风速
        dataupload->adjust_others(roomid,submacinfo.value(roomid)); //写操作报表
    }
    QJsonObject message;
    message.insert("Action","Changewind_S");
    message.insert("roomID",roomid);
    message.insert("requiredwindspeed",windspeed);
    qDebug()<<"回复服务队列更改风速请求的报文"<<message;
    this->send_data(message);
}

void  Window::subw_temp(int roomid,double targettemp) //从控机更改温度请求响应
{
    qDebug()<<"进入更改温度请求响应函数";
    double get_fee;
    QMap<int,Submachine>::iterator ptr;
    if((ptr=submacinfo.find(roomid))!=submacinfo.end()){  //找到该房间
        get_fee = ptr.value().count_free(fee_coe); //计算期间费用
        ptr.value().fee_counttime = QDateTime::currentDateTime(); //更新计费时间段
        ptr.value().set_temp=targettemp;  //修改清单中对应值
        dataupload->sub_ctemp(roomid,targettemp);//数据库修改目标温度
        dataupload->adjust_others(roomid,submacinfo.value(roomid)); //写操作报表
    }
    QJsonObject message;
    message.insert("Action","Changetemp_S");
    message.insert("roomID",roomid);
    message.insert("settem",targettemp);
    qDebug()<<"回复更改温度请求的报文"<<message;
    this->send_data(message);
}

void  Window::subw_mode(int roomid,QString model)//从控机更改模式请求响应
{
    double get_fee;
    qDebug()<<"进入更改模式响应函数,请求model"<<model;
    int modelint;
    if(model.compare("cold") == 0)
        modelint = managerCold;
    else if(model.compare("hot") == 0)
        modelint = managerHeat;

    QMap<int,Submachine>::iterator ptr;
    if((ptr=submacinfo.find(roomid))!=submacinfo.end()){  //找到该房间
        get_fee = ptr.value().count_free(fee_coe); //计算期间费用
        ptr.value().fee_counttime = QDateTime::currentDateTime(); //更新计费时间段
        ptr.value().now_model=model;  //修改清单中对应值
        dataupload->sub_cmode(roomid,modelint);//数据库修改工作模式
        dataupload->adjust_others(roomid,submacinfo.value(roomid)); //写操作报表
    }
    QJsonObject message;
    message.insert("Action","Changemode_S");
    message.insert("roomID",roomid);
    qDebug()<<"回复更改模式请求的报文"<<message;
    this->send_data(message);
}

void  Window::sub_windstart(int roomid,int windspeed) //从控机送风请求响应
{
    qDebug()<<"进入送风请求响应函数,请求model";
    double get_fee;
    QMap<int,Submachine>::iterator ptr;
    if((ptr=submacinfo.find(roomid))!=submacinfo.end()){  //找到该房间
        get_fee = ptr.value().count_free(fee_coe); //计算期间费用
        ptr.value().fee_counttime = QDateTime::currentDateTime();//计费时间更新
        ptr.value().fan_speed=windspeed;  //修改清单中对应值
        dataupload->sub_startwind(roomid,windspeed);//数据库修改
        dataupload->sub_scheduletimes(roomid,1); //被调度次数+1
        dataupload->adjust_others(roomid,submacinfo.value(roomid)); //写操作报表
    }
    QJsonObject message;
    message.insert("Action","Startwind_S");
    message.insert("roomID",roomid);
    qDebug()<<"回复送风请求请求的报文"<<message;
    this->send_data(message);
}

void  Window::sub_windstop(int roomid) //从控机停风请求响应，从控机到达目标温度时发出
{
    qDebug()<<"进入停风请求响应函数,请求model";
    QMap<int,Submachine>::iterator ptr;
    if((ptr=submacinfo.find(roomid)) != submacinfo.end()){  //找到该房间
        double countfree = ptr.value().count_free(fee_coe);//计算期间费用
        ptr.value().fee_counttime = QDateTime::currentDateTime();//计费时间更新
        ptr.value().fan_speed = Nowind;  //修改清单中对应值
        //ptr.value().currentT=ptr.value().targetT;
        //em!!!!!!!!!!!!!!
        //上面条语句出霰了很隐藏的错误，朱昊等人在六月二十日晚及时发现了，先注释掉……
        dataupload->sub_stopwind(roomid);//数据库修改停风状态
        dataupload->adjust_others(roomid,submacinfo.value(roomid)); //写操作报表
    }
    QJsonObject message;
    message.insert("Action","Stopwind_S");
    message.insert("roomID",roomid);
    qDebug()<<"回复送风请求请求的报文"<<message;
    this->send_data(message);
}

void  Window::sub_updatetemp(int roomid,double roomtemp) //从控机当前状态更新报文响应
{
    qDebug()<<"进入状态更新响应函数";
    QMap<int,Submachine>::iterator ptr;
    if((ptr=submacinfo.find(roomid))!=submacinfo.end()){  //找到该房间
        ptr.value().current_temp=roomtemp;  //修改清单中对应值
        dataupload->sub_ctemp2(roomid,roomtemp);//数据库修改
    }
    QJsonObject message;
    message.insert("Action","Sendtemp_S");
    Dbcontrol *datacontrollor=new Dbcontrol(this);

    double cost = datacontrollor->sub_totalfee(roomid);
    message.insert("totalfee",cost);
    message.insert("roomID",roomid);
    qDebug()<<message;

    this->send_data(message);
}

void Window::sub_towait(int roomid) //通知从控机已被移出服务队列
{
    qDebug()<<"进入通知移除服务队列响应函数";
    QMap<int,Submachine>::iterator ptr;
    if((ptr=submacinfo.find(roomid))!=submacinfo.end()){  //找到该房间
        double countfree = ptr.value().count_free(fee_coe);//计算费用
        ptr.value().fee_counttime = QDateTime::currentDateTime();//计费时间更新
        ptr.value().fan_speed = Nowind;  //修改清单中对应值
        dataupload->sub_stopwind(roomid);//数据库修改停风
        dataupload->adjust_others(roomid,submacinfo.value(roomid)); //写操作报表
    }
    QJsonObject message;
    message.insert("Action","Stopwind_S");
    message.insert("roomID",roomid);
    qDebug()<<"回复移除服务队列的报文"<<message;
    this->send_data(message);
}

void Window::sub_toserve(int roomid,int windspeed) //通知从控机已进入服务队列
{
    qDebug()<<"进入通知进入服务队列响应函数";
    QMap<int,Submachine>::iterator ptr;
    if((ptr=submacinfo.find(roomid))!=submacinfo.end()){  //找到该房间
        double countfree = ptr.value().count_free(fee_coe);//计算期间费用
        ptr.value().fee_counttime = QDateTime::currentDateTime();//计费时间更新
        ptr.value().fan_speed = Nowind;  //修改清单中对应值
        dataupload->sub_startwind(roomid,windspeed);//数据库修改
        dataupload->adjust_others(roomid,submacinfo.value(roomid)); //写操作报表
    }
    QJsonObject message;
    message.insert("Action","Startwind_S");
    message.insert("roomID",roomid);
    qDebug()<<"回复进入服务队列的报文"<<message;
    this->send_data(message);
}

void Window::send_data(QJsonObject info)//发送数据一个JSON数据
{
    int roomid = info.value("roomID").toVariant().toInt();

    QJsonDocument rectJsonDoc;
    rectJsonDoc.setObject(info);
    QString request(rectJsonDoc.toJson(QJsonDocument::Compact));


    QByteArray block;   //用于暂存要发送的数据
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_5_6);    //设置数据流的版本，客户端和服务器端使用的版本要相同
    out << (quint16)0;  //预留两字节
    out << request;

    out.device()->seek(0);  //  跳转到数据块的开头
    out << (quint16)(block.size() - sizeof(quint16));   //填写大小信息

    qDebug()<<"sendData函数执行";
    if(roomsocketmap.contains(roomid)){
        roomsocketmap[roomid]->write(block);
        roomsocketmap[roomid]->flush();
    }
}

