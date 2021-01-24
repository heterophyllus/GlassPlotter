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
    m_customPlot = customPlot;
    m_graphPoints = m_customPlot->addGraph();
    m_textlabels.clear();
}

QCPScatterChart::~QCPScatterChart()
{
    m_customPlot->removeGraph(m_graphPoints);
    m_graphPoints = nullptr;

    for(int i = 0; i < m_textlabels.size(); i++){
        m_customPlot->removeItem(m_textlabels[i]);
    }
    m_textlabels.clear();
    m_customPlot = nullptr;
}

QCPScatterChart::QCPScatterChart(QCPScatterChart &other)
{
    m_customPlot = other.parentPlot();
    m_graphPoints = other.graphPoints();
    m_pointCount = other.textLabels().count();
    m_textlabels = other.textLabels();
}

void QCPScatterChart::setName(QString name)
{
    m_graphPoints->setName(name);
}

void QCPScatterChart::setColor(QColor color)
{
    QPen pen;
    pen.setColor(color);
    m_graphPoints->setPen(pen);
}

QCustomPlot* QCPScatterChart::parentPlot()
{
    return m_customPlot;
}

QCPGraph* QCPScatterChart::graphPoints()
{
    return m_graphPoints;
}

QList<QCPItemText*> QCPScatterChart::textLabels()
{
    return m_textlabels;
}

QString QCPScatterChart::name()
{
    return m_graphPoints->name();
}

void QCPScatterChart::setData(QVector<double> x, QVector<double> y, QVector<QString> str)
{   
    if(!m_graphPoints) return;

    //set data to points
    m_graphPoints->setData(x,y);
    m_graphPoints->setLineStyle(QCPGraph::lsNone);
    m_graphPoints->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,8));
    m_graphPoints->setVisible(false);

    //set text to labels
    m_textlabels.clear();
    QCPItemText *label;
    for(int i = 0;i< str.size(); i++){
        label = new QCPItemText(m_customPlot);
        m_textlabels.append(label);
        m_textlabels.last()->position->setCoords(x[i],y[i]);
        m_textlabels.last()->setPositionAlignment(Qt::AlignRight|Qt::AlignBottom);
        m_textlabels.last()->setText(str[i]);
        m_textlabels.last()->setObjectName(str[i]); //used for mouse click
        m_textlabels[i]->setVisible(false);
    }

}

void QCPScatterChart::setVisiblePointSeries(bool state)
{
    if(m_customPlot->hasPlottable(m_graphPoints)){
        m_graphPoints->setVisible(state);
    }
}

void QCPScatterChart::setVisibleTextLabels(bool state)
{
    for(int i = 0; i < m_textlabels.size(); i++){
        m_textlabels[i]->setVisible(state);
    }
}
