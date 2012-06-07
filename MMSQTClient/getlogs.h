#ifndef GETLOGS_H
#define GETLOGS_H

#include <QDialog>
#include <string>

using std::string;

namespace Ui {
class GetLogs;
}

class GetLogs : public QDialog
{
    Q_OBJECT
    
public:
    explicit GetLogs(QWidget *parent = 0);
    ~GetLogs();

signals:
    void getLogs(string arduino_name);
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::GetLogs *ui;
};

#endif // GETLOGS_H
