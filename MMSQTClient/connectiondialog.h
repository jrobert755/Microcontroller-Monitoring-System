#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <string>

using std::string;

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConnectionDialog(QWidget *parent = 0);
    ~ConnectionDialog();

signals:
    void connectRequest(string host, string port, string username);
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::ConnectionDialog *ui;
};

#endif // CONNECTIONDIALOG_H
