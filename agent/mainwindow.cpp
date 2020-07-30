#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <qDebug>
#include <iostream>
#include <fstream>
#include <locale>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    ui->pushButton->setEnabled(true);
    key_date = date;
}


void MainWindow::on_pushButton_clicked()//查询报表
{
    ui->tableWidget->clear();
    ui->pushButton_2->setEnabled(true);
    ui->tableWidget->setColumnCount(8);
    QStringList header;
    header<<"房间号 "<<"使用次数 "<<"最常用温度 "<<"最常用风速 "<<"达到温度次数 "<<"被调度次数 "<<"详单记录数 "<<"总费用 ";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->resizeColumnsToContents();
    search_data();
 }

int calculate_diff(QVariant begin_time,QVariant end_time)//求时间差
{
    return (begin_time.toDateTime()).secsTo(end_time.toDateTime());
}

void MainWindow::search_data(){

    QSqlDatabase mydatabase;
    //QSqlDatabase创建连接数据库实例,一个QSqlDatabase的实例代表了一个数据库连接。
    if(QSqlDatabase::contains("/Users/boger/Desktop/314b/linkdata.db"))
    //判断是否已经创建数据库文件
    //连接QSQLITE类型的数据库linkdata.db
    {
        mydatabase = QSqlDatabase::database("/Users/boger/Desktop/314b/linkdata.db");
    }
    else
    {
        mydatabase=QSqlDatabase::addDatabase("QSQLITE","wconntosqlite1");
        mydatabase.setDatabaseName("/Users/boger/Desktop/314b/linkdata.db");//database name

    }

    if(mydatabase.open())
    {

        int roomid_num = 0;
        int roomlist_num = 0;

        QSqlQuery query(mydatabase);
        if(!query.exec(search_room_id)) {
            qDebug()<<query.lastError();
        }
        else {
            int roww = 0;
            query.last();
            roww = query.at() + 1;
            ui->tableWidget->setRowCount(roww);
        }

        if(!query.exec(search_adjust)) {
            qDebug()<<query.lastError();
        }
        else {
            while(query.next()) {
                if(key_date == query.value(2).toDateTime().date()) {
                    int i,flag = 0;
                    for(i = 0;i < roomlist_num;i++) {
                        if(query.value(0) == RoomList[i].roomid) {
                            flag=1;
                            break;
                        }
                    }
                    if(flag == 1) {
                        totalfee[i] += query.value(4).toDouble();
                        count_use[i] += query.value(11).toInt();
                        count_settemp[i] += query.value(12).toInt();
                        count_list[i] += query.value(13).toInt();

                        int ii,mark1 = 0,mark2 = 0;
                        for(ii = 0;ii < RoomList[i].settemp_num;ii++) {
                            if(query.value(8) == RoomList[i].settemp[ii]) {
                                mark1=1;
                                break;
                            }
                        }
                        if(mark1 == 1) {
                            RoomList[i].ltemp[ii]+=calculate_diff(query.value(2),query.value(3));
                        }
                        else {
                            RoomList[i].settemp[RoomList[i].settemp_num]=query.value(8).toDouble();
                            RoomList[i].ltemp[RoomList[i].settemp_num]=calculate_diff(query.value(2),query.value(3));
                            RoomList[i].settemp_num++;
                        }

                        for(int ii = 0;ii < RoomList[i].setfanspeed_num;ii++) {
                            if(query.value(10) == RoomList[i].setfanspeed[ii]) {
                                mark2=1;
                                break;
                            }
                        }
                        if(mark2 == 1) {
                            RoomList[i].lwind[ii]+=calculate_diff(query.value(2),query.value(3));
                        }
                        else {
                            RoomList[i].setfanspeed[RoomList[i].setfanspeed_num]=query.value(10).toInt();
                            RoomList[i].lwind[RoomList[i].setfanspeed_num]=calculate_diff(query.value(2),query.value(3));
                            RoomList[i].setfanspeed_num++;
                        }
                    }
                    else {
                        roomid[roomlist_num] = query.value(0).toInt();
                        totalfee[roomlist_num] = query.value(4).toDouble();
                        count_use[roomlist_num] = query.value(11).toInt();
                        count_settemp[roomlist_num] = query.value(12).toInt();
                        count_list[roomlist_num] = query.value(13).toInt();

                        RoomList[roomlist_num].roomid = query.value(0).toInt();
                        RoomList[roomlist_num].settemp[0] = query.value(8).toDouble();
                        RoomList[roomlist_num].setfanspeed[0] = query.value(10).toInt();
                        RoomList[roomlist_num].ltemp[0] = calculate_diff(query.value(2),query.value(3));
                        RoomList[roomlist_num].lwind[0] = calculate_diff(query.value(2),query.value(3));
                        RoomList[roomlist_num].settemp_num++;
                        RoomList[roomlist_num].setfanspeed_num++;
                        roomlist_num++;
                    }
                }
            }

            for(int j = 0;j < roomlist_num;j++) {
                int no1 = 0,no2 = 0;
                double max1 = RoomList[j].ltemp[0];
                double max2 = RoomList[j].lwind[0];
                for(int k1 = 1;k1<RoomList[j].settemp_num;k1++) {
                    if(RoomList[j].ltemp[k1]>max1) {
                        max1 = RoomList[j].ltemp[k1];
                        no1 = k1;
                    }
                }
                for(int k2 = 1;k2 < RoomList[j].setfanspeed_num;k2++) {
                    if(RoomList[j].lwind[k2]>max2) {
                        max2 = RoomList[j].lwind[k2];
                        no2 = k2;
                    }
                }
                QString Mtemp = QString::number(RoomList[j].settemp[no1],'f',1);//最常用目标温度
                QString Mwind = QString::number(RoomList[j].setfanspeed[no2],10);//最常用风速
                ui->tableWidget->setItem(j,2,new QTableWidgetItem(Mtemp));
                ui->tableWidget->setItem(j,3,new QTableWidgetItem(Mwind));
                ui->tableWidget->setItem(j,0,new QTableWidgetItem(QString::number(roomid[j])));
                ui->tableWidget->setItem(j,1,new QTableWidgetItem(QString::number(count_use[j])));
                ui->tableWidget->setItem(j,4,new QTableWidgetItem(QString::number(count_settemp[j])));
                ui->tableWidget->setItem(j,6,new QTableWidgetItem(QString::number(count_list[j])));
                ui->tableWidget->setItem(j,7,new QTableWidgetItem(QString::number(totalfee[j], 'g', 2)));
            }
        }

        if(!query.exec(search_room)) {
            qDebug()<<query.lastError();
        }
        else {
            while(query.next()) {
                int i,symbol=0;
                for(i = 0;i < roomid_num;i++) {
                    if(query.value(0) == roomid[i]) {
                        symbol = 1;
                        break;
                    }
                }
                if(symbol == 1) {
                    count_schdule[i] = query.value(9).toInt();
                }
                else {
                    count_schdule[roomid_num] = query.value(9).toInt();
                    roomid_num++;
                }
            }

            for(int j = 0;j < roomlist_num;j++) {
                ui->tableWidget->setItem(j,5,new QTableWidgetItem(QString::number(count_schdule[j])));
            }
        }
    }

}

void MainWindow::on_pushButton_2_clicked()//打印报表
{
    ofstream fileout("report.txt",ios::trunc);
    if(fileout.is_open())
    {
        qDebug()<<"row count:"<<ui->tableWidget->rowCount()<<"column count:"<<ui->tableWidget->columnCount();
        fileout<<"房间号"<<"\t"<<"使用空调次数"<<"\t"<<"最常用目标温度"<<"\t"<<"最常用风速"<<"\t"<<
              "达到目标温度次数"<<"\t"<<"被调度次数"<<"\t"<<"详单记录数"<<"\t"<<"总费用"<<"\t"<<"\n";
        qDebug("saveTable start.");
        for(int i = 0; i < ui->tableWidget->rowCount();i++) {
            for(int j = 0;j < ui->tableWidget->columnCount();j++) {
                QString text;
                if(ui->tableWidget->item(i, j))
                    text = ui->tableWidget->item(i, j)->text();
                else text = "";
                if(text.isEmpty()) {
                    fileout<<"  "<<"\t";
                }
                else {
                    string tex = text.toStdString();
                    fileout<<"\t"<<tex<<"\t";
                }
            }
            fileout<<"\n";
        }
        fileout.flush();
        fileout.close();
    }
}
