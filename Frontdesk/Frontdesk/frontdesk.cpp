#include "frontdesk.h"
#include "ui_frontdesk.h"
#include<QHeaderView>

Frontdesk::Frontdesk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Frontdesk)
{
    ui->setupUi(this);
    tableWidget_Doc = new QTableWidget(this);//实例化窗口页面
    tableWidget_Fee = new QTableWidget(this);

    ui->stackedWidget->addWidget(tableWidget_Doc);
    ui->stackedWidget->addWidget(tableWidget_Fee);
    ui->stackedWidget->setCurrentWidget(tableWidget_Doc);

    tableWidget_Doc->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget_Fee->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

Frontdesk::~Frontdesk()
{
    delete ui;
}

void Frontdesk::on_calendarWidget_selectionChanged()
{
    ui->textEdit->setText(ui->calendarWidget->selectedDate().toString("yyyy-MM-dd"));
}

void Frontdesk::on_calendarWidget_activated(const QDate &date)
{

}


void Frontdesk::on_lineEdit_textChanged(const QString &arg1)
{

}

void Frontdesk::on_textEdit_textChanged()
{

}

void Frontdesk::on_adjustbutton_clicked()
{
    //点击创建详单
    if(ui->stackedWidget->currentWidget() != tableWidget_Doc){
            ui->stackedWidget->setCurrentWidget(tableWidget_Doc);
    }
    tableWidget_Doc->setRowCount(1);
    tableWidget_Doc->setColumnCount(8);
    QStringList header1;
    //创建详单属性
    header1<<tr("房间号 ")<<tr("模式 ")<<tr("设置风速 ")<<tr("当前温度 ")<<tr("设定温度 ")<<tr("起始时间       ")<<tr("结束时间       ")<<tr("费用    ");
    tableWidget_Doc->setHorizontalHeaderLabels(header1);

    QFont font = tableWidget_Doc->horizontalHeader()->font();
    font.setBold(true);
    tableWidget_Doc->horizontalHeader()->setFont(font);
    tableWidget_Doc->setStyleSheet("selection-background-color:lightblue;");
    tableWidget_Doc->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightblue;}");
    tableWidget_Doc->resizeColumnsToContents();
    QSqlQuery query;
    int i = 0;
    int j = 0;
    int nRow = 0;
    int room_id = (ui->lineEdit->text()).toInt();
    QString s;
    query.exec(QString("select count(*) from adjust where roomid=%1").arg(room_id));
    query.first();
    nRow = query.value(0).toInt();
    tableWidget_Doc->setRowCount(nRow);
    query.exec(QString("select * from adjust where roomid=%1").arg(room_id));
    //添加显示详单
    while(query.next())
    {
        i=0;//房间号
        tableWidget_Doc->setItem(j,i,new QTableWidgetItem(query.value(0).toString()));
        qDebug()<<query.value(0);
        i++;//模式
        s = query.value(1).toString();
        if (s.compare("1")==0)
            tableWidget_Doc->setItem(j,i,new QTableWidgetItem("制冷"));
        else
            tableWidget_Doc->setItem(j,i,new QTableWidgetItem("制热"));
        qDebug()<<s;
        i++;//设置风速
        s = query.value(10).toString();
        if (s.compare("1")==0)
            tableWidget_Doc->setItem(j,i,new QTableWidgetItem("低"));
        else if(s.compare("2")==0)
            tableWidget_Doc->setItem(j,i,new QTableWidgetItem("中"));
        else
            tableWidget_Doc->setItem(j,i,new QTableWidgetItem("高"));
        qDebug()<<s;
        i++;//起始温度
        s = query.value(7).toString();
        tableWidget_Doc->setItem(j,i,new QTableWidgetItem(QString::number(query.value(7).toDouble(),10,2)+"℃"));
        qDebug()<<s;
        i++;//结束温度
        s = query.value(8).toString();
        tableWidget_Doc->setItem(j,i,new QTableWidgetItem(QString::number(query.value(8).toDouble(),10,2)+"℃"));
        qDebug()<<s;
        i++;//起始时间
        s = query.value(2).toString();
        tableWidget_Doc->setItem(j,i,new QTableWidgetItem(s));
        qDebug()<<s;
        i++;//结束时间
        s = query.value(3).toString();
        tableWidget_Doc->setItem(j,i,new QTableWidgetItem(s));
        qDebug()<<s;
        i++;//费用
        s = query.value(4).toString();
        tableWidget_Doc->setItem(j,i,new QTableWidgetItem(QString::number(query.value(4).toDouble(),10,2)));
        qDebug()<<s;
        j++;
    }


    //将详单保存成文件
    QFile file;
    file.setFileName("Doc.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msg;
        msg.setText("打开文件失败!");
        msg.exec();
    }
    else
    {
        qDebug("open file success");
        QTextStream out(&file);
        int rowCount = tableWidget_Doc->rowCount();
        int colCount = tableWidget_Doc->columnCount();

        //将表头写入文件
        qDebug()<<"row count:::"<<rowCount<<"col count:::"<<colCount;
        //for(int i = 0; i < colCount; i++)
        //{
            //QString rowHeaderString =tableWidget_Doc->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString();
            //out << rowHeaderString << "\t";
            //qDebug()<<rowHeaderString;
        //}
        QString str =header1.join("\t");
        qDebug()<< str;
        out << str;
        out << "\n";
        QString text;
        //读取单元格的数据并写入文件
        for (int r = 0; r < rowCount; r++)
        {
            for (int c = 0; c < colCount; c++)
            {
                if (tableWidget_Doc->item(r,c) == 0)
                //不能用isEmpty,QTableWidgetItem * QTableWidget::item ( int row, int column ) const
                //Returns the item for the given row and column if one has been set; otherwise returns 0.
                {
                    text = "null";
                }
               else
                {
                    text = tableWidget_Doc->item(r,c)->text();
                }
                qDebug()<<"text:::"<<text;
                out << text << "\t";
            }
            out << "\n";
        }
        out.flush();
        qDebug("saveDoc end:::");
        file.close();
        QMessageBox::information(0,tr("详单"),tr("导出成功！"));
    }
}

void Frontdesk::on_billbutton_clicked()
{
    //点击创建账单
    if(ui->stackedWidget->currentWidget() != tableWidget_Fee){
            ui->stackedWidget->setCurrentWidget(tableWidget_Fee);
    }
    tableWidget_Fee->setRowCount(1);
    tableWidget_Fee->setColumnCount(2);
    QStringList header2;
    header2<<"roomid"<<"totalfee";
    tableWidget_Fee->setHorizontalHeaderLabels(header2);

    QFont font = tableWidget_Fee->horizontalHeader()->font();
    font.setBold(true);
    tableWidget_Fee->horizontalHeader()->setFont(font);
    tableWidget_Fee->setStyleSheet("selection-background-color:lightblue;");
    tableWidget_Fee->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightblue;}");
    tableWidget_Fee->resizeColumnsToContents();
    QSqlQuery query;
    double fee=0;
    int room_id = (ui->lineEdit->text()).toInt();
    query.exec(QString("select fee from adjust where roomid=%1").arg(room_id));


    while(query.next())
    {
        fee = fee + query.value(0).toDouble();
        qDebug()<<query.value(0);
        qDebug()<<fee;
    }
    QString room_idString;
    room_idString = QString::number(room_id,10,0); //QString::number(a,基底，精度)
    tableWidget_Fee->setItem(0,0,new QTableWidgetItem(room_idString));
    QString feeString;
    feeString = QString::number(fee,10,5);
    tableWidget_Fee->setItem(0,1,new QTableWidgetItem(feeString));

    //创建账单文件
    QFile file;
    file.setFileName("Fee.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msg;
        msg.setText("打开文件失败!");
        msg.exec();
    }
    else
    {
        qDebug("open file success");
        QTextStream out(&file);
        int rowCount = tableWidget_Fee->rowCount();
        int colCount = tableWidget_Fee->columnCount();


        qDebug()<<"row count:::"<<rowCount<<"col count:::"<<colCount;
        QString str = header2.join("\t");
        qDebug()<< str;
        out << str;
        out << "\n";
        QString text;
        //将账单属性写进文件
        for (int r = 0; r < rowCount; r++)
        {
            for (int c = 0; c < colCount; c++)
            {
                if (tableWidget_Fee->item(r,c) == 0)
                //不能用isEmpty,QTableWidgetItem * QTableWidget::item ( int row, int column ) const
                //Returns the item for the given row and column if one has been set; otherwise returns 0.
                {
                    text = "null";
                }
               else
                {
                    text = tableWidget_Fee->item(r,c)->text();
                }
                qDebug()<<"text:::"<<text;
                out << text << "\t";
            }
            out << "\n";
        }
        out.flush();
        qDebug("saveDoc end:::");
        file.close();
        QMessageBox::information(0,tr("账单"),tr("导出成功！"));
    }
}
