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

namespace Ui {
class GlassSearchForm;
}

class QMdiArea;
class QComboBox;
class QTableWidget;
class GlassCatalog;
class Glass;

class GlassSearchForm : public QWidget
{
    Q_OBJECT

public:
    explicit GlassSearchForm(QList<GlassCatalog*> catalogList, QMdiArea *parent = nullptr);
    ~GlassSearchForm();

private slots:
    /** Execute search and show result */
    void showSearchResult();
    void addParameter();
    void removeParameter();

private:
    bool compareErrorValue(Glass* g1, Glass* g2);
    double getErrorValue(Glass* g);
    QComboBox* createParameterCombo();
    void setCellValue(QTableWidget* table, int row, int col, QString str);
    inline QString numToQString(double val, char fmt='f', int digit=6);

    Ui::GlassSearchForm *ui;
    QMdiArea*     m_parentMdiArea;
    QTableWidget* m_tableProperties;
    QTableWidget* m_tableResult;

    QList<GlassCatalog*> m_catalogList;
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
