#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include <sstream>
#include <ctime>
using std::ostringstream;

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::on_buttonBox_accepted()
{
    QString host = ui->address->text();
    QByteArray host_array = host.toLocal8Bit();
    string std_host = host_array.data();
    QString port = ui->port->text();
    QByteArray port_array = port.toLocal8Bit();
    string std_port = port_array.data();
    QString username = ui->username->text();
    QByteArray username_array = username.toLocal8Bit();
    string std_username = username_array.data();
    emit connectRequest(std_host, std_port, std_username);
}
