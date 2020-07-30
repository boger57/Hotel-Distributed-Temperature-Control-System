#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
struct RoomID//房间信息结构体
{
    int roomid;//房间号
    double settemp[20];//目标温度
    int setfanspeed[20];//设置风速
    int ltemp[20];//目标温度的持续时间,秒为单位
    int lwind[20];//设置风速的持续时间,秒为单位
    int settemp_num=0;//目标温度个数
    int setfanspeed_num=0;//设置风速个数
    //double Mtemp;//最常用目标温度
    //int setfanspeed;//最常用风速
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QDate key_date;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    RoomID RoomList[10];//房间结构体数组
    int roomid[10];//房间号数组
    int count_use[10];//使用空调次数
    int count_schdule[10];//被调度次数
    int count_settemp[10];//达到目标温度次数
    int count_list[10];//详单记录数
    double totalfee[10];//总费用
    QString search_room_id = "select distinct roomid from adjust";
    QString search_adjust = "select * from adjust";
    QString search_room = "select * from room";

    void search_data();
private slots:
    void on_calendarWidget_clicked(const QDate &date);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
