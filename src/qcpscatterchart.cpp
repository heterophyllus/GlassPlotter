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
    // delete graph
    try {
        m_customPlot->removeGraph(m_graphPoints);
    }  catch (...) {
        m_graphPoints = nullptr;
    }
    m_graphPoints = nullptr;

    // delete text labels
    int labelCount = m_textlabels.size();
    for(int i = 0; i < labelCount; i++){
        try {
            m_customPlot->removeItem(m_textlabels[i]);
        }  catch (...) {
            m_textlabels[i] = nullptr;
        }
        m_textlabels[i] = nullptr;
    }
    m_textlabels.clear();

    m_customPlot = nullptr;
}

QCPScatterChart::QCPScatterChart(QCPScatterChart &other)
{
    m_customPlot  = other.parentPlot();
    m_graphPoints = other.graphPoints();
    m_textlabels  = other.textLabels();
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
    //set data to points
    m_graphPoints->setData(x,y);
    m_graphPoints->setLineStyle(QCPGraph::lsNone);
    m_graphPoints->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,8));


    //set text to labels
    if(!m_textlabels.isEmpty())
    {
        int labelCount = m_textlabels.size();
        for(int i = 0; i < labelCount; i++){
            try {
                m_customPlot->removeItem(m_textlabels[i]);
            }  catch (...) {
                m_textlabels[i] = nullptr;
            }
            m_textlabels[i] = nullptr;
        }
    }
    m_textlabels.clear();

    QCPItemText *label;
    int nlabels = str.size();
    for(int i = 0; i < nlabels; i++){
        label = new QCPItemText(m_customPlot);
        label->position->setCoords(x[i],y[i]);
        label->setPositionAlignment(Qt::AlignRight|Qt::AlignBottom);
        label->setText(str[i]);
        label->setObjectName(str[i]); //used for mouse click
        m_textlabels.append(label);
    }

}

void QCPScatterChart::setVisiblePointSeries(bool state)
{    
    m_graphPoints->setVisible(state);
}

void QCPScatterChart::setVisibleTextLabels(bool state)
{
    int nlabels = m_textlabels.size();
    for(int i = 0; i < nlabels; i++){
        m_textlabels[i]->setVisible(state);
    }
}

int QCPScatterChart::dataCount()
{
    return m_textlabels.size();
}


