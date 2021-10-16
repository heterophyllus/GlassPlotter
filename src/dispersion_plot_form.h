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


#ifndef DISPERSION_PLOT_FORM_H
#define DISPERSION_PLOT_FORM_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <QListWidget>

#include "glass.h"
#include "property_plot_form.h"

namespace Ui {
class DispersionPlotForm;
}


class DispersionPlotForm : public PropertyPlotForm
{
    Q_OBJECT

public:
    explicit DispersionPlotForm(QWidget *parent = nullptr);
    ~DispersionPlotForm();

private slots:
    void on_comboBoxChanged();
    void addGraph() override;
    void deleteGraph() override;
    void updateAll() override;
    void clearAll() override;

private:
    Ui::DispersionPlotForm *ui;

    QComboBox*    m_comboBoxFormula;
    QCheckBox*    m_chkCurve;
    QTableWidget* m_tableCoefs;

    QList<Glass*>  m_glassList;

    QVector<double> computeUserDefinedCurve(const QVector<double>& xdata);

};

#endif // DISPERSION_PLOT_FORM_H
