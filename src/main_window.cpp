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


#include "main_window.h"
#include "ui_main_window.h"

#include <QFileDialog>
#include <QMessageBox>

#include "glassmap_form.h"
#include "dispersion_plot_form.h"
#include "transmittance_plot_form.h"
#include "glass_datasheet_form.h"
#include "dndt_plot_form.h"
#include "catalog_view_form.h"
#include "glass_search_form.h"
#include "load_catalog_result_dialog.h"
#include "preference_dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setWindowTitle("GlassPlotter");

    // File menu
    QObject::connect(ui->action_loadAGF,    SIGNAL(triggered()), this, SLOT(loadNewAGF()));
    QObject::connect(ui->action_loadXML,    SIGNAL(triggered()), this, SLOT(loadNewXML()));
    QObject::connect(ui->action_Preference, SIGNAL(triggered()), this, SLOT(showPreferenceDlg()));

    // Tools menu
    QObject::connect(ui->action_NdVd,              SIGNAL(triggered()),this, SLOT(showGlassMapNdVd()));
    QObject::connect(ui->action_NeVe,              SIGNAL(triggered()),this, SLOT(showGlassMapNeVe()));
    QObject::connect(ui->action_VdPgF,             SIGNAL(triggered()),this, SLOT(showGlassMapVdPgF()));
    QObject::connect(ui->action_VdPCt,             SIGNAL(triggered()),this, SLOT(showGlassMapVdPCt()));
    QObject::connect(ui->action_Buchdahl,          SIGNAL(triggered()),this, SLOT(showGlassMapBuchdahl()));
    QObject::connect(ui->action_DispersionPlot,    SIGNAL(triggered()),this, SLOT(showDispersionPlot()));
    QObject::connect(ui->action_TransmittancePlot, SIGNAL(triggered()),this, SLOT(showTransmittancePlot()));
    QObject::connect(ui->action_DnDtabsPlot,       SIGNAL(triggered()),this, SLOT(showDnDtabsPlot()));
    QObject::connect(ui->action_CatalogView,       SIGNAL(triggered()),this, SLOT(showCatalogViewForm()));
    QObject::connect(ui->action_GlassSearch,       SIGNAL(triggered()),this, SLOT(showGlassSearchForm()));

    // Window menu
    QObject::connect(ui->action_Tile,    SIGNAL(triggered()),this, SLOT(tileWindows()));
    QObject::connect(ui->action_Cascade, SIGNAL(triggered()),this, SLOT(cascadeWindows()));
    QObject::connect(ui->action_CloseAll,SIGNAL(triggered()),this, SLOT(closeAll()));

    // Help menu
    QObject::connect(ui->action_About, SIGNAL(triggered()), this, SLOT(showAbout()));

    //setAttribute(Qt::WA_DeleteOnClose);
    setUnifiedTitleAndToolBarOnMac(true);

    // preference
    QDir qdir(QApplication::applicationDirPath());
    if(!qdir.exists("INI")) qdir.mkdir("INI");
    const QString settingFile = QApplication::applicationDirPath() + "/INI/" + "preference.ini";

    m_settings = new QSettings(settingFile, QSettings::IniFormat);
    m_settings->setIniCodec(QTextCodec::codecForName("UTF-8"));

    //Set temperature
    double temperature = m_settings->value("Temperature", 25).toDouble();
    Glass::setCurrentTemperature(temperature);

    // Load default catalogs
    //loadDefaultCatalogFiles();

}

MainWindow::~MainWindow()
{
    try {
        delete m_settings;
    }  catch (...) {
        qDebug() << "memory release error: m_setting in ~MainWindow";
    }
    m_settings = nullptr;

    if(!m_catalogList.isEmpty()){
        for(auto &cat: m_catalogList){
            try {
                delete cat;
            }  catch (...) {
                cat = nullptr;
            }
            cat = nullptr;
        }
    }
    m_catalogList.clear();

    delete ui;
}

void MainWindow::loadCatalogFiles(const QStringList& catalogFilePaths)
{
    if(catalogFilePaths.empty()) {
        return;
    }

    // clear old catalogs
    if(!m_catalogList.isEmpty())
    {
        for (auto &cat: m_catalogList) {
            delete cat;
        }
        m_catalogList.clear();
    }

    // load catalogs
    GlassCatalog* catalog;
    QString parse_result, parse_result_all;

    QFileInfo finfo;
    finfo.setFile(catalogFilePaths.first());
    QString ext = finfo.suffix().toLower(); // .agf, .xml

    for(int i = 0; i < catalogFilePaths.size(); i++){
        catalog = new GlassCatalog;
        parse_result.clear();

        bool ok;
        if(ext == "agf"){
            ok = catalog->loadAGF(catalogFilePaths[i], parse_result);
        }else{
            ok = catalog->loadXml(catalogFilePaths[i], parse_result);
        }

        if(ok){
            m_catalogList.append(catalog);
            parse_result_all += parse_result;
        }
        else{
            parse_result_all += ("Catalog loading error:" + catalogFilePaths[i] + "\n");
            try {
                delete catalog;
            }  catch (...) {
                qDebug() << "memory release error";
                return;
            }
            continue;
        }
    }

    catalog = nullptr;


    // show parse result
    if(m_loadWithResult) {
        LoadCatalogResultDialog dlg(this);
        dlg.setLabel("Loading catalog files has been finished.\nBelows are notable parse results.");
        dlg.setText(parse_result_all);
        dlg.exec();
    }else{
        QMessageBox::information(this, tr("Info"), "Catalog files were newly loaded");
    }

}

void MainWindow::loadDefaultCatalogFiles()
{
    m_settings->beginGroup("Preference");

    int catalogCount = m_settings->value("NumFiles", 0).toInt();
    QStringList catalogFilePaths;
    if(catalogCount > 0) {
        for(int i = 0; i < catalogCount; i++) {
            QString filePath = m_settings->value("File" + QString::number(i), "").toString();
            catalogFilePaths.append(filePath);
        }
    }

    m_loadWithResult = m_settings->value("ShowResult", false).toBool();

    m_settings->endGroup();

    loadCatalogFiles(catalogFilePaths);

}

void MainWindow::loadNewAGF()
{
    // open file selection dialog
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                                                          tr("select AGF"),
                                                          QApplication::applicationDirPath(),
                                                          tr("AGF files(*.agf);;All Files(*.*)"));
    if(filePaths.empty()){
        QMessageBox::warning(this,tr("Canceled"), tr("Canceled"));
        return;
    }else{
        ui->mdiArea->closeAllSubWindows();
        loadCatalogFiles(filePaths);
        //QMessageBox::information(this, tr("Info"), tr("AGF files have been newly loaded"));
    }

}

void MainWindow::loadNewXML()
{
    // open file selection dialog
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                                                          tr("select XML"),
                                                          QApplication::applicationDirPath(),
                                                          tr("XML files(*.xml);;All Files(*.*)"));
    if(filePaths.empty()){
        QMessageBox::warning(this,tr("Canceled"), tr("Canceled"));
        return;
    } else {
        ui->mdiArea->closeAllSubWindows();
        loadCatalogFiles(filePaths);
        //QMessageBox::information(this, tr("Info"), tr("XML files have been newly loaded"));
    }
}


void MainWindow::showPreferenceDlg()
{
    closeAll();

    PreferenceDialog* dlg = new PreferenceDialog(m_settings, this);
    if(dlg->exec() == QDialog::Accepted){
        int ans = QMessageBox::question(this, tr("Question"), tr("Setting has been updated. Will you load newly set catalog files?"));
        if(ans == QMessageBox::Yes){
            QStringList paths = dlg->getCatalogPaths();
            if(paths.empty()){
                QMessageBox::warning(this, tr("Warning"), tr("No catalog paths"));
            }else{
                //closeAll();
                loadDefaultCatalogFiles();
            }
        }
    }

    delete dlg;

}

void MainWindow::showGlassMap(QString xdataname, QString ydataname, QCPRange xrange, QCPRange yrange, bool xreversed)
{
    if(m_catalogList.empty()){
        QMessageBox::warning(this,tr("Error"), tr("No catalog has been loaded."));
        return;
    }

    GlassMapForm *subwindow = new GlassMapForm(&m_catalogList, xdataname, ydataname, xrange, yrange, xreversed, ui->mdiArea);
    ui->mdiArea->addSubWindow(subwindow);
    subwindow->setAttribute(Qt::WA_DeleteOnClose);
    subwindow->parentWidget()->setGeometry(0,10,this->width()*3/4,this->height()*3/4);
    subwindow->show();
}

void MainWindow::showGlassMapNdVd()
{
    showGlassMap("vd", "nd", QCPRange(10,100), QCPRange(1.4,2.1));
}

void MainWindow::showGlassMapNeVe()
{
    showGlassMap("ve", "ne", QCPRange(10,100), QCPRange(1.4,2.1));
}

void MainWindow::showGlassMapVdPgF()
{
    showGlassMap("vd", "PgF", QCPRange(10,100), QCPRange(0.5, 0.7));
}

void MainWindow::showGlassMapVdPCt()
{
    showGlassMap("vd", "PCt_",QCPRange(10,100), QCPRange(0.6,0.9));
}

void MainWindow::showGlassMapBuchdahl()
{
    showGlassMap("eta2", "eta1",QCPRange(-0.025,0.175), QCPRange(-0.25,0.0), false);
}



template<class F> void MainWindow::showAnalysisForm()
{
    if(m_catalogList.empty()){
        QMessageBox::warning(this,tr("File"), tr("No catalog has been loaded"));
        return;
    }

    F *subwindow = new F(&m_catalogList, ui->mdiArea);
    ui->mdiArea->addSubWindow(subwindow);
    subwindow->setAttribute(Qt::WA_DeleteOnClose);
    subwindow->parentWidget()->setGeometry(0,10,this->width()*3/4,this->height()*3/4);
    subwindow->show();
}

void MainWindow::showDispersionPlot()
{
    showAnalysisForm<DispersionPlotForm>();
}

void MainWindow::showTransmittancePlot()
{
    showAnalysisForm<TransmittancePlotForm>();
}

void MainWindow::showDnDtabsPlot()
{
    showAnalysisForm<DnDtPlotForm>();
}

void MainWindow::showCatalogViewForm()
{    
    showAnalysisForm<CatalogViewForm>();
}

void MainWindow::showGlassSearchForm()
{
    showAnalysisForm<GlassSearchForm>();
}

void MainWindow::tileWindows()
{
    ui->mdiArea->tileSubWindows();
}

void MainWindow::cascadeWindows()
{
    ui->mdiArea->cascadeSubWindows();
}

void MainWindow::closeAll()
{
    ui->mdiArea->closeAllSubWindows();
}

void MainWindow::showAbout()
{
    QString text =
            "GlassPlotter<br><br>"
            "<font size=1>Version: 0.19.0 <br><br>"
            "This program is distributed in the hope that it will be useful,\n"
            "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
            "GNU General Public License for more details.<br><br>"
            "Copyright(C) 2020 - present  Hiiragi<br><br>"
            "Contact : heterophyllus.work@gmail.com<br><br>"
            "Please see: "
            "<a href=\"https://github.com/heterophyllus/GlassPlotter\">"
            "https://github.com/heterophyllus/GlassPlotter</a></font>";

    QMessageBox msgBox(this);
    msgBox.setText(text);
    msgBox.setWindowTitle(tr("About GlassPlotter"));
    msgBox.setIconPixmap(QPixmap(":data/icon/GlassPlotterIcon.png").scaled(64,64,Qt::KeepAspectRatio, Qt::FastTransformation));
    msgBox.exec();
}
