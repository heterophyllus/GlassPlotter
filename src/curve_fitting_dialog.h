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

class QTableWidget;
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
    explicit CurveFittingDialog(QList<GlassCatalog*> catalogList, QString xdataname, QString ydataname, QWidget *parent = nullptr);
    ~CurveFittingDialog();

    bool getFittingResult(QList<double>& result);

private slots:
    void addNewRow(QString s1="", QString s2="", QString s3="");
    void addGlassForNewRow();
    void deleteSelectedRow();

private:
    Ui::CurveFittingDialog *ui;

    QList<GlassCatalog*> m_catalogList;

    QString m_xDataName;
    QString m_yDataName;

    QComboBox*    m_comboBoxOrder;
    QTableWidget* m_table;

    const int m_maxFittingOrder = 3;

};

#endif // CURVE_FITTING_DIALOG_H
