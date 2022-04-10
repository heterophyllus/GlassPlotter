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

#include "preference_dialog.h"
#include "ui_preference_dialog.h"

#include <QFileDialog>
#include <QDebug>
#include "glass.h"

PreferenceDialog::PreferenceDialog(GlobalSettingsIO *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Preference");

    ui->lineEdit_Temperature->setValidator(new QDoubleValidator(-30, 70, 4, this));

    QObject::connect(ui->pushButton_Browse, SIGNAL(clicked()), this, SLOT(browseCatalogFiles()));
    QObject::connect(ui->pushButton_Clear,  SIGNAL(clicked()), this, SLOT(clearCatalogFiles()));
    QObject::connect(ui->buttonBox,         SIGNAL(accepted()), this, SLOT(onAccept()));

    m_globalSettings = settings;
    syncUiWithSettings();
}

PreferenceDialog::~PreferenceDialog()
{
    m_globalSettings = nullptr;
    delete ui;
}

QStringList PreferenceDialog::getCatalogPaths()
{
    int pathCount = ui->listWidget_DefaultFiles->count();
    QStringList paths;
    for(int i = 0; i < pathCount; i++){
        paths.append(ui->listWidget_DefaultFiles->item(i)->text());
    }
    return paths;
}

void PreferenceDialog::browseCatalogFiles()
{
    ui->listWidget_DefaultFiles->clear();

    // open file selection dialog
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                                                          tr("select AGF"),
                                                          QApplication::applicationDirPath(),
                                                          tr("AGF files(*.agf);;XML Files(*.xml)"));
    if(!filePaths.empty()){
        ui->listWidget_DefaultFiles->addItems(filePaths);
    }

}

void PreferenceDialog::clearCatalogFiles()
{
    ui->listWidget_DefaultFiles->clear();
}

void PreferenceDialog::syncUiWithSettings()
{
    // catalog file paths
    int catalogFilecount = m_globalSettings->numberOfFiles();
    QStringList filePaths = m_globalSettings->defaultFilePaths();
    if(catalogFilecount > 0) {
        for(int i = 0; i < catalogFilecount; i++) {
            ui->listWidget_DefaultFiles->addItem(filePaths[i]);
        }
    }

    // on/off result dialog
    bool loadWithResult = m_globalSettings->doShowResult();
    ui->checkBox_ParseResult->setChecked(loadWithResult);

    // environment
    double temperature = m_globalSettings->temperature();
    ui->lineEdit_Temperature->setText(QString::number(temperature));
}

void PreferenceDialog::onAccept()
{
    // catalog file paths
    int catalogFileCount = ui->listWidget_DefaultFiles->count();
    m_globalSettings->setNumFiles(catalogFileCount);

    QStringList defaultFilePaths;
    if( catalogFileCount > 0) {
        for(int i = 0; i < catalogFileCount; i++) {
            defaultFilePaths.append( ui->listWidget_DefaultFiles->item(i)->text() );
        }
    }
    m_globalSettings->setDefaultFilePaths(defaultFilePaths);

    // on/off result dialog
    m_globalSettings->setDoShowResult(ui->checkBox_ParseResult->checkState());

    //environment
    double temperature = ui->lineEdit_Temperature->text().toDouble();
    m_globalSettings->setTemperature(temperature);

    Glass::setCurrentTemperature(temperature);

    m_globalSettings->saveIniFile();

    accept();
}
