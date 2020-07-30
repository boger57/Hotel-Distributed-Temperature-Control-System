#include "window.h"
#include "ui_window.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);//主界面建立

    /***********************************/
    /*主控机属性初始化*/
    this->airc_state = managerON; //关闭
    ui->pushButton_3->setEnabled(false); //OFF不可用
    this->airc_mode = managerUnset;//工作模式未设置
    this->default_temp = managerUnset;//缺省温度未设置
    this->fee_coe = managerUnset;//计费模式未设置
    //房间列表UI初始化
    QTimer *submactimer = new QTimer(this);
    connect(submactimer, SIGNAL(timeout()), this, SLOT(update_subui()));
    submactimer->start(2000);
    /***********************************/
    /*初始化网络连接*/
    this->TCP_init();

    /***********************************/
    /*调度策略*/
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(wait_queue()));
    timer->start(2000);

    /***********************************/
    /*初始化数据库模块*/
    dataupload=new Dbcontrol(this);
}

Window::~Window()
{
    delete ui;
}

void Window::host_off() //将中央空调改为关闭状态，将所有初始化设定清空
{
    //工作队列为空则关闭
    //不为空则不做处理
    this->airc_mode = managerUnset;//工作模式未设置
    this->default_temp = managerUnset;//温度未设置
    this->fee_coe = managerUnset;//计费模式未设置
    ui->label_6->setText("Unset");
    ui->label_2->setText("Unset");
    ui->label_4->setText("Unset");
}

bool Window::init_judge() //开机初始设定的合法性判断
{
    //合法则返回true
    //不合法返回false，并弹出弹窗提示
    if(airc_mode == managerUnset)
        return false;
    if(default_temp == managerUnset )
        return false;
    if(fee_coe == managerUnset)
        return false;
    return true;
}

void Window::on_pushButton_clicked()    //ON按钮
{
    qDebug()<<"进入开机函数";
    airc_mode = dataupload->host_initmode();
    default_temp = dataupload->host_inittemp();
    fee_coe = dataupload->host_initrate();
    qDebug()<<"从数据库读取的初始化设定为"<<airc_mode<<default_temp<<fee_coe;
    if(!init_judge())//如果设置不合法
    {
        QMessageBox::information(NULL, "Warning", "初始化设置错误！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    ui->pushButton_3->setEnabled(true); //OFF可用
    ui->pushButton->setEnabled(false);  //ON不可用
    ui->pushButton_2->setEnabled(false);//模式设置不可用
    ui->pushButton_5->setEnabled(false);//温度范围设置不可用
    ui->pushButton_6->setEnabled(false);//计费模式设置不可用
    this->airc_state = managerON; //更改中央空调状态为打开
    dataupload->host_turnon(); //数据库更改中央空调状态为打开
    ui->label_2->setText(QString::number(this->default_temp));
    ui->label_4->setText(QString::number(this->fee_coe));
    QPalette pa;
    if(this->airc_mode == managerCold)
    {
        pa.setColor(QPalette::WindowText,Qt::blue);
        ui->label_6->setPalette(pa);
        ui->label_6->setText("制冷");
    }
    else
    {
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label_6->setPalette(pa);
        ui->label_6->setText("制热");
    }
    this->TCP_new();  //建立TCP监听
    qDebug()<<"主控机工作状态变为"<<this->airc_state;
}

void Window::on_pushButton_3_clicked()  //OFF按钮
{
    qDebug()<<"进入关机函数";
    ui->pushButton_3->setEnabled(false); //OFF不可用
    ui->pushButton->setEnabled(true);  //ON可用
    ui->pushButton_2->setEnabled(true);//模式设置可用
    ui->pushButton_5->setEnabled(true);//温度范围设置可用
    ui->pushButton_6->setEnabled(true);//计费模式设置可用
    this->airc_state = managerOff;//更改状态为关闭
    waiting_queue.clear();
    serve_queue.clear();   //清空两个队列
    //向工作队列中各房间从控机发送消息，通知其断开
    this->TCP_off(); //关闭TCP监听
    /*完善工作日志*/
    this->host_off();//执行关闭函数，清空所有初始化设定
    dataupload->host_turnoff();
    qDebug()<<"主控机工作状态变为"<<this->airc_state;
}

void Window::on_pushButton_2_clicked()  //模式设置按钮
{
    QPalette pa;
    if(this->airc_mode == managerCold)
    {
        this->airc_mode = managerHeat;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label_6->setPalette(pa);
        ui->label_6->setText("制热");
    }
    else
    {
        this->airc_mode = managerCold;
        pa.setColor(QPalette::WindowText,Qt::blue);
        ui->label_6->setPalette(pa);
        ui->label_6->setText("制冷");
    }
    if(this->airc_mode == managerHeat)
        dataupload->host_cmode(0);
    else {
        dataupload->host_cmode(1);
    }
    qDebug()<<"主控机默认模式变为"<<this->airc_mode;
}

void Window::on_pushButton_5_clicked()  //温度范围设置按钮
{
    bool ok;
    double inputTem = QInputDialog::getDouble(this,tr("请输入缺省温度"),tr("缺省温度"),25.0,18.0,30.0,1,&ok);
    if(ok)
        this->default_temp = inputTem; //更改设置
    ui->label_2->setText(QString::number(this->default_temp));
    dataupload->host_ctemp(inputTem,30.0,18.0);
    qDebug()<<"主控机默认温度变为"<<this->default_temp;
}

void Window::on_pushButton_6_clicked()  //计费模式设置按钮
{
    bool ok;
    double inputFree = QInputDialog::getDouble(this,tr("请输入费率"),tr("计费费率"),0.3,0.1,1.0,1,&ok);
    if(ok)
        this->fee_coe = inputFree; //更改设置
    ui->label_4->setText(QString::number(this->fee_coe));
    dataupload->host_crate(this->fee_coe);
    qDebug()<<"主控机默认计费费率变为"<<this->fee_coe;
}

void Window::update_subui() //房间信息UI刷新
{
    delete ItemModel; //先释放
    ItemModel = new QStandardItemModel(this);//否则可能存在内存泄漏

    QMap<int,Submachine>::Iterator ptr = submacinfo.begin();
    qDebug()<<"执行房间信息UI更新"<<submacinfo.size();
    while(ptr != submacinfo.end())
    {
        QStandardItem *item = new QStandardItem();
        item->setData(QVariant::fromValue(ptr.value()),Qt::UserRole+1);
        ItemModel->appendRow(item);
        qDebug()<<"进入房间UI刷新"<<ptr.value().now_model<<ptr.value().roomid;
        ptr ++;
    }
    Display *pItemDelegate = new Display(this);
    ui->listView->setItemDelegate(pItemDelegate);
    ui->listView->setModel(ItemModel);
    ui->listView->setSpacing(50);                   //为视图设置控件间距
    ui->listView->setViewMode(QListView::IconMode); //设置Item图标显示

}

Display::Display(QObject *parent) :
QStyledItemDelegate(parent)
{
}

void Display::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if (index.isValid()) {

        painter->save();//save() 和 restore() 可以以堆栈的形式嵌套式地保存和恢复画笔状态

        QVariant var = index.data(Qt::UserRole+1);
//QVariant可以存储各种数据类型，QVariant行为类似于C/C++的union，
//但在Qt中比union强大很多, QVariant内置支持所有QMetaType::Type里声明的类型如:int，QString，QFont，QColor等，
//甚至QList，QMap<QString， QVariant>等组成的任意复杂类型。简单的说QVariant可以存储任意数据类型
        Submachine itemData = var.value<Submachine>();


        // item 矩形区域

        QRectF rect;
//矩形类 rect
        rect.setX(option.rect.x());
//设置x点坐标
        rect.setY(option.rect.y());
//设置y点坐标
        rect.setWidth(option.rect.width());
//设置宽度
        rect.setHeight(option.rect.height());
//设置高度

        //QPainterPath画圆角矩形
        const qreal radius = 7;
        QPainterPath path;
        //QPainterPath 类（绘图路径）提供了一个容器，用于绘图操作，可以创建和重用图形形状。
        //QPainterPath 可以被填充、描绘轮廓、裁剪。要为一个指定的绘图路径生成可填充的轮廓，可以使用 QPainterPathStroker 类。
        //与正常绘图相比，QPainterPath 的主要优点在于：复杂的图形只需创建一次，然后只需调用 QPainter::drawPath() 函数即可绘制多次。
        path.moveTo(rect.topRight() - QPointF(radius, 0));
        path.lineTo(rect.topLeft() + QPointF(radius, 0));
        path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
        path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
        path.lineTo(rect.bottomRight() - QPointF(radius, 0));
        path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
        path.lineTo(rect.topRight() + QPointF(0, radius));
        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));


        painter->setPen(QPen(QColor(89, 225, 255),3));
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(path);


        QRectF roomidRect = QRect(rect.left()+5, rect.top()+5, 200, 20);
//把房间号这个小矩形放入大矩形
        QRectF currentRect = QRect(rect.left()+5, roomidRect.bottom()+5, 200, 20);
//把当前温度这个小矩形放入大矩形
        QRectF targetTRect = QRect(rect.left()+5, currentRect.bottom()+5, 200, 20);
//把目标风速这个小矩形放入大矩形
        QRectF speedRect = QRect(rect.left()+5, targetTRect.bottom()+5, 200, 20);
//把当前风速这个小矩形放入大矩形
        QRectF targetspeedRect = QRect(rect.left()+5, speedRect.bottom()+5, 200, 20);
//把目标风速这个小矩形放入大矩形
        QRectF currentModelRect =QRect(rect.left()+5, targetspeedRect.bottom()+5,200,20);
//把当前模式这个小矩形放入大矩形
        QString s;
        painter->setPen(QPen(Qt::black));
        //设置颜色黑色的画笔
        painter->setFont(QFont("Microsoft Yahei",10,QFont::Bold));
        //设置画笔字体Microsoft Yahei10号加粗
        s = QString::number(itemData.roomid);
        s = "房间号： " + s;
        painter->drawText(roomidRect, s);
        //画出s字符串的文本
        painter->setPen(QPen(Qt::black));//设置颜色黑色的画笔
        painter->setFont(QFont("Microsoft Yahei",10,QFont::Bold));//设置画笔字体Microsoft Yahei10号加粗
        s = QString::number(itemData.current_temp);
        s = "当前温度： " + s;
        painter->drawText(currentRect, s);



        painter->setPen(QPen(Qt::black));//设置颜色黑色的画笔
        painter->setFont(QFont("Microsoft Yahei", 10,QFont::Bold));//设置画笔字体Microsoft Yahei10号加粗
        s = QString::number(itemData.set_temp);
        s = "目标温度： " + s;//画出s字符串的文本
        painter->drawText(targetTRect, s);



        painter->setPen(QPen(Qt::black));//设置颜色黑色的画笔
        painter->setFont(QFont("Microsoft Yahei", 10,QFont::Bold));//设置画笔字体Microsoft Yahei10号加粗
        s = QString::number(itemData.fan_speed);
        s = "当前风速： " + s;//画出s字符串的文本
        painter->drawText( speedRect, s);



        painter->setPen(QPen(Qt::black));//设置颜色黑色的画笔
        painter->setFont(QFont("Microsoft Yahei", 10,QFont::Bold));//设置画笔字体Microsoft Yahei10号加粗
        s = QString::number(itemData.set_speed);
        s = "设定风速： " + s;
        painter->drawText(targetspeedRect , s);//画出s字符串的文本



        painter->setPen(QPen(Qt::black));//设置颜色黑色的画笔
        painter->setFont(QFont("Microsoft Yahei", 10,QFont::Bold));//设置画笔字体Microsoft Yahei10号加粗
        s = "当前模式： " + itemData.now_model;
        painter->drawText(currentModelRect,s );
//画出s字符串的文本
        painter->restore();//save() 和 restore() 可以以堆栈的形式嵌套式地保存和恢复画笔状态

    }

}

QSize Display::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(200, 200);//设置大小范围为150*150
}
