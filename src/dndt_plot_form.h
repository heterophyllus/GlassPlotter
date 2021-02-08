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

#ifndef DNDT_PLOT_FORM_H
#define DNDT_PLOT_FORM_H

#include <QWidget>
#include "qcustomplot.h"
#include "qcputil.h"
#include "glasscatalog.h"
#include "glass_selection_dialog.h"

namespace Ui {
class DnDtPlotForm;
}

class DnDtPlotForm : public QWidget
{
    Q_OBJECT

public:
    explicit DnDtPlotForm(QList<GlassCatalog*> catalogList, QWidget *parent = nullptr);
    ~DnDtPlotForm();

private slots:
    void setGlass();
    void addGraph();
    void deleteGraph();
    void setAxis();
    void setLegendVisible();
    void clearAll();
    void updateAll();


private:
    Ui::DnDtPlotForm *ui;

    QList<GlassCatalog*> m_catalogList;
    QList<double> m_wvlList;

    Glass* m_currentGlass = nullptr;
    QCustomPlot* m_customPlot;
    QTableWidget* m_table;

    const int m_maxGraphCount = 7;
    const double m_plotStep = 5;

    void setColorToGraph(QCPGraph* graph, QColor color);
    void setDefault();
};

#endif // DNDT_PLOT_FORM_H
