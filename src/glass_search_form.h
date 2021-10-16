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
 **  Date    : 2020-5-20                                                    **
 *****************************************************************************/

#ifndef GLASS_SEARCH_FORM_H
#define GLASS_SEARCH_FORM_H

#include <QWidget>
#include <QList>
#include <QMdiArea>
#include <QTableWidget>
#include <QComboBox>

#include "glass.h"

namespace Ui {
class GlassSearchForm;
}

/** Form for glass search */
class GlassSearchForm : public QWidget
{
    Q_OBJECT

public:
    explicit GlassSearchForm(QMdiArea *parent = nullptr);
    ~GlassSearchForm();

private slots:
    /** Execute search and show result */
    void showSearchResult();

    /** Add new line to search parameter table */
    void addParameter();

    /** Remove line from search parameter table */
    void removeParameter();

    /** Validate cell input value of search parameter table */
    void validateCellInput(int row, int col);

private:
    /** Compute error function value */
    double getErrorValue(const Glass* g);
    QComboBox* createParameterCombo();
    void setCellValue(QTableWidget* table, int row, int col, QString str);

    inline QString numToQString(double val, char fmt='f', int digit=6);

    Ui::GlassSearchForm *ui;
    QMdiArea*     m_parentMdiArea;
    QTableWidget* m_tableProperties;
    QTableWidget* m_tableResult;
};


QString GlassSearchForm::numToQString(double val, char fmt, int digit)
{
    if(qIsNaN(val)){
        return "-";
    }
    else{
        return QString::number(val,fmt,digit);
    }
}

#endif // GLASS_SEARCH_FORM_H
