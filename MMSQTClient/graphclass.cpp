#include "graphclass.h"
#include <QScrollBar>

/*GraphWidget::GraphWidget( QWidget * parent = 0, Qt::WindowFlags f = 0 ) : QWidget(parent, f){
}*/

QwtText GraphClassScaleDraw::label( double value ) const
{
    int int_time = (int)value;
    time_t time = (time_t)int_time;
    //char* str_time = ctime(&time);
    //return QString::fromLocal8Bit(str_time);
    struct tm* time_struct = localtime(&time);
    char* str_time = new char[256];
    sprintf(str_time, "%i/%i/%i %i:%.2i:%.2i", time_struct->tm_mon+1, time_struct->tm_mday, time_struct->tm_year+1900, time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec);
    QString q_string = QString::fromLocal8Bit(str_time);
    delete[] str_time;
    return q_string;
    //return QLocale().toString( value );
}

void GraphWidget::closeEvent(QCloseEvent* event){
    emit closing();
    event->accept();
}

GraphClass::GraphClass(string arduino_name, int pin)
{
    window = new GraphWidget();
    connect(window, SIGNAL(closing()), this, SIGNAL(windowClosed()));
    window->setWindowTitle(QString::fromLocal8Bit(arduino_name.c_str()));
    layout = new QHBoxLayout( window );
    layout->setContentsMargins( 0, 0, 0, 0 );
    //layout->addWidget( plot );
    //RandomPlot* plot = new RandomPlot(window);
    plot = new IncrementalPlot(window);
    QString str = QString::fromLocal8Bit(arduino_name.c_str());
    str += QString(", pin ");
    str += QString::number(pin);
    plot->setTitle(str);
    plot->setAxisTitle(QwtPlot::xBottom, QString("Time (seconds since Jan 1, 1970)"));
    plot->setAxisTitle(QwtPlot::yLeft, QString("Voltage (V)"));
    time_convert = new GraphClassScaleDraw();
    plot->setAxisScaleDraw(QwtPlot::xBottom, time_convert);
    QFont font = plot->axisFont(QwtPlot::xBottom);
    font.setPointSize(8);
    plot->setAxisFont(QwtPlot::xBottom, font);
    layout->addWidget(plot);

    window->resize(600,400);
    //window->show();
    this->arduino_name = arduino_name;
    this->pin = pin;
}

void GraphClass::addPoint(QPointF& point){
    plot->appendPoint(point);
}

GraphClass::~GraphClass(){
    //delete window;
    //delete layout;
    delete plot;
}
