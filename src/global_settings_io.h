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
 **  Date    : 2022-4-10                                                    **
 *****************************************************************************/

#ifndef GLOBAL_SETTINGS_IO_H
#define GLOBAL_SETTINGS_IO_H

#include <QSettings>

/** Preference settings io */
class GlobalSettingsIO
{
public:
    GlobalSettingsIO();
    ~GlobalSettingsIO();

    QSettings* settings() const;

    void loadIniFile();
    void saveIniFile();

    int numberOfFiles() const;
    QStringList defaultFilePaths() const;
    bool doShowResult() const;
    double temperature() const;

    void setNumFiles(int n);
    void setDefaultFilePaths(QStringList filepaths);
    void setDoShowResult(bool status);
    void setTemperature(double t);

private:
    QString iniFilePath;
    QSettings *m_settings;

    // contents
    int m_numFiles;
    QStringList m_defaultFilePaths;
    bool m_doShowResult;
    double m_temperature;
};



#endif
