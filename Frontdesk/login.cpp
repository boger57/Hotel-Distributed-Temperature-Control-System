#include "login.h"
#include "ui_login.h"
#include <QSqlError>


Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}



void Login::on_login_button_clicked()
{
    //验证身份
    int flag=0;
    QString username = ui->usernameedit->text();
    QString password = ui->passwordedit->text();

    QSqlQuery query;
    QString sqlstr="select * from user";
    if(!query.exec(sqlstr))
    {
        qDebug()<<query.lastError();
    }
    else
    {
        while(query.next())
        {
            if(username==query.value(0).toString() && password==query.value(1).toString() && query.value(2).toInt()==1)
            {
                flag=1;
                break;
            }
        }
    }

    if(flag==1)
    {
        Frontdesk * w;
        w = new Frontdesk();
        w->show();
        this->hide();
    }
    else
        QMessageBox::critical(NULL, "critical", "用户名或密码错误", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void Login::on_cancel_button_clicked()
{
    this->close();
    exit(0);
}
