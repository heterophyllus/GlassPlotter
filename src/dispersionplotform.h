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
  * Form class for dispersion plot
  *
  */


#ifndef DISPERSIONPLOTFORM_H
#define DISPERSIONPLOTFORM_H

#include <QWidget>

#include "qcustomplot.h"
#include "glasscatalog.h"

#include "glassselectiondialog.h"

namespace Ui {
class DispersionPlotForm;
}

class DispersionPlotForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief DispersionPlotForm
     * @param catalogList Glass catalig list
     * @param parent parent QWidget MDI area
     */
    explicit DispersionPlotForm(QList<GlassCatalog*> catalogList, QWidget *parent = nullptr);
    ~DispersionPlotForm();

    /**
     * @brief The inner class to manage user defined curve
     */
    class CurveCtrl
    {
    public:
        CurveCtrl(DispersionPlotForm *super);
        ~CurveCtrl();
        QCPGraph* graph;
        QVector<double> xdata;
        QVector<double> ydata;
        QList<double> coefs;
        void setData();
        void setVisible(bool state);

    private:
        DispersionPlotForm *m_super;
    };

    CurveCtrl* m_curveCtrl;

    const double plotStep = 5;
    const int maxGraphCount = 5;

private slots:
    void addGraph();
    void deleteGraph();
    void setAxis();
    void setLegendVisible();
    void updateAll();
    void clearAll();

private:
    class PlottedGraph
    {
    public:
        PlottedGraph(DispersionPlotForm *super);
        ~PlottedGraph();
        QString name;
        Glass* glass;
        QCPGraph* graph;
        QVector<double> xdata;
        QVector<double> ydata;

        void setData(QCPRange xrange);
        void setColor(int index);
    private:
        DispersionPlotForm *m_super;
    };

    QList<PlottedGraph*> m_plottedGraphList;

    Ui::DispersionPlotForm *ui;

    QList<GlassCatalog*> m_catalogList;

    QCustomPlot* m_customPlot;

    QCPRange m_xrange;
    QCPRange m_yrange;

    QCheckBox* m_checkBox;
    QTableWidget* m_table;



private:

    void setDefault();


};

#endif // DISPERSIONPLOTFORM_H
