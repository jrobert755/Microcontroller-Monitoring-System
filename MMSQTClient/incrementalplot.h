#ifndef _INCREMENTALPLOT_H_
#define _INCREMENTALPLOT_H_ 1

#include <qwt_plot.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_zoomer.h>

class QwtPlotCurve;
class QwtPlotDirectPainter;

class GraphClassScaleDraw : public QwtScaleDraw{
public:
    GraphClassScaleDraw() : QwtScaleDraw() {
    }

    QwtText label( double value ) const;
};

class IncrementalPlot : public QwtPlot
{
    Q_OBJECT

public:
    IncrementalPlot(QWidget *parent = NULL);
    virtual ~IncrementalPlot();

    void appendPoint( const QPointF &);
    void clearPoints();

private:
    QwtPlotCurve *d_curve;
    QwtPlotDirectPainter *d_directPainter;
    QwtPlotZoomer* zoomer;
    double x_lowest, x_highest;

public slots:
    void clear()
    {
        clearPoints();
        replot();
    }
};

#endif // _INCREMENTALPLOT_H_
