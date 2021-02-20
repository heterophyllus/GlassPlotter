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
 **  Date    : 2020-5-25                                                    **
 *****************************************************************************/


#ifndef CURVE_FITTING_DIALOG_H
#define CURVE_FITTING_DIALOG_H

#include <QDialog>
#include <QList>

class QListWidget;
class QComboBox;
class Glass;
class GlassCatalog;

namespace Ui {
class CurveFittingDialog;
}

class CurveFittingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CurveFittingDialog(QList<GlassCatalog*> catalogList, QWidget *parent = nullptr);
    ~CurveFittingDialog();


    /**
     * @brief Calculate coefficients of fitting curve. Called from the parent Glassmapform.
     * @param plotType
     * @return success/failed
     * @note coefficients data are stored in m_fittingResult;
     */
    bool calculateFitting(QString xdataname, QString ydataname);

    /**
     * @brief get fitting result
     * @return fitting result
     */
    QList<double> fittingResult();

private slots:
    void addGlass();
    void deleteSelectedGlass();

private:
    Ui::CurveFittingDialog *ui;

    QList<double> m_fittingResult;
    QList<GlassCatalog*> m_catalogList;
    QList<Glass*> m_targetGlassList;

    QComboBox* m_comboBoxOrder;
    QListWidget* m_listWidget;

    const int m_maxFittingOrder = 3;

    /**
     * @brief update m_targetGlassList from m_listWidget
     */
    void updateGlassList();
};

#endif // CURVE_FITTING_DIALOG_H
