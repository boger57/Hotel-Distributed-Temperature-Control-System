#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication mainapp(argc, argv);
    //创建一个app对象

    login l;//创建一个login对象

    l.setWindowTitle("空调管理员登录");//设置窗口名称为空调管理员登录
    l.show();//显示对象

    return mainapp.exec();
}
