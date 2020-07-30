#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Dialogue;
}

class Dialogue : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogue(QWidget *parent = 0);
    ~Dialogue();

private:
    Ui::Dialogue *ui;

signals:
    void login_data(QString);   //A signal used to transmit data.

private slots:
    void on_loginbtn_clicked();
    void on_exitbtn_clicked();
};

#endif // DIALOGUE_H
