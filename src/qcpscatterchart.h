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

/**
  * Class for scatter chart using QCustomPlot
  */

#ifndef QCPSCATTERCHART_H
#define QCPSCATTERCHART_H

#include "qcustomplot.h"

class QCPScatterChart
{

public:
    QCPScatterChart(QCustomPlot *customPlot);
    ~QCPScatterChart();
    QCPScatterChart(QCPScatterChart &other);

    /**
     * @brief parent QCustomPlot
     * @return
     */
    QCustomPlot* parentPlot();

    /**
     * @brief get QCPGraph that expresses scatter points
     * @return
     */
    QCPGraph* graphPoints();

    /**
     * @brief get QCPItemText list of glass name labels
     * @return
     */
    QList<QCPItemText*> textLabels();

    /**
     * @brief chart name
     * @return
     */
    QString name();

    /**
     * @brief set data to scatter chart
     * @param x x coordinates of the point
     * @param y y coordinates of the point
     * @param str label text for every point
     */
    void setData(QVector<double> x, QVector<double> y, QVector<QString> str);
    void setName(QString name);
    void setColor(QColor color);
    void setVisiblePointSeries(bool state);
    void setVisibleTextLabels(bool state);

    void setAxis(QCPRange xrange, QCPRange yrange);

private:
    int m_pointCount;
    QCustomPlot* m_customPlot;
    QCPGraph* m_graphPoints; //points
    QList<QCPItemText*> m_textlabels; // text label for each point
};

#endif // QCPSCATTERCHART_H
