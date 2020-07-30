#ifndef LOGIN_H
#define LOGIN_H


#include <QDialog>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <frontdesk.h>
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
private slots:






    void on_login_button_clicked();

    void on_cancel_button_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
