/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue May 29 10:59:09 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConnect;
    QAction *actionGet_Logs;
    QAction *actionGet_Logs_By_Pin;
    QAction *actionDisconnect;
    QWidget *centralWidget;
    QPushButton *pushButton;
    QListWidget *arduino_list;
    QMenuBar *menuBar;
    QMenu *menuConnection;
    QMenu *menuLogs;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 303);
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionGet_Logs = new QAction(MainWindow);
        actionGet_Logs->setObjectName(QString::fromUtf8("actionGet_Logs"));
        actionGet_Logs->setEnabled(false);
        actionGet_Logs_By_Pin = new QAction(MainWindow);
        actionGet_Logs_By_Pin->setObjectName(QString::fromUtf8("actionGet_Logs_By_Pin"));
        actionGet_Logs_By_Pin->setEnabled(false);
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        actionDisconnect->setEnabled(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setEnabled(false);
        pushButton->setGeometry(QRect(200, 10, 75, 23));
        arduino_list = new QListWidget(centralWidget);
        arduino_list->setObjectName(QString::fromUtf8("arduino_list"));
        arduino_list->setEnabled(false);
        arduino_list->setGeometry(QRect(10, 10, 171, 261));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 21));
        menuConnection = new QMenu(menuBar);
        menuConnection->setObjectName(QString::fromUtf8("menuConnection"));
        menuLogs = new QMenu(menuBar);
        menuLogs->setObjectName(QString::fromUtf8("menuLogs"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuConnection->menuAction());
        menuBar->addAction(menuLogs->menuAction());
        menuConnection->addAction(actionConnect);
        menuConnection->addAction(actionDisconnect);
        menuLogs->addAction(actionGet_Logs);
        menuLogs->addAction(actionGet_Logs_By_Pin);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("MainWindow", "Connect", 0, QApplication::UnicodeUTF8));
        actionGet_Logs->setText(QApplication::translate("MainWindow", "Get Logs", 0, QApplication::UnicodeUTF8));
        actionGet_Logs_By_Pin->setText(QApplication::translate("MainWindow", "Get Logs By Pin", 0, QApplication::UnicodeUTF8));
        actionDisconnect->setText(QApplication::translate("MainWindow", "Disconnect", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "PushButton", 0, QApplication::UnicodeUTF8));
        menuConnection->setTitle(QApplication::translate("MainWindow", "Connection", 0, QApplication::UnicodeUTF8));
        menuLogs->setTitle(QApplication::translate("MainWindow", "Logs", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
