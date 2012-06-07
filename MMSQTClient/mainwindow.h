#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef _WIN32
#pragma comment(lib, "ws2_32")
#endif

#include <QMainWindow>
#include <QFileDialog>
#include <MMS.h>
#include <MMSConnections.h>
#include <MMSListeners.h>
#include "readthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void registerConnectionListener(MMSConnectionListener* listener);
    void addArduinoToList(string name);
    void removeArduinoFromList(string name);

public slots:
    void createSaveDialog(QString caption, QString path, QString outputs, int options);
    void createBaseWidget(QString type, vector<string> parameters);
    
private slots:
    void on_pushButton_clicked();

    void on_actionConnect_triggered();

    void connectRequest(string host, string port, string username);
    void getLogs(string name);
    void getLogsByPin(string name);

    void on_actionGet_Logs_triggered();

    void on_actionGet_Logs_By_Pin_triggered();
    void onReadThreadClose();

    void on_actionDisconnect_triggered();

private:
    Ui::MainWindow *ui;
    MMSConnectionListener* m_connection_listener;
    MMSConnection* m_connection;
    ReadThread* m_thread;
    string m_name;
#ifdef _WIN32
    WSADATA wsaData;
#endif

    void setUI(bool connected);
};

#endif // MAINWINDOW_H
