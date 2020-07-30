/********************************************************************************
** Form generated from reading UI file 'subscriber.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUBSCRIBER_H
#define UI_SUBSCRIBER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Subscriber
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout;
    QPushButton *on;
    QPushButton *exit;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QPushButton *set_temp;
    QPushButton *set_temp_2;
    QPushButton *set_wind;
    QPushButton *set_mode;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *model_of_main;
    QLabel *cost;
    QLabel *wind_speed_2;
    QLabel *label;
    QLabel *wind_speed;
    QLabel *label_6;
    QLCDNumber *lcd_goaltemp;
    QLabel *label_2;
    QLCDNumber *lcd_currenttemp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Subscriber)
    {
        if (Subscriber->objectName().isEmpty())
            Subscriber->setObjectName(QString::fromUtf8("Subscriber"));
        Subscriber->resize(829, 540);
        QFont font;
        font.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font.setPointSize(10);
        Subscriber->setFont(font);
        Subscriber->setStyleSheet(QString::fromUtf8(""));
        centralWidget = new QWidget(Subscriber);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(140, 80, 511, 401));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\215\216\346\226\207\351\232\266\344\271\246"));
        font1.setPointSize(36);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        label_5->setFont(font1);
        label_5->setLayoutDirection(Qt::LeftToRight);
        label_5->setStyleSheet(QString::fromUtf8("font: 36pt \"\345\215\216\346\226\207\351\232\266\344\271\246\";\n"
"color: rgb(0, 0, 255);"));
        label_5->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        on = new QPushButton(layoutWidget);
        on->setObjectName(QString::fromUtf8("on"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221 Light"));
        font2.setPointSize(11);
        on->setFont(font2);
        on->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        horizontalLayout->addWidget(on);

        exit = new QPushButton(layoutWidget);
        exit->setObjectName(QString::fromUtf8("exit"));
        exit->setFont(font2);
        exit->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 127);"));

        horizontalLayout->addWidget(exit);


        horizontalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2->setStretch(0, 4);
        horizontalLayout_2->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        set_temp = new QPushButton(layoutWidget);
        set_temp->setObjectName(QString::fromUtf8("set_temp"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\345\215\216\346\226\207\346\226\260\351\255\217"));
        font3.setPointSize(11);
        set_temp->setFont(font3);

        verticalLayout->addWidget(set_temp);

        set_temp_2 = new QPushButton(layoutWidget);
        set_temp_2->setObjectName(QString::fromUtf8("set_temp_2"));
        set_temp_2->setFont(font3);

        verticalLayout->addWidget(set_temp_2);

        set_wind = new QPushButton(layoutWidget);
        set_wind->setObjectName(QString::fromUtf8("set_wind"));
        set_wind->setFont(font3);
        set_wind->setStyleSheet(QString::fromUtf8("color: rgb(85, 255, 0);"));

        verticalLayout->addWidget(set_wind);

        set_mode = new QPushButton(layoutWidget);
        set_mode->setObjectName(QString::fromUtf8("set_mode"));
        set_mode->setFont(font3);
        set_mode->setStyleSheet(QString::fromUtf8("color: rgb(85, 255, 0);"));

        verticalLayout->addWidget(set_mode);


        horizontalLayout_3->addLayout(verticalLayout);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font4;
        font4.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font4.setPointSize(11);
        font4.setBold(false);
        font4.setItalic(false);
        font4.setWeight(50);
        label_3->setFont(font4);
        label_3->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label_3->setFrameShape(QFrame::StyledPanel);
        label_3->setFrameShadow(QFrame::Raised);

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QFont font5;
        font5.setFamily(QString::fromUtf8("\345\215\216\346\226\207\347\220\245\347\217\200"));
        font5.setPointSize(12);
        label_7->setFont(font5);
        label_7->setFrameShape(QFrame::NoFrame);

        gridLayout_2->addWidget(label_7, 3, 0, 1, 1);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font5);
        label_8->setFrameShape(QFrame::NoFrame);

        gridLayout_2->addWidget(label_8, 4, 0, 1, 1);

        model_of_main = new QLabel(layoutWidget);
        model_of_main->setObjectName(QString::fromUtf8("model_of_main"));
        QFont font6;
        font6.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font6.setPointSize(10);
        font6.setBold(false);
        font6.setWeight(50);
        model_of_main->setFont(font6);
        model_of_main->setFrameShape(QFrame::StyledPanel);
        model_of_main->setFrameShadow(QFrame::Raised);
        model_of_main->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(model_of_main, 5, 1, 1, 1);

        cost = new QLabel(layoutWidget);
        cost->setObjectName(QString::fromUtf8("cost"));
        QFont font7;
        font7.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font7.setPointSize(10);
        font7.setBold(true);
        font7.setWeight(75);
        cost->setFont(font7);
        cost->setFrameShape(QFrame::StyledPanel);
        cost->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(cost, 0, 1, 1, 1);

        wind_speed_2 = new QLabel(layoutWidget);
        wind_speed_2->setObjectName(QString::fromUtf8("wind_speed_2"));
        wind_speed_2->setFont(font6);
        wind_speed_2->setFrameShape(QFrame::StyledPanel);
        wind_speed_2->setFrameShadow(QFrame::Raised);
        wind_speed_2->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(wind_speed_2, 4, 1, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font5);
        label->setFrameShape(QFrame::NoFrame);

        gridLayout_2->addWidget(label, 2, 0, 1, 1);

        wind_speed = new QLabel(layoutWidget);
        wind_speed->setObjectName(QString::fromUtf8("wind_speed"));
        wind_speed->setFont(font6);
        wind_speed->setFrameShape(QFrame::StyledPanel);
        wind_speed->setFrameShadow(QFrame::Raised);
        wind_speed->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(wind_speed, 3, 1, 1, 1);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font5);
        label_6->setFrameShape(QFrame::NoFrame);

        gridLayout_2->addWidget(label_6, 5, 0, 1, 1);

        lcd_goaltemp = new QLCDNumber(layoutWidget);
        lcd_goaltemp->setObjectName(QString::fromUtf8("lcd_goaltemp"));

        gridLayout_2->addWidget(lcd_goaltemp, 1, 1, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font5);
        label_2->setFrameShape(QFrame::NoFrame);

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        lcd_currenttemp = new QLCDNumber(layoutWidget);
        lcd_currenttemp->setObjectName(QString::fromUtf8("lcd_currenttemp"));

        gridLayout_2->addWidget(lcd_currenttemp, 2, 1, 1, 1);


        horizontalLayout_3->addLayout(gridLayout_2);


        verticalLayout_2->addLayout(horizontalLayout_3);

        Subscriber->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(Subscriber);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Subscriber->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Subscriber);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Subscriber->setStatusBar(statusBar);

        retranslateUi(Subscriber);
        QObject::connect(on, SIGNAL(clicked()), Subscriber, SLOT(state_change()));
        QObject::connect(set_mode, SIGNAL(clicked()), Subscriber, SLOT(mode_change()));
        QObject::connect(set_wind, SIGNAL(clicked()), Subscriber, SLOT(speed_change()));
        QObject::connect(set_temp, SIGNAL(clicked()), Subscriber, SLOT(temperature_up()));
        QObject::connect(set_temp_2, SIGNAL(clicked()), Subscriber, SLOT(temperature_down()));
        QObject::connect(exit, SIGNAL(clicked()), Subscriber, SLOT(check_out()));

        QMetaObject::connectSlotsByName(Subscriber);
    } // setupUi

    void retranslateUi(QMainWindow *Subscriber)
    {
        Subscriber->setWindowTitle(QApplication::translate("Subscriber", "User", nullptr));
        label_5->setText(QApplication::translate("Subscriber", "\344\273\216\346\216\247\346\234\272\347\263\273\347\273\237", nullptr));
        on->setText(QApplication::translate("Subscriber", "\347\224\265\346\272\220", nullptr));
        exit->setText(QApplication::translate("Subscriber", "\351\200\200\346\210\277", nullptr));
        set_temp->setText(QApplication::translate("Subscriber", "\346\270\251\345\272\246+", nullptr));
        set_temp_2->setText(QApplication::translate("Subscriber", "\346\270\251\345\272\246-", nullptr));
        set_wind->setText(QApplication::translate("Subscriber", "\351\243\216\351\200\237", nullptr));
        set_mode->setText(QApplication::translate("Subscriber", "\346\250\241\345\274\217", nullptr));
        label_3->setText(QApplication::translate("Subscriber", "\350\264\271  \347\224\250\357\274\232", nullptr));
        label_7->setText(QApplication::translate("Subscriber", "\350\256\276\345\256\232\351\243\216\351\200\237\357\274\232", nullptr));
        label_8->setText(QApplication::translate("Subscriber", "\345\275\223\345\211\215\351\243\216\351\200\237\357\274\232", nullptr));
        model_of_main->setText(QString());
        cost->setText(QString());
        wind_speed_2->setText(QApplication::translate("Subscriber", "0", nullptr));
        label->setText(QApplication::translate("Subscriber", "\345\275\223\345\211\215\346\270\251\345\272\246\357\274\232", nullptr));
        wind_speed->setText(QApplication::translate("Subscriber", "0", nullptr));
        label_6->setText(QApplication::translate("Subscriber", "\347\251\272\350\260\203\346\250\241\345\274\217\357\274\232", nullptr));
        label_2->setText(QApplication::translate("Subscriber", "\347\233\256\346\240\207\346\270\251\345\272\246\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Subscriber: public Ui_Subscriber {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBSCRIBER_H
