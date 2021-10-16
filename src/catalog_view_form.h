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


#ifndef CATALOG_VIEW_FORM_H
#define CATALOG_VIEW_FORM_H

#include <QWidget>
#include <QMdiArea>
#include <QList>
#include <QTableWidget>
#include <QComboBox>
#include <QtMath>

#include "glass_catalog.h"

namespace Ui {
class CatalogViewForm;
}

class CatalogViewForm : public QWidget
{
    Q_OBJECT

public:
    explicit CatalogViewForm(QMdiArea *parent = nullptr);
    ~CatalogViewForm();

private slots:
    void update();
    void setUpTable(QStringList properties,GlassCatalog* catalog, int digit=6);
    void showDatasheet();
    void showSettingDlg();

private:
    Ui::CatalogViewForm *ui;

    QMdiArea*     m_parentMdiArea;
    QTableWidget* m_table;
    QComboBox*    m_comboBox;

    QStringList m_allPropertyList;
    QStringList m_currentPropertyList;
    int         m_currentDigit;

    void addTableItem(int row, int col, QString str);
    inline QString numToQString(double val, char fmt='f', int digit=6);
};


QString CatalogViewForm::numToQString(double val, char fmt, int digit)
{
    if(qIsNaN(val)){
        return "-";
    }
    else{
        return QString::number(val,fmt,digit);
    }
}

#endif // CATALOG_VIEW_FORM_H
