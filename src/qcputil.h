#ifndef QCPUTIL_H
#define QCPUTIL_H

#include "qcustomplot.h"

class QCPUtil
{
public:
    static QColor getColorFromIndex(int index, int maxIndex=5);
    static QVector<double> scaleVector(QVector<double> v, double scale);
    static QVector<double> getVectorFromRange(QCPRange range, double step=5.0);
    static void setColorToGraph(QCPGraph* graph, QColor color);
};

#endif // QCPUTIL_H
