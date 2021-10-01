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


#ifndef GLASS_DATASHEET_FORM_H
#define GLASS_DATASHEET_FORM_H

#include <QWidget>
#include <QGridLayout>

#include "glass.h"

namespace Ui {
class GlassDataSheetForm;
}


/** Qt form for glass datasheet */
class GlassDataSheetForm : public QWidget
{
    Q_OBJECT

public:
    explicit GlassDataSheetForm(Glass* glass, QWidget *parent = nullptr);
    ~GlassDataSheetForm();

private:
    Ui::GlassDataSheetForm *ui;
    Glass* m_glass;

    void setUpBasicTab();
    void setUpIndicesTab();
    void setUpPartialTab();
    void setUpDispersionTab();
    void setUpThermalTab();
    void setUpTransmittanceTab();
    void setUpOtherDataTab();

    void addGridItem(QGridLayout* gridLayout, int row, int col, QString str);

    inline QString numToQString(double val, char fmt='f', int digit=6);
};

QString GlassDataSheetForm::numToQString(double val, char fmt, int digit)
{
    if(qIsNaN(val)){
        return "-";
    }
    else{
        return QString::number(val,fmt,digit);
    }
}

#endif // GLASS_DATASHEET_FORM_H
