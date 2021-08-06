#ifndef PROPERTYPLOTFORM_H
#define PROPERTYPLOTFORM_H

#include <QWidget>
#include <QTableWidget>

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
    PropertyPlotForm(QWidget *parent = nullptr);

protected slots:

    /** Add new graph. This function is designed to be used as slot for "Add" button. */
    virtual void addGraph() = 0;

    /** Delete selected graph. This function is designed to be used as slot for "Delete" button. */
    virtual void deleteGraph() = 0;

    /** Replot all graphs and set data to the table */
    virtual void updateAll() = 0;

    /** Clear graphs and table data */
    virtual void clearAll() = 0;

    /** Set axis using current input values in UI.  This can be slot for "Replot" button. */
    void setAxis();

    /** Set default axes.  This can be slot for "Reset" button. */
    void setDefault();

    /** Set legend visible on/off.  This can be slot for the checkbox. */
    void setLegendVisible();

    /** Show context menu at right click on the plot */
    void showContextMenu();

    /** Export current plot to image file */
    void exportImage();

protected:
    /** Add data to the specified cell in plot data table */
    void addTableItem(int row, int col, QString str);

    /** Get color at specified position in calormap */
    QColor getColorFromIndex(int index, int maxIndex=5);

    /** Get scaled vector */
    QVector<double> scaleVector(QVector<double> v, double scale);

    /** Get QVector<double> within the range */
    QVector<double> getVectorFromRange(QCPRange range, double step=5.0);

    /** Plotteing widget */
    QCustomPlot*  m_customPlot;

    /** Plota data table widget */
    QTableWidget* m_plotDataTable;

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

    /** Graph count limit.  This value is used to set colors to graphs. */
    int m_maxGraphCount;

};

#endif // PROPERTYPLOTFORM_H
