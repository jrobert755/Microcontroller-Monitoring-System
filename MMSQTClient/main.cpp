#include <QtGui/QApplication>
#include <QObject>
#include "mainwindow.h"
#include "MMSQTHandlers.h"

MainWindow* main_window = NULL;
GUIMessenger* messenger = new GUIMessenger();

int main(int argc, char *argv[])
{
    qRegisterMetaType<vector<string> >("vector<string>");
    QApplication a(argc, argv);

    MainListener* listener = new MainListener();
    listener->registerDefaultMessageHandler(defaultHandler);
    listener->registerMessageHandler("connectedArduinos", connectedArduinos);
    listener->registerMessageHandler("disconnectedArduino", disconnectedArduino);
    listener->registerMessageHandler("arduinoReadings", arduinoReadings);
    listener->registerMessageHandler("newArduinoConnection", newArduinoConnection);
    listener->registerMessageHandler("newFile", newFile);

    MainWindow w;
    main_window = &w;
    QObject::connect(messenger, SIGNAL(createDialog(QString,QString,QString,int)), &w, SLOT(createSaveDialog(QString,QString,QString,int)), Qt::QueuedConnection);
    QObject::connect(messenger, SIGNAL(createGUIWidget(QString,vector<string>)), &w, SLOT(createBaseWidget(QString,vector<string>)), Qt::QueuedConnection);
    w.registerConnectionListener(listener);
    w.show();
    
    return a.exec();
}
