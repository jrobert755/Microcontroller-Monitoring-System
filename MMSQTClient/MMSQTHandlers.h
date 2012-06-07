#ifndef MMSQTHANDLERS_H
#define MMSQTHANDLERS_H

#include <QFileDialog>
#include <QObject>

#include <MMS.h>
#include <MMSConnections.h>
#include <Listeners.h>
#include <QString>
#include <QVector>
#include "mainwindow.h"
#include "graphclass.h"

class GUIMessenger : public QObject{
    Q_OBJECT

public:
    QString createSaveFileDialog(QString caption, QString path, QString outputs, QFileDialog::Options options);
    void setReturnData(QString data);

    void createWidget(const char *type, vector<string> parameters);
    //void setReturnWidget(BaseWidget* widget);
private:
    bool m_data_ready;
    QString m_return_value;

    //bool m_widget_ready;
    //BaseWidget* m_widget;
signals:
    void createDialog(QString, QString, QString, int);
    void createGUIWidget(QString, vector<string>);
};

extern MainWindow* main_window;
extern GUIMessenger* messenger;

bool defaultHandler(Message* message, MMSConnection* connection);

bool connectedArduinos(Message* message, MMSConnection* connection);
bool disconnectedArduino(Message* message, MMSConnection* connection);

bool newArduinoConnection(Message* message, MMSConnection* connection);

bool arduinoReadings(Message* message, MMSConnection* connection);

bool newFile(Message* message, MMSConnection* connection);

#endif // MMSQTHANDLERS_H
