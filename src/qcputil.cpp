#include "qcputil.h"

QCPUtil::QCPUtil()
{

}

QColor QCPUtil::getColorFromIndex(int index, int maxIndex)
{
    QCPColorGradient colorgrad;
    colorgrad.loadPreset(QCPColorGradient::gpHues);
    QColor color = colorgrad.color(index, QCPRange(0, maxIndex));

    return color;
}

QVector<double> QCPUtil::scaleVector(QVector<double> v, double scale)
{
    QVector<double> vv;
    vv = v;

    for(int i = 0; i < v.size(); i++){
        vv[i] = v[i]*scale;
    }
    return vv;
}

QVector<double> QCPUtil::getVectorFromRange(QCPRange range, double step)
{
    QVector<double> xdata;
    double lambdamin = range.lower;
    double lambdamax = range.upper;
    double lambdanano = lambdamin;
    while(lambdanano < lambdamax)
    {
        xdata.append(lambdanano/1000);
        lambdanano += step;
    }
    return xdata;
}

