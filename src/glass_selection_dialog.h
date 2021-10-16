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
  * Qt Dialog Class for glass selection
  */


#ifndef GLASS_SELECTION_DIALOG_H
#define GLASS_SELECTION_DIALOG_H

#include <QDialog>
#include <QList>
#include <QStringList>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>

#include "glass.h"

namespace Ui {
class GlassSelectionDialog;
}

class GlassSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GlassSelectionDialog(QWidget *parent = nullptr);
    ~GlassSelectionDialog();

    Glass* getSelectedGlass();

private slots:
    void updateGlassList();
    void createGlassNameList();
    void onComboChanged();

private:
    Ui::GlassSelectionDialog *ui;

    QComboBox*   m_comboBoxSupplyer;
    QLineEdit*   m_lineEditFilter;
    QListWidget* m_listWidgetGlass;

    QStringList  m_glassNameList;

};

#endif // GLASS_SELECTION_DIALOG_H
