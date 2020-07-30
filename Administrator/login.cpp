#include "login.h"
#include "ui_login.h"
#include "dbcontrol.h"
#include "window.h"
#include <QDebug>
#include <QApplication>

#pragma execution_character_set("utf-8")

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setWindowTitle("登录");
    connect(ui->pushButton,&QPushButton::clicked,this,&login::dengluClick);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&login::tuichuclick);

}

login::~login()
{
    delete ui;
}

void login::dengluClick(){
    username=ui->yhEdit->text();
    secret=ui->mmEdit->text();
    Dbcontrol *datacontrollor=new Dbcontrol(this);
    int kind=datacontrollor->host_login(username,secret);
    qDebug()<<kind;
    if(kind==-2)
        QMessageBox::warning(0,"错误","密码错误！",QMessageBox::Ok);
    else if(kind==-1)
        QMessageBox::warning(0,"错误","不存在此用户名！",QMessageBox::Ok);
    else if(kind==0){
        this->close();
        Window *w = new Window();
        w->setWindowTitle("中央空调主控机面板");
        w->show();
    }
    else{
        QMessageBox::warning(0,"错误","类型错误！",QMessageBox::Ok);
    }
}

void login::tuichuclick()
{
   this->close();
}
