#include "loginwindow.h"
#include "mainwindow.h"
#include "ui_dialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <qDebug>
#include <QMessageBox>

loginwindow::loginwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginwindow){
    ui->setupUi(this);
}

loginwindow::~loginwindow(){
    delete ui;
}

void loginwindow::on_pushButton_clicked(){
    QSqlDatabase mydatabase=QSqlDatabase::addDatabase("QSQLITE","wconntosqlite1");
    mydatabase.setDatabaseName("/Users/boger/Desktop/314b/linkdata.db");
    if(mydatabase.open()){
        QSqlQuery query(mydatabase);
        QString sqlstr="select * from user";
        if(!query.exec(sqlstr)){
            qDebug()<<query.lastError();
        }
        else{
            while(query.next()){
                if(ui->lineEdit->text()==query.value(0).toString() && ui->lineEdit_2->text()==query.value(1).toString() && query.value(2).toInt()==2){
                    MainWindow *m;
                    m=new MainWindow();
                    m->show();
                    this->hide();
                    break;
                }
                else{
                     QMessageBox::critical(NULL, "警告", "用户名或密码错误", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                }
            }
        }
    }
}

void loginwindow::on_pushButton_2_clicked(){
    this->close();
    exit(0);
}
