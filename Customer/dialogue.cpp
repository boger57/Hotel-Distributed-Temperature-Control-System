#include "dialogue.h"
#include "ui_dialogue.h"


Dialogue::Dialogue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogue)
{
    ui->setupUi(this);
    this->setWindowTitle("Customer landing");
    QPixmap pixmap("E://Qt//Qtworkfile//314b//314b//1.png");
    ui->mlabel->setPixmap(pixmap);
    ui->mlabel->show();
}

Dialogue::~Dialogue()
{
    delete ui;
}


void Dialogue::on_loginbtn_clicked()
{
    if(ui->userline->text().isEmpty())
        QMessageBox::warning(this, QString::fromLocal8Bit("worning"), QString::fromLocal8Bit("empty input"));
    else{
        this->hide();
        emit login_data(ui->userline->text());
    }
}

void Dialogue::on_exitbtn_clicked()
{
    this->close();
    exit(0);
}
