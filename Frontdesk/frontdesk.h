#ifndef FRONTDESK_H
#define FRONTDESK_H

#include <QWidget>

#include <QString>
#include <QTableWidget>
#include <QObject>
#include <QSqlQuery>
#include <QDebug>
#include <QTableWidget>
#include <QFile>
#include <QMessageBox>

namespace Ui {
class Frontdesk;
}

class Frontdesk : public QWidget
{
    Q_OBJECT

public:
    explicit Frontdesk(QWidget *parent = nullptr);
    ~Frontdesk();

private slots:
    void on_calendarWidget_selectionChanged();

    void on_calendarWidget_activated(const QDate &date);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_textEdit_textChanged();

    //void on_tableWidget_Doc_activated(const QModelIndex &index);

    void on_adjustbutton_clicked();

    void on_billbutton_clicked();

signals:
    void signal_Doc();
    void signal_Fee();

private:
    Ui::Frontdesk *ui;

    QTableWidget * tableWidget_Doc;//创建窗口页面指针
    QTableWidget * tableWidget_Fee;
};

#endif // FRONTDESK_H
