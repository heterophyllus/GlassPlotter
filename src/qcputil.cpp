#include "qcputil.h"


QColor QCPUtil::getColorFromIndex(int index, int maxIndex)
{
    QCPColorGradient colorgrad;
    colorgrad.loadPreset(QCPColorGradient::gpHues);
    QColor color = colorgrad.color(index, QCPRange(0, maxIndex));

    return color;
}

QVector<double> QCPUtil::scaleVector(QVector<double> v, double scale)
{
    int len = v.size();
    QVector<double> sv(len);

    for(int i = 0; i < len; i++){
        sv[i] = v[i]*scale;
    }
    return sv;
}

QVector<double> QCPUtil::getVectorFromRange(QCPRange range, double step)
{
    double xmin = range.lower;
    double xmax = range.upper;
    QVector<double> xdata;
    xdata.reserve(floor((xmax - xmin)/step) + 1 );
    double x = xmin;
    while(x <= xmax)
    {
        xdata.append(x);
        x += step;
    }
    return xdata;
}

void QCPUtil::setColorToGraph(QCPGraph *graph, QColor color)
{
    QPen pen = graph->pen();
    pen.setWidth(2);
    pen.setColor(color);
    graph->setPen(pen);
}

