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

#include <QDir>
#include <QApplication>
#include <QTextCodec>
#include "global_settings_io.h"

GlobalSettingsIO::GlobalSettingsIO()
{
    QDir qdir(QApplication::applicationDirPath());
    if(!qdir.exists("INI")) qdir.mkdir("INI");
    iniFilePath = QApplication::applicationDirPath() + "/INI/" + "preference.ini";

    m_settings = new QSettings(iniFilePath, QSettings::IniFormat);
    m_settings->setIniCodec(QTextCodec::codecForName("UTF-8"));
}

GlobalSettingsIO::~GlobalSettingsIO()
{
    delete m_settings;
}

QSettings* GlobalSettingsIO::settings() const
{
    return m_settings;
}

int GlobalSettingsIO::numberOfFiles() const
{
    return m_numFiles;
}

QStringList GlobalSettingsIO::defaultFilePaths() const
{
    return m_defaultFilePaths;
}

bool GlobalSettingsIO::doShowResult() const
{
    return m_doShowResult;
}

double GlobalSettingsIO::temperature() const
{
    return m_temperature;
}

void GlobalSettingsIO::setNumFiles(int n)
{
    m_numFiles = n;
}

void GlobalSettingsIO::setDefaultFilePaths(QStringList filepaths)
{
    m_defaultFilePaths = filepaths;
}

void GlobalSettingsIO::setDoShowResult(bool status)
{
    m_doShowResult = status;
}

void GlobalSettingsIO::setTemperature(double t)
{
    m_temperature = t;
}

void GlobalSettingsIO::loadIniFile()
{
    m_settings->beginGroup("Preference");
    m_numFiles = m_settings->value("NumFiles", 0).toInt();

    m_defaultFilePaths.clear();
    if(m_numFiles > 0) {
        for(int i = 0; i < m_numFiles; i++) {
            QString filePath  = m_settings->value("File" + QString::number(i), "").toString();
            m_defaultFilePaths.append(filePath);
        }
    }

    m_doShowResult = m_settings->value("ShowResult", false).toBool();
    m_temperature = m_settings->value("Temperature", 25).toDouble();

    m_settings->endGroup();
}

void GlobalSettingsIO::saveIniFile()
{
    m_settings->beginGroup("Preference");

    m_settings->setValue("NumFiles", m_numFiles);
    if( m_numFiles > 0) {
        for(int i = 0; i < m_numFiles; i++) {
            m_settings->setValue("File" + QString::number(i), m_defaultFilePaths[i]);
        }
    }

    m_settings->setValue("ShowResult", m_doShowResult);
    m_settings->setValue("Temperature", m_temperature);

    m_settings->endGroup();
    m_settings->sync();
}


