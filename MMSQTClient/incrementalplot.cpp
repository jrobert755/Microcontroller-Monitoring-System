#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_directpainter.h>
#include "incrementalplot.h"
#include <qpaintengine.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_grid.h>

class MyZoomer: public QwtPlotZoomer
{
public:
    MyZoomer(QwtPlotCanvas *canvas):
        QwtPlotZoomer(canvas)
    {
        setTrackerMode(AlwaysOn);
    }

    virtual QwtText trackerTextF(const QPointF &pos) const
    {
        QColor bg(Qt::white);
        bg.setAlpha(200);

        QwtText text = QwtPlotZoomer::trackerTextF(pos);
        text.setBackgroundBrush( QBrush( bg ));
        return text;
    }
};

class CurveData: public QwtArraySeriesData<QPointF>
{
public:
    CurveData()
    {
    }

    virtual QRectF boundingRect() const
    {
        if ( d_boundingRect.width() < 0.0 )
            d_boundingRect = qwtBoundingRect( *this );

        return d_boundingRect;
    }

    inline void append( const QPointF &point )
    {
        d_samples += point;
    }

    void clear()
    {
        d_samples.clear();
        d_samples.squeeze();
        d_boundingRect = QRectF( 0.0, 0.0, -1.0, -1.0 );
    }
};

IncrementalPlot::IncrementalPlot(QWidget *parent): 
    QwtPlot(parent),
    d_curve(NULL)
{
    d_directPainter = new QwtPlotDirectPainter(this);

#if defined(Q_WS_X11)
    canvas()->setAttribute(Qt::WA_PaintOutsidePaintEvent, true);
    canvas()->setAttribute(Qt::WA_PaintOnScreen, true);
#endif

    d_curve = new QwtPlotCurve("Test Curve");
    d_curve->setStyle(QwtPlotCurve::NoCurve);
    d_curve->setData( new CurveData() );

    d_curve->setSymbol(new QwtSymbol(QwtSymbol::XCross,
        Qt::NoBrush, QPen(Qt::white), QSize( 4, 4 ) ) );

    d_curve->attach(this);

    setAutoReplot(false);

    //RandomPlot
    setFrameStyle(QFrame::NoFrame);
    setLineWidth(0);
    setCanvasLineWidth(2);

    plotLayout()->setAlignCanvasToScales(true);

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
    grid->attach(this);

    setCanvasBackground(QColor(29, 100, 141)); // nice blue

    setAxisScale(xBottom, 0, 1);
    //setAxisAutoScale(xBottom, true);
    setAxisScale(yLeft, 0, 5);
    //axisAutoScale(xBottom);
    //axisAutoScale(yLeft);
    setAxisMaxMajor(xBottom, 6);

    replot();
    x_lowest = -1;

    zoomer = new MyZoomer(canvas());
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,
        Qt::RightButton);
}

IncrementalPlot::~IncrementalPlot()
{
    delete d_curve;
}

void IncrementalPlot::appendPoint( const QPointF &point )
{
    CurveData *data = static_cast<CurveData *>( d_curve->data() );
    data->append(point);

    const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
    if ( doClip )
    {
        /*
           Depending on the platform setting a clip might be an important
           performance issue. F.e. for Qt Embedded this reduces the
           part of the backing store that has to be copied out - maybe
           to an unaccelerated frame buffer device.
         */
        const QwtScaleMap xMap = canvasMap( d_curve->xAxis() );
        const QwtScaleMap yMap = canvasMap( d_curve->yAxis() );

        QRegion clipRegion;

        const QSize symbolSize = d_curve->symbol()->size();
        QRect r( 0, 0, symbolSize.width() + 2, symbolSize.height() + 2 );

        const QPointF center = 
            QwtScaleMap::transform( xMap, yMap, point );
        r.moveCenter( center.toPoint() );
        clipRegion += r;

        d_directPainter->setClipRegion( clipRegion );
    }

    if(x_lowest == -1) x_lowest = point.x() - 5;
    x_highest = point.x() + 5;

    //if(axisScaleDiv(xBottom)->lowerBound() == 0.0) setAxisScale(xBottom, point.x()-5, point.x()+5);
    //else setAxisScale(xBottom, axisScaleDiv(xBottom)->lowerBound(), point.x() + 5);
    setAxisScale(xBottom, x_lowest, x_highest);
    setAxisScale(yLeft, 0, 5);
    updateAxes();
    
    d_directPainter->drawSeries(d_curve, 
        data->size() - 1, data->size() - 1);

    replot();

    zoomer->setZoomBase();

    //if(axisScaleDiv(xBottom)->lowerBound() == 0.0) setAxisScale(xBottom, point.x()-5, point.x()+5);
    //else setAxisScale(xBottom, axisScaleDiv(xBottom)->lowerBound(), point.x() + 5);
    setAxisScale(xBottom, x_lowest, x_highest);
    setAxisScale(yLeft, 0, 5);
    updateAxes();
    replot();
}

void IncrementalPlot::clearPoints()
{
    CurveData *data = static_cast<CurveData *>( d_curve->data() );
    data->clear();

    replot();
}
