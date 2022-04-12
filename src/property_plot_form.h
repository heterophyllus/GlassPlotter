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

#ifndef PROPERTYPLOTFORM_H
#define PROPERTYPLOTFORM_H

#include "qcustomtablewidget.h"
#include "qcustomplot.h"

namespace Ui {
class PropertyPlotForm;
}

/**
 * @brief Base class for glass property plot form
 * @header property_plot_form
 *
 * This class provides default implementation for glass property plot form.
 */

class PropertyPlotForm : public QWidget
{
    Q_OBJECT

public:
    enum ButtonLabel{
        AddGraph,
        DeleteGraph,
        SetAxis,
        Clear
    };

public:
    PropertyPlotForm(QWidget *parent = nullptr);

protected slots:

    /** Add a new graph. Slot for "Add" button. */
    virtual void addGraph() = 0;

    /** Delete the selected graph. Slot for "Delete" button. */
    virtual void deleteGraph() = 0;

    /** Replot all graphs and set data to the table */
    virtual void updateAll() = 0;

    /** Clear all graphs and table data */
    virtual void clearAll() = 0;

    /** Set axis range using current input values in UI.  Slot for "Replot" button. */
    void setAxis();

    /** Set default axes.  Slot for "Reset" button. */
    void setDefault();

    /** Set the legend visible on/off.  Slot for the checkbox. */
    void setLegendVisible();

    /** Show a context menu in case of right click on the plot */
    void showContextMenuOnPlot();

    void showContextMenuOnTable();

    void setupFundamentalUi(const QList<QPushButton*>& buttons, QCheckBox* chkLegend);

    /** Export the current plot to image file */
    void exportImage();

    /** Export the current data table to csv file */
    void exportCSV();

protected:
    /** Add data to the specified cell in plot data table */
    void setValueToCell(int row, int col, double val, int digit= 1);

    /** Get color at specified position in calormap */
    QColor getColorFromIndex(int index, int maxIndex=5);

    /** Get scaled vector */
    QVector<double> scaleVector(const QVector<double>& v, double scale);

    /** Get QVector<double> within the range */
    QVector<double> getVectorFromRange(QCPRange range, double step=5.0);


    /** Plotting widget */
    QCustomPlot*  m_customPlot;

    /** Plota data table widget */
    QCustomTableWidget* m_plotDataTable;

    /** Line edit widget (x min) */
    QLineEdit* m_editXmin;

    /** Line edit widget (x max) */
    QLineEdit* m_editXmax;

    /** Line edit widget (y min) */
    QLineEdit* m_editYmin;

    /** Line edit widget (y max) */
    QLineEdit* m_editYmax;

    /** default axis range */
    QCPRange m_defaultXrange;
    QCPRange m_defaultYrange;

    /** data step */
    QLineEdit* m_editPlotStep;

    /** Legend on/off checkbox */
    QCheckBox* m_chkLegend;

    bool m_draggingLegend;
    QPointF m_dragLegendOrigin;

    /** Graph count limit.  This value is used to set colors to graphs. */
    int m_maxGraphCount;

protected slots:
    void mouseMoveSignal(QMouseEvent *event);
    void mousePressSignal(QMouseEvent *event);
    void mouseReleaseSignal(QMouseEvent *event);
    void beforeReplot();

};

#endif // PROPERTYPLOTFORM_H
