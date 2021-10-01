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

#ifndef PRESET_DIALOG_H
#define PRESET_DIALOG_H

#include <QDialog>

#include <QListWidget>
#include <QSettings>

namespace Ui {
class PresetDialog;
}

class PresetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PresetDialog(QSettings* settings, QList<double> currentCoefs, QWidget *parent = nullptr);
    ~PresetDialog();

    QList<double> getCoefs();

private slots:
    void loadPreset();
    void saveAsNew();
    void deletePreset();

private:
    Ui::PresetDialog *ui;

    QSettings* m_settings;
    QListWidget* m_list;

    QList<double> m_coefs;

    void updateList();

};

#endif // PRESET_DIALOG_H
