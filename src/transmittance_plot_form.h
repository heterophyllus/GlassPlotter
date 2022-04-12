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


#ifndef TRANSMITTANCE_PLOT_FORM_H
#define TRANSMITTANCE_PLOT_FORM_H

#include <QWidget>

#include "property_plot_form.h"
#include "glass.h"

namespace Ui {
class TransmittancePlotForm;
}


class TransmittancePlotForm : public PropertyPlotForm
{
    Q_OBJECT

public:
    explicit TransmittancePlotForm(QWidget *parent = nullptr);
    ~TransmittancePlotForm();

private slots:
    void addGraph() override;
    void deleteGraph() override;
    void clearAll() override;
    void updateAll() override;

private:
    Ui::TransmittancePlotForm *ui;

    QList<Glass*>  m_glassList;

    QLineEdit* m_editThickness;

};

#endif // TRANSMITTANCE_PLOT_FORM_H
