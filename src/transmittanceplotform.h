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
  * Qt Form Class for Transmittance Plot
  *
  */


#ifndef TRANSMITTANCEPLOTFORM_H
#define TRANSMITTANCEPLOTFORM_H

#include <QWidget>

#include "qcustomplot.h"
#include "glasscatalog.h"

#include "glassselectiondialog.h"


namespace Ui {
class TransmittancePlotForm;
}

class TransmittancePlotForm : public QWidget
{
    Q_OBJECT

public:
    explicit TransmittancePlotForm(QList<GlassCatalog*> catalogList, QWidget *parent = nullptr);
    ~TransmittancePlotForm();

    void setDefault();

    double plotStep = 5;

    const int maxGraphCount = 5;

private slots:
    void addGraph();
    void deleteGraph();
    void setAxis();
    void setLegendVisible();
    void clearAll();
    void updateAll();

private:

    class PlottedGraph
    {
    public:
        PlottedGraph(TransmittancePlotForm *super);
        ~PlottedGraph();
        QString name;
        Glass* glass;
        QCPGraph* graph;
        QCPItemTracer* upperTracer;
        QCPItemTracer* lowerTracer;
        QVector<double> xdata;
        QVector<double> ydata;

        void setData(QCPRange xrange);
        void setColor(int  index);
        double m_thickness=25;
    private:
        TransmittancePlotForm* m_super;
    };

    QList<PlottedGraph*> m_plottedGraphList;
    QList<GlassCatalog*> m_catalogList;

    Ui::TransmittancePlotForm *ui;

    QCustomPlot* m_customPlot;

    QCPRange m_xrange;
    QCPRange m_yrange;

    QTableWidget* m_table;



};

#endif // TRANSMITTANCEPLOTFORM_H
