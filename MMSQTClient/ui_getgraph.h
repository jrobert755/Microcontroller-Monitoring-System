/********************************************************************************
** Form generated from reading UI file 'getgraph.ui'
**
** Created: Tue May 29 10:59:09 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GETGRAPH_H
#define UI_GETGRAPH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_GetGraph
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GetGraph)
    {
        if (GetGraph->objectName().isEmpty())
            GetGraph->setObjectName(QString::fromUtf8("GetGraph"));
        GetGraph->resize(400, 300);
        buttonBox = new QDialogButtonBox(GetGraph);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(GetGraph);
        QObject::connect(buttonBox, SIGNAL(accepted()), GetGraph, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), GetGraph, SLOT(reject()));

        QMetaObject::connectSlotsByName(GetGraph);
    } // setupUi

    void retranslateUi(QDialog *GetGraph)
    {
        GetGraph->setWindowTitle(QApplication::translate("GetGraph", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GetGraph: public Ui_GetGraph {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GETGRAPH_H
