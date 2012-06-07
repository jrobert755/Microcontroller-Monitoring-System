#include "MMSQTHandlers.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>

#ifdef _WIN32
#pragma warning(disable : 4100)
#endif

using std::cout;
using std::endl;
using std::ofstream;
using std::ios_base;

QString GUIMessenger::createSaveFileDialog(QString caption, QString path, QString outputs, QFileDialog::Options options){
    m_data_ready = false;
    emit(createDialog(caption, path, outputs, options));
    while(!m_data_ready){ Sleep(1000);}
    return m_return_value;
}

void GUIMessenger::setReturnData(QString data){
    m_return_value = data;
    m_data_ready = true;
}

void GUIMessenger::createWidget(const char* type, vector<string> parameters){
    //m_widget_ready = false;
    emit(createGUIWidget(QString::fromLocal8Bit(type), parameters));
    //while(!m_widget_ready){}
    //return m_widget;
}

/*void GUIMessenger::setReturnWidget(BaseWidget* widget){
    m_widget = widget;
    m_widget_ready = true;
}*/

bool defaultHandler(Message* message, MMSConnection* connection){
    cout << message->getFunctionName() << endl;
    return true;
}

bool connectedArduinos(Message* message, MMSConnection* connection){
    vector<string> parameters = message->getParameters();

    for(vector<string>::iterator iter = parameters.begin(); iter != parameters.end(); iter++){
        main_window->addArduinoToList(*iter);
    }
    return true;
}

bool disconnectedArduino(Message* message, MMSConnection* connection){
    vector<string> parameters = message->getParameters();
    if(parameters.size() == 0) return true;
    string name = parameters[0];

    main_window->removeArduinoFromList(name);
    return true;
}

bool newArduinoConnection(Message* message, MMSConnection* connection){
    vector<string> parameters = message->getParameters();
    if(parameters.size() == 0) return true;
    string name = parameters[0];

    main_window->addArduinoToList(name);
    return true;
}

bool arduinoReadings(Message* message, MMSConnection* connection){
    vector<string> parameters = message->getParameters();
    if(parameters.size() <= 2) return true;
    //vector<string> creation_parameters;
    //creation_parameters.push_back(parameters[0]);
    //creation_parameters.push_back(parameters[1]);

    /*GraphClass* graph_class = (GraphClass*)*/messenger->createWidget("GraphClass", parameters);
    /*for(unsigned int i = 2; i < parameters.size(); i++){
        string current = parameters[i];
        int time;
        double reading;

        sscanf(current.c_str(), "%d:%lf", &time, &reading);
        graph_class->addPoint(QPointF((double)time, reading));
    }
    graph_class->show();*/
    return true;
}

bool newFile(Message* message, MMSConnection* connection){
    vector<string> parameters = message->getParameters();
    if(parameters.size() == 0) return true;

    QString log_save_location = messenger->createSaveFileDialog(QObject::tr("Save File"), QDir::currentPath(), QObject::tr("TarBZip (*.tar.bzip2)"), QFileDialog::ShowDirsOnly);
    //cout << "Here" << endl << log_save_location.toStdString() << endl;
    if(log_save_location.length() == 0){
        return true;
    }

    vector<char> complete_file;
    complete_file.insert(complete_file.end(), parameters[0].begin(), parameters[0].end());
    for(unsigned int i = 1; i < parameters.size(); i++){
        complete_file.push_back(' ');
        complete_file.insert(complete_file.end(), parameters[i].begin(), parameters[i].end());
    }

    QByteArray array_name = log_save_location.toLocal8Bit();

    //ofstream ostr("testing.tar.bz2", ios_base::out | ios_base::binary);
    ofstream ostr(array_name.data(), ios_base::out | ios_base::binary);
    ostr.write(&complete_file[0], complete_file.size());
    return true;
}
