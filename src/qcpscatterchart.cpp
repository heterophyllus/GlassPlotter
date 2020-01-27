/*****************************************************************************
 **                                                                         **
 **  This file is part of GlassPlotter.                                     **
 **                                                                         **
 **  GlassPlotter is free software: you can redistribute it and/or modify   **
 **  it under the terms of the GNU General Public License as published by   **
 **  the Free Software Foundation, either version 3 of the License, or      **
 **  (at your option) any later version.                                    **
 **                                                                         **
 **  GlassPlotter is distributed in the hope that it will be useful,        **
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of         **
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          **
 **  GNU General Public License for more details.                           **
 **                                                                         **
 **  You should have received a copy of the GNU General Public License      **
 **  along with GlassPlotter.  If not, see <http://www.gnu.org/licenses/>.  **
 **                                                                         **
 *****************************************************************************
 **  Author  : Hiiragi                                                      **
 **  Contact : heterophyllus.work@gmail.com                                 **
 **  Website : https://github.com/heterophyllus/glassplotter                **
 **  Date    : 2020-1-25                                                    **
 *****************************************************************************/


#include "qcpscatterchart.h"

QCPScatterChart::QCPScatterChart(QCustomPlot *customPlot)
{  
    _customPlot = customPlot;
    _textlabels.clear();
}

QCPScatterChart::~QCPScatterChart()
{
    _customPlot->removeGraph(_pointSeries);
    for(int i=0; i < _textlabels.size();i++)
    {
        _customPlot->removeItem(_textlabels[i]);
    }

    _pointSeries = NULL;
    _textlabels.clear();
    _customPlot = NULL;
}

QCPScatterChart::QCPScatterChart(QCPScatterChart &other)
{
    _customPlot = other.parentPlot();
    _pointSeries = other.pointSeries();
    _pointCount = other.textLabels().count();
    _textlabels = other.textLabels();
}


void QCPScatterChart::setName(QString name)
{
    _pointSeries->setName(name);
}

void QCPScatterChart::setColor(QColor color)
{
    QPen pen;
    pen.setColor(color);

    _pointSeries->setPen(pen);
}

QCustomPlot* QCPScatterChart::parentPlot()
{
    return _customPlot;
}

QCPGraph* QCPScatterChart::pointSeries()
{
    return _pointSeries;
}


QList<QCPItemText*> QCPScatterChart::textLabels()
{
    return _textlabels;
}

QString QCPScatterChart::name()
{
    return _pointSeries->name();
}

void QCPScatterChart::setData(QVector<double> x, QVector<double> y, QVector<QString> str)
{   
    //set data to points
    _pointSeries = _customPlot->addGraph();
    _pointSeries->setData(x,y);
    _pointSeries->setLineStyle(QCPGraph::lsNone);
    _pointSeries->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,8));
    _pointSeries->setVisible(false);

    //set text to labels
    _textlabels.clear();
    QCPItemText *label;
    for(int i = 0;i< str.size(); i++){
        label = new QCPItemText(_customPlot);
        _textlabels.append(label);
        _textlabels.last()->position->setCoords(x[i],y[i]);
        _textlabels.last()->setPositionAlignment(Qt::AlignRight|Qt::AlignBottom);
        _textlabels.last()->setText(str[i]);
        _textlabels[i]->setVisible(false);
    }
}

void QCPScatterChart::setVisiblePointSeries(bool state)
{
    if(_customPlot->hasPlottable(_pointSeries)){
        _pointSeries->setVisible(state);
    }
}

void QCPScatterChart::setVisibleTextLabels(bool state)
{
    for(int i = 0; i < _textlabels.size(); i++)
    {
        _textlabels[i]->setVisible(state);
    }
}
