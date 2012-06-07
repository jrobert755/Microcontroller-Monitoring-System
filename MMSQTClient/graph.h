#include <qapplication.h>
#include <qlayout.h>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_series_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_symbol.h>
#include <qwt_text.h>
#include <qwt_math.h>
#include <math.h>

//-----------------------------------------------------------------
//              simple.cpp
//
//      A simple example which shows how to use QwtPlot connected
//      to a data class without any storage, calculating each values
//      on the fly.
//-----------------------------------------------------------------

class FunctionData: public QwtSyntheticPointData
{
public:
    FunctionData(double(*y)(double)):
        QwtSyntheticPointData(100),
        d_y(y)
    {
    }

    virtual double y(double x) const
    {
        return d_y(x);
    }

private:
    double(*d_y)(double);
};

class Plot : public QwtPlot
{
public:
    Plot( QWidget *parent = NULL);

protected:
    virtual void resizeEvent( QResizeEvent * );

private:
    void populate();
    void updateGradient();
};


Plot::Plot(QWidget *parent):
    QwtPlot( parent )
{
    setAutoFillBackground( true );
    setPalette( QPalette( QColor( 165, 193, 228 ) ) );
    updateGradient();

    setTitle("A Simple QwtPlot Demonstration");
    insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    // axes
    setAxisTitle(xBottom, "x -->" );
    setAxisScale(xBottom, 0.0, 10.0);

    setAxisTitle(yLeft, "y -->");
    setAxisScale(yLeft, -5.0, 5.0);

    // canvas
    canvas()->setLineWidth( 1 );
    canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas()->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas()->setPalette( canvasPalette );

    setAutoReplot(false);

    populate();
}

void Plot::populate()
{
    // Insert new curves
    QwtPlotCurve *cSin = new QwtPlotCurve("y = sin(x)");
    cSin->setRenderHint(QwtPlotItem::RenderAntialiased);
    cSin->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    cSin->setPen(QPen(Qt::red));
    cSin->attach(this);

    double* x = new double[11];
    double* y1 = new double[11];
    double* y2 = new double[11];

    for(int i = 0; i <= 10; i++){
        y1[i] = i-5;
        y2[i] = 5-i;
        x[i] = i;
    }

    QwtPlotCurve *cCos = new QwtPlotCurve("y = cos(x)");
    cCos->setStyle(QwtPlotCurve::NoCurve);
    cCos->setSymbol(new QwtSymbol(QwtSymbol::XCross,
        Qt::NoBrush, QPen(Qt::white), QSize( 4, 4 ) ) );

    cCos->attach(this);

    cSin->setRawSamples(x, y2, 11);
    cCos->setRawSamples(x, y1, 11);

    // Insert markers

    //  ...a horizontal line at y = 0...
    QwtPlotMarker *mY = new QwtPlotMarker();
    mY->setLabel(QString::fromLatin1("y = 0"));
    mY->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    mY->setLineStyle(QwtPlotMarker::HLine);
    mY->setYValue(0.0);
    mY->attach(this);
}

void Plot::updateGradient()
{
    QPalette pal = palette();

    const QColor buttonColor = pal.color( QPalette::Button );

#ifdef Q_WS_X11
    // Qt 4.7.1: QGradient::StretchToDeviceMode is buggy on X11

    QLinearGradient gradient( rect().topLeft(), rect().bottomLeft() );
    gradient.setColorAt( 0.0, Qt::white );
    gradient.setColorAt( 0.7, buttonColor );
    gradient.setColorAt( 1.0, buttonColor );
#else
    QLinearGradient gradient( 0, 0, 0, 1 );
    gradient.setCoordinateMode( QGradient::StretchToDeviceMode );
    gradient.setColorAt( 0.0, Qt::white );
    gradient.setColorAt( 0.7, buttonColor );
    gradient.setColorAt( 1.0, buttonColor );
#endif

    pal.setBrush( QPalette::Window, gradient );
    setPalette( pal );
}

void Plot::resizeEvent( QResizeEvent *event )
{
    QwtPlot::resizeEvent( event );
#ifdef Q_WS_X11
    updateGradient();
#endif
}
