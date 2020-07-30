#ifndef NEWCONNECT_H
#define NEWCONNECT_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/Users/boger/Desktop/314b/linkdata.db");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection."
                     ), QMessageBox::Cancel);
        return false;
    }
     QSqlQuery query;
     query.exec("insert into room values(0, 0, 0)");
     return true;
}

#endif // NEWCONNECT_H
