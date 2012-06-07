#include "getlogs.h"
#include "ui_getlogs.h"

GetLogs::GetLogs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetLogs)
{
    ui->setupUi(this);
}

GetLogs::~GetLogs()
{
    delete ui;
}

void GetLogs::on_buttonBox_accepted()
{
    QString name = ui->arduino_name->text();
    QByteArray name_array = name.toLocal8Bit();
    string std_name = name_array.data();
    emit getLogs(std_name);
}
