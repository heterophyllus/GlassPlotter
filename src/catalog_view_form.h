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
#include <QComboBox>
#include <QTableWidget>

class GlassCatalog;

namespace Ui {
class CatalogViewForm;
}

class CatalogViewForm : public QWidget
{
    Q_OBJECT

public:
    explicit CatalogViewForm(QList<GlassCatalog*> catalogList, QMdiArea *parent = nullptr);
    ~CatalogViewForm();

private slots:
    void setUpTable();
    void showDatasheet();

private:
    Ui::CatalogViewForm *ui;

    QMdiArea* m_parentMdiArea;
    QTableWidget* m_table;
    QComboBox* m_comboBox;

    QList<GlassCatalog*> m_catalogList;

    void addTableItem(int row, int col, QString str);
};

#endif // CATALOG_VIEW_FORM_H
