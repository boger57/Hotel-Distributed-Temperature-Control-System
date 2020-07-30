#include "window.h"
#include "ui_window.h"

void Window::TCP_init()
{
    qDebug()<<"初始化TCP";
    mainserver=new QTcpServer(this);
    connect(mainserver,&QTcpServer::newConnection,this,&Window::connection);
}

void Window::TCP_new()
{
    mainserver->listen(QHostAddress("10.128.228.93"),5555);//set ip and port
    //listen函数会对指定的地址和端口进行绑定，该绑定模式应该是ShareAddress
    qDebug()<<"开始TCP监听";
}

void Window::TCP_off()
{
    qDebug()<<"删除TCPSocket";
    mainserver->close();//关闭了
}

void Window::re_data() //接收消息并调用处理函数
{
    qDebug()<<"接收到一个包";

    QString received_data;
    for(QMap<int,QTcpSocket*>::iterator i=roomsocketmap.begin();i!=roomsocketmap.end();++i){  //遍历socketmap，处理其中有消息的端口
        if(i.value()->bytesAvailable()>0){  //如果有消息，bytesAvailable()函数返回当前已经获取的数据的大小
            qDebug()<<"消息来自已建立的链接,长度为："<<i.value()->bytesAvailable();
            QDataStream in(i.value());
            in.setVersion(QDataStream::Qt_5_9);// 设置数据流版本，这里要和客户端相同
            in >> blocksize;
            in >> received_data;
            qDebug()<<"接受的消息为"<<received_data;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(received_data.toLocal8Bit().data());
            //QJsonDocument既可以从一个基于文本表示的UTF-8编码，又可以从Qt自己的二进制格式读取和写入这个文件。
            QJsonObject subrespond = jsonDoc.object();
            //QJsonObject类用于封装JSON对象。
            //JSON对象是键值对，其中键是唯一的字符串，其值由QJsonValue代表。一个QJsonObject可以从QVariantMap转换/被转换。
            deal_data(subrespond);
        }
    }
    for(int i=unknownconnection.size()-1;i>=0;--i)
    {     //遍历未知房间的socket
        if(unknownconnection[i]->bytesAvailable()>0)//bytesAvailable()函数返回当前已经获取的数据的大小
        {
            qDebug()<<"消息来自新连接,标号为"<<i<<"   "<<unknownconnection[i]->bytesAvailable();
            int roomid;
            QDataStream in(unknownconnection[i]);
            in.setVersion(QDataStream::Qt_5_9);
            in >> blocksize;
            in >> received_data;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(received_data.toLocal8Bit().data());
            QJsonObject subrespond = jsonDoc.object();
            qDebug()<<subrespond;
            if(subrespond.contains("roomID")) //从消息中获取房间ID
            {
                roomid = subrespond.value("roomID").toVariant().toInt();
            }
            qDebug()<<"获取的房间ID为"<<roomid;
            if(roomid != -1)  //成功获取
            {
                if(roomsocketmap.contains(roomid)==false) //socketmap中该ID未有socket
                {
                    roomsocketmap.insert(roomid,unknownconnection[i]);  //将此socket从未知房间号list移至已知房间号list
                    unknownconnection.removeAt(i);
                }
            }
            else//未成功获取ID，直接断开链接
                {
                    unknownconnection[i]->disconnectFromHost();
                    unknownconnection.removeAt(i);
                }
            this->deal_data(subrespond);  //处理该报文
        }
    }
}


void Window::connection()
{
    qDebug()<<"接收到连接建立请求";
    unknownconnection.push_back(mainserver->nextPendingConnection());
    //push_back是STL中常见的方法，都是向数据结构中添加元素
    connect(unknownconnection.last(),&QTcpSocket::readyRead,this,&Window::re_data);
    connect(unknownconnection.last(),&QTcpSocket::disconnected,unknownconnection.last(),&QTcpSocket::deleteLater);
}
