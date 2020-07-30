/********************************************************************************
** Form generated from reading UI file 'dialogue.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGUE_H
#define UI_DIALOGUE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialogue
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *titlelabel;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *userlabel;
    QLineEdit *userline;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *loginbtn;
    QSpacerItem *horizontalSpacer;
    QPushButton *exitbtn;
    QSpacerItem *horizontalSpacer_4;
    QLabel *mlabel;

    void setupUi(QDialog *Dialogue)
    {
        if (Dialogue->objectName().isEmpty())
            Dialogue->setObjectName(QString::fromUtf8("Dialogue"));
        Dialogue->resize(923, 415);
        verticalLayout_2 = new QVBoxLayout(Dialogue);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        titlelabel = new QLabel(Dialogue);
        titlelabel->setObjectName(QString::fromUtf8("titlelabel"));
        QPalette palette;
        QBrush brush(QColor(0, 170, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush);
        QBrush brush1(QColor(0, 120, 215, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush1);
        titlelabel->setPalette(palette);
        QFont font;
        font.setFamily(QString::fromUtf8("Levenim MT"));
        font.setPointSize(20);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(8);
        titlelabel->setFont(font);
        titlelabel->setStyleSheet(QString::fromUtf8("font: 65 20pt \"Levenim MT\";"));

        verticalLayout_2->addWidget(titlelabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        userlabel = new QLabel(Dialogue);
        userlabel->setObjectName(QString::fromUtf8("userlabel"));
        userlabel->setStyleSheet(QString::fromUtf8("font: 75 10pt \"Segoe Print\";\347\224\250\346\210\267\345\220\215"));

        horizontalLayout_4->addWidget(userlabel);

        userline = new QLineEdit(Dialogue);
        userline->setObjectName(QString::fromUtf8("userline"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(userline->sizePolicy().hasHeightForWidth());
        userline->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(userline);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        loginbtn = new QPushButton(Dialogue);
        loginbtn->setObjectName(QString::fromUtf8("loginbtn"));
        loginbtn->setStyleSheet(QString::fromUtf8("font: 75 10pt \"Segoe Print\";"));

        horizontalLayout->addWidget(loginbtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        exitbtn = new QPushButton(Dialogue);
        exitbtn->setObjectName(QString::fromUtf8("exitbtn"));
        exitbtn->setStyleSheet(QString::fromUtf8("font: 75 10pt \"Segoe Print\";\346\263\250\345\206\214\n"
""));

        horizontalLayout->addWidget(exitbtn);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);

        mlabel = new QLabel(Dialogue);
        mlabel->setObjectName(QString::fromUtf8("mlabel"));

        horizontalLayout_2->addWidget(mlabel);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalLayout_2->setStretch(0, 2);
        verticalLayout_2->setStretch(1, 8);

        retranslateUi(Dialogue);

        QMetaObject::connectSlotsByName(Dialogue);
    } // setupUi

    void retranslateUi(QDialog *Dialogue)
    {
        Dialogue->setWindowTitle(QApplication::translate("Dialogue", "Dialog", nullptr));
        titlelabel->setText(QApplication::translate("Dialogue", "Distributed Temperature Control System Customer Login", nullptr));
        userlabel->setText(QApplication::translate("Dialogue", "room id\357\274\232", nullptr));
        loginbtn->setText(QApplication::translate("Dialogue", "login", nullptr));
        exitbtn->setText(QApplication::translate("Dialogue", "quit", nullptr));
        mlabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Dialogue: public Ui_Dialogue {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGUE_H
