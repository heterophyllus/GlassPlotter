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
  * Qt Form Class for Glassmap
  */

#ifndef GLASSMAPFORM_H
#define GLASSMAPFORM_H

#include <QWidget>

#include "qcputil.h"
#include "qcpscatterchart.h"
#include "glasscatalog.h"
#include "glassdatasheetform.h"
#include "curvefittingdialog.h"
#include "presetdialog.h"

namespace Ui {
class GlassMapForm;
}


class GlassMapForm : public QWidget
{
    Q_OBJECT

public:
    explicit GlassMapForm(QList<GlassCatalog*> catalogList, QString xdataname, QString ydataname, QCPRange xrange, QCPRange yrange, QMdiArea *parent = nullptr);
    ~GlassMapForm();

    /**
     * @brief The inner class to manage glass map of the respective supplyer
     */
    class GlassMapCtrl
    {
    public:
        GlassMapCtrl(QCustomPlot* customPlot);
        ~GlassMapCtrl();
        QCPScatterChart* glassmap;
        QLabel* labelSupplyer;
        QCheckBox* checkBoxPlot;
        QCheckBox* checkBoxLabel;
    private:
        QCustomPlot* m_customPlot;
    };

    void setGlassmapData(QCPScatterChart* glassmap,GlassCatalog* catalog, QString xlabel, QString ylabel, QColor color);
    void setUpScrollArea();
    void setDefault();
    void saveSetting();

    QCPGraph* curveGraph;
    QCheckBox* checkBoxCurve;
    QList<double> getCurveCoefs();
    void setCurveCoefsToUI(QList<double> coefs);
    void setCurveData();

private slots:

    /**
     * @brief set legend visible
     * @name SLOT
     */
    void setLegendVisible();

    /**
     * @brief show CurveFittingDlg
     * @name SLOT
     */
    void showCurveFittingDlg();

    /**
     * @brief show neighboring glass list
     * @param item selected text item on the glassmap
     * @param event mouse event
     * @name SLOT
     */
    void showNeighbors(QCPAbstractItem* item, QMouseEvent *event);

    /**
     * @brief clear neighboring glass list
     * @param event mouse event
     * @name SLOT
     */
    void clearNeighbors(QMouseEvent* event);

    /**
     * @brief show GlassDataSheet form
     * @name SLOT
     */
    void showGlassDataSheet();

    void update();

    /**
     * @brief Reset axis of the glassmap
     * @name SLOT
     */
    void resetView();

    void showPresetDlg();

private:
    Ui::GlassMapForm *ui;

    QMdiArea* m_parentMdiArea;
    QCustomPlot* m_customPlot;

    QCheckBox* m_checkBoxLegend;

    QList<GlassCatalog*> m_catalogList;
    QList<GlassMapCtrl*> m_glassMapCtrlList;

    QListWidget* m_listWidgetNeighbors;

    const double m_neighborThreshold = 0.5;


    Glass* getGlassFromName(QString glassName);

    QSettings* m_settings;
    QString m_settingFile;

    QString m_xDataName;
    QString m_yDataName;

    QCPRange m_defaultXrange;
    QCPRange m_defaultYrange;

};

#endif // GLASSMAPFORM_H
