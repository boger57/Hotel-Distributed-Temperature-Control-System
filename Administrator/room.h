#ifndef STRUCT_H
#define STRUCT_H


#include <QDateTime>
#include <QVector>
#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QMetaType>



class Submachine;  //房间信息
class MuItemDelegate;
Q_DECLARE_METATYPE(Submachine)



class Submachine{
public:
    int roomid;     //房间号
    double current_temp;  //当前温度
    double set_temp;    //设置温度
    int fan_speed;      //当前风速
    int set_speed;//目标风速
    double use_fee;    //使用费用
    QString now_model; //当前模式
    QDateTime fee_counttime;

    Submachine(){  //初始化
        current_temp=25;
        set_temp=25;
        fan_speed=0;
        set_speed=1;
        use_fee=0;
        now_model = "cold";
        roomid = 00001;
    }

    Submachine(int ct,int tt,int id){  //设置
        current_temp=25;
        set_temp=ct;
        fan_speed=0;
        set_speed=1;
        use_fee=0;
        roomid = id;
        if(tt == 1)
            now_model = "cold";
        else
            now_model = "hot";
        fee_counttime = QDateTime::currentDateTime();
    }

    double count_free(double freemodel)   //更新计算出费用，根据风速
    {
        //数据库操作，将当前时间段的费用写入
        QDateTime  now = QDateTime::currentDateTime();
        //与计费时间计算时间差
        double counttime = fee_counttime.secsTo(now);
        if(fan_speed == 1)//当前为低风
        {
            use_fee = (counttime/3)*freemodel;
            use_fee = use_fee/60.0;
        }
        else if(fan_speed == 2)//当前为中风
        {
            use_fee = (counttime/2)*freemodel;
            use_fee = use_fee/60.0;
        }
        else if(fan_speed == 3)//当前为高风
        {
            use_fee = counttime*freemodel;
            use_fee = use_fee/60.0;
        }
        else if(fan_speed == 0)//当前为无风，无费用
        {
            use_fee = 0.0;
        }
        return use_fee;
    }
};
class Display : public QStyledItemDelegate//
        //QStyledItemDelegate类为模型中的数据项提供显示和编辑功能。
{
public:
    Display(QObject *parent = nullptr);
    // painting
    void paint(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // STRUCT_H

