#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectiondialog.h"
#include "graphclass.h"
#include "getlogs.h"
#include "MMSQTHandlers.h"

#include <iostream>

using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_connection = NULL;
    m_connection_listener = NULL;
    m_thread = NULL;
    m_name = "";

#ifdef _WIN32
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        //cerr << "Unable to start WSAData" << endl;
        //return 1;
        throw -1;
    }
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
#ifdef _WIN32
    WSACleanup();
#endif
}

void MainWindow::registerConnectionListener(MMSConnectionListener *listener){
    m_connection_listener = listener;
}

void MainWindow::on_pushButton_clicked()
{
    //string to_send = "echo hello";
    string to_send = "getPinData CIC 0 0 0 0";
    vector<byte> to_send_vector;
    to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
    m_connection->handleWrite(to_send_vector, 1, m_name);

    /*GraphClass* graph = new GraphClass("testing", 2);
    graph->addPoint(QPointF((double)1334156743, 4.0));
    graph->addPoint(QPointF((double)1334156752, 3.0));
    graph->show();*/
}

void MainWindow::on_actionConnect_triggered()
{
    ConnectionDialog connection_dialog(this);
    connect(&connection_dialog, SIGNAL(connectRequest(string,string,string)), this, SLOT(connectRequest(string,string,string)));
    connection_dialog.exec();
}

void MainWindow::connectRequest(string host, string port, string username){
    if(m_connection == NULL){
        if(host == "" || port == "" || username == ""){
            ConnectionDialog connection_dialog(this);
            connect(&connection_dialog, SIGNAL(connectRequest(string,string,string)), this, SLOT(connectRequest(string,string,string)));
            connection_dialog.exec();
            return;
        }
        m_connection = new MMSConnection();
        int connection_status = m_connection->connectToHost(host, port, false);
        if(connection_status != 0){
            ConnectionDialog connection_dialog(this);
            connect(&connection_dialog, SIGNAL(connectRequest(string,string,string)), this, SLOT(connectRequest(string,string,string)));
            connection_dialog.exec();
            return;
        }
        MMSConnectEvent connectEvent(m_connection, port);
        m_connection_listener->onConnectEvent(connectEvent);
        m_thread = new ReadThread(m_connection);
        connect(m_thread, SIGNAL(finished()), this, SLOT(onReadThreadClose()));
        m_thread->start();
    } else{
        if(!m_connection->isConnected()){
            if(host == "" || port == "" || username == ""){
                ConnectionDialog connection_dialog(this);
                connect(&connection_dialog, SIGNAL(connectRequest(string,string,string)), this, SLOT(connectRequest(string,string,string)));
                connection_dialog.exec();
                return;
            }
            int connection_status = m_connection->connectToHost(host, port, false);
            if(connection_status != 0){
                ConnectionDialog connection_dialog(this);
                connect(&connection_dialog, SIGNAL(connectRequest(string,string,string)), this, SLOT(connectRequest(string,string,string)));
                connection_dialog.exec();
                return;
            }
            MMSConnectEvent connectEvent(m_connection, port);
            m_connection_listener->onConnectEvent(connectEvent);
            delete m_thread;
            m_thread = new ReadThread(m_connection);
            connect(m_thread, SIGNAL(finished()), this, SLOT(onReadThreadClose()));
            m_thread->start();
        }
    }

    m_name = username;
    string to_send = "getConnectedArduinos";
    vector<byte> to_send_vector;
    to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
    m_connection->handleWrite(to_send_vector, 2, m_name);
    setUI(true);
}

void MainWindow::getLogs(string name){
    string to_send = "getLogs ";
    to_send += name;
    vector<byte> to_send_vector;
    to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
    m_connection->handleWrite(to_send_vector, 2, m_name);
}

void MainWindow::getLogsByPin(string name){
    string to_send = "getLogsByPin ";
    to_send += name;
    vector<byte> to_send_vector;
    to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
    m_connection->handleWrite(to_send_vector, 2, m_name);
}

void MainWindow::addArduinoToList(string name){
    ui->arduino_list->addItem(new QListWidgetItem(QString::fromLocal8Bit(name.c_str())));
}

void MainWindow::removeArduinoFromList(string name){
    QString q_name = QString::fromLocal8Bit(name.c_str());
    Qt::MatchFlags flags = Qt::MatchExactly || Qt::MatchCaseSensitive;
    QList<QListWidgetItem*> items = ui->arduino_list->findItems(q_name, flags);
    //QList<QListWidgetItem*> selected = lists->selectedItems();
    if(items.size() != 0){
        for(int i = 0; i < items.size(); i++){
            /*if(selected.contains(items[i])){
                clearPinData();
            }*/
            ui->arduino_list->removeItemWidget(items[i]);
            delete items[i];
        }
    }
}

void MainWindow::on_actionGet_Logs_triggered()
{
    GetLogs get_logs(this);
    connect(&get_logs, SIGNAL(getLogs(string)), this, SLOT(getLogs(string)));
    get_logs.exec();
}

void MainWindow::on_actionGet_Logs_By_Pin_triggered()
{
    GetLogs get_logs(this);
    connect(&get_logs, SIGNAL(getLogs(string)), this, SLOT(getLogsByPin(string)));
    get_logs.exec();
}

void MainWindow::createSaveDialog(QString caption, QString path, QString outputs, int options){
    QObject* sender_object = sender();
    GUIMessenger* messenger = qobject_cast<GUIMessenger*>(sender_object);
    if(messenger == 0) return;
    QString log_save_location = QFileDialog::getSaveFileName(this, caption, path, outputs, 0, (QFileDialog::Options)options);
    cout << "Gets save location" << endl << log_save_location.toStdString() << endl;
    messenger->setReturnData(log_save_location);
}

void MainWindow::createBaseWidget(QString type, vector<string> parameters){
    QObject* sender_object = sender();
    GUIMessenger* messenger = qobject_cast<GUIMessenger*>(sender_object);
    if(messenger == 0) return;
    if(type == "GraphClass"){
        //if(parameters.size() != 2) messenger->setReturnWidget(NULL);
        string name = parameters[0];
        int pin = atoi(parameters[1].c_str());
        GraphClass* graph_class = new GraphClass(name, pin);
        for(unsigned int i = 2; i < parameters.size(); i++){
            string current = parameters[i];
            int time;
            double reading;

            sscanf(current.c_str(), "%d:%lf", &time, &reading);
            graph_class->addPoint(QPointF((double)time, reading));
        }
        graph_class->show();
        //messenger->setReturnWidget(graph_class);
    }
}

void MainWindow::onReadThreadClose(){
    cout << "Thread exited" << endl;
    setUI(false);
}

void MainWindow::on_actionDisconnect_triggered()
{
    m_connection->setConnected(false);
}

void MainWindow::setUI(bool connected){
    ui->arduino_list->setEnabled(connected);
    ui->actionConnect->setEnabled(!connected);
    ui->actionDisconnect->setEnabled(connected);
    ui->actionGet_Logs->setEnabled(connected);
    ui->actionGet_Logs_By_Pin->setEnabled(connected);
    ui->pushButton->setEnabled(connected);
}
