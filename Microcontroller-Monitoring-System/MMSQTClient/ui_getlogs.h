/********************************************************************************
** Form generated from reading UI file 'getlogs.ui'
**
** Created: Tue May 29 10:59:09 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GETLOGS_H
#define UI_GETLOGS_H

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

class Ui_GetLogs
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *arduino_name;

    void setupUi(QDialog *GetLogs)
    {
        if (GetLogs->objectName().isEmpty())
            GetLogs->setObjectName(QString::fromUtf8("GetLogs"));
        GetLogs->resize(351, 82);
        buttonBox = new QDialogButtonBox(GetLogs);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 40, 321, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(GetLogs);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 71, 16));
        arduino_name = new QLineEdit(GetLogs);
        arduino_name->setObjectName(QString::fromUtf8("arduino_name"));
        arduino_name->setGeometry(QRect(90, 10, 251, 20));

        retranslateUi(GetLogs);
        QObject::connect(buttonBox, SIGNAL(accepted()), GetLogs, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), GetLogs, SLOT(reject()));

        QMetaObject::connectSlotsByName(GetLogs);
    } // setupUi

    void retranslateUi(QDialog *GetLogs)
    {
        GetLogs->setWindowTitle(QApplication::translate("GetLogs", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GetLogs", "Arduino Name", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GetLogs: public Ui_GetLogs {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GETLOGS_H
