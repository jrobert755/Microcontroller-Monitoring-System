/********************************************************************************
** Form generated from reading UI file 'connectiondialog.ui'
**
** Created: Tue May 29 10:59:09 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTIONDIALOG_H
#define UI_CONNECTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_ConnectionDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *address;
    QLineEdit *port;
    QLineEdit *username;

    void setupUi(QDialog *ConnectionDialog)
    {
        if (ConnectionDialog->objectName().isEmpty())
            ConnectionDialog->setObjectName(QString::fromUtf8("ConnectionDialog"));
        ConnectionDialog->resize(400, 153);
        buttonBox = new QDialogButtonBox(ConnectionDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(20, 110, 361, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(ConnectionDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 101, 16));
        label_2 = new QLabel(ConnectionDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 46, 13));
        label_3 = new QLabel(ConnectionDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 70, 51, 16));
        address = new QLineEdit(ConnectionDialog);
        address->setObjectName(QString::fromUtf8("address"));
        address->setGeometry(QRect(120, 10, 271, 20));
        port = new QLineEdit(ConnectionDialog);
        port->setObjectName(QString::fromUtf8("port"));
        port->setGeometry(QRect(120, 40, 271, 20));
        username = new QLineEdit(ConnectionDialog);
        username->setObjectName(QString::fromUtf8("username"));
        username->setGeometry(QRect(120, 70, 271, 20));
        QWidget::setTabOrder(address, port);
        QWidget::setTabOrder(port, username);
        QWidget::setTabOrder(username, buttonBox);

        retranslateUi(ConnectionDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConnectionDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConnectionDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConnectionDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectionDialog)
    {
        ConnectionDialog->setWindowTitle(QApplication::translate("ConnectionDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ConnectionDialog", "Server Address/URL", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ConnectionDialog", "Port", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ConnectionDialog", "Username", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConnectionDialog: public Ui_ConnectionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTIONDIALOG_H
