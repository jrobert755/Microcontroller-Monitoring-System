#ifndef GRAPHCLASS_H
#define GRAPHCLASS_H
#include <QHBoxLayout>
#include <QCloseEvent>
#include <qwt_scale_draw.h>
#include "incrementalplot.h"
#include <time.h>
#include <string>
#include "basewidget.h"

using std::string;

class GraphWidget : public QWidget{
    Q_OBJECT
public:

protected:
    void closeEvent(QCloseEvent* event);

signals:
    void closing();
};

class GraphClass : public QObject, public BaseWidget
{
    Q_OBJECT
public:
    GraphClass(string arduino_name, int pin);
    ~GraphClass();
    void show(){ window->show(); }

    string arduinoName(){ return arduino_name; }

    int getPin(){ return pin; }

    void addPoint(QPointF& point);

private:
    QHBoxLayout* layout;
    IncrementalPlot* plot;
    GraphWidget* window;
    string arduino_name;
    int pin;
    GraphClassScaleDraw* time_convert;

signals:
    void windowClosed();
};

struct GraphClassSort{
    bool operator()(GraphClass* one, GraphClass* two){
        return one->arduinoName().compare(two->arduinoName()) < 0;
    }
};

#endif // GRAPHCLASS_H
