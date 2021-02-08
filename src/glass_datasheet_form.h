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
  * Qt Form Class for Glass Datasheet
  */

#ifndef GLASS_DATASHEET_FORM_H
#define GLASS_DATASHEET_FORM_H

#include <QWidget>

namespace Ui {
class GlassDataSheetForm;
}

class Glass;

class GlassDataSheetForm : public QWidget
{
    Q_OBJECT

public:
    explicit GlassDataSheetForm(Glass* glass, QWidget *parent = nullptr);
    ~GlassDataSheetForm();

private:
    Ui::GlassDataSheetForm *ui;
    Glass* m_glass;

    /**
     * @brief set up Basic Tab
     */
    void setUpBasicTab();

    /**
     * @brief set up Indices Tab
     */
    void setUpIndicesTab();

    /**
     * @brief set up Partial Tab
     */
    void setUpPartialTab();

    /**
     * @brief set up Dispersion Tab
     */
    void setUpDispersionTab();

    /**
     * @brief set up Thermal Tab
     */
    void setUpThermalTab();
};

#endif // GLASS_DATASHEET_FORM_H
