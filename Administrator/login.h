#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QButtonGroup>
#include <ctime>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT
public:
    explicit login(QWidget *parent = 0);
    ~login();
public slots:
    //void openFailed();
    void dengluClick();//点击登录按钮
    void tuichuclick();//点击退出按钮

private:
    Ui::login *ui;
    QString username;
    QString secret;
};

#endif // LOGIN_H
