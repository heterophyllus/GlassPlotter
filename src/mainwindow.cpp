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


#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setWindowTitle("GlassPlotter");

    // File menu
    QObject::connect(ui->action_loadAGF,SIGNAL(triggered()), this, SLOT(loadAGF()));
    QObject::connect(ui->action_loadXML,SIGNAL(triggered()), this, SLOT(loadXML()));

    // Tools menu
    QObject::connect(ui->action_NdVd,              SIGNAL(triggered()),this, SLOT(showGlassMapNdVd()));
    QObject::connect(ui->action_NeVe,              SIGNAL(triggered()),this, SLOT(showGlassMapNeVe()));
    QObject::connect(ui->action_VdPgF,             SIGNAL(triggered()),this, SLOT(showGlassMapVdPgF()));
    QObject::connect(ui->action_VdPCt,             SIGNAL(triggered()),this, SLOT(showGlassMapVdPCt()));
    QObject::connect(ui->action_DispersionPlot,    SIGNAL(triggered()),this, SLOT(showDispersionPlot()));
    QObject::connect(ui->action_TransmittancePlot, SIGNAL(triggered()),this, SLOT(showTransmittancePlot()));
    QObject::connect(ui->action_DnDtabsPlot,       SIGNAL(triggered()),this, SLOT(showDnDtabsPlot()));
    QObject::connect(ui->action_CatalogView,       SIGNAL(triggered()),this, SLOT(showCatalogViewForm()));

    // Window menu
    QObject::connect(ui->action_Tile,    SIGNAL(triggered()),this, SLOT(tileWindows()));
    QObject::connect(ui->action_Cascade, SIGNAL(triggered()),this, SLOT(cascadeWindows()));
    QObject::connect(ui->action_CloseAll,SIGNAL(triggered()),this, SLOT(closeAll()));

    // Help menu
    QObject::connect(ui->action_About, SIGNAL(triggered()), this, SLOT(showAbout()));

    //setAttribute(Qt::WA_DeleteOnClose);
    setUnifiedTitleAndToolBarOnMac(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateStatusBar()
{
    ui->statusbar->showMessage(m_agfDir);
}

void MainWindow::loadAGF()
{
    // clear before reload
    ui->mdiArea->closeAllSubWindows();

    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                                                          tr("select AGF"),
                                                          QApplication::applicationDirPath(),
                                                          tr("AGF files(*.agf);;All Files(*.*)"));
    if(filePaths.empty()){
        QMessageBox::warning(this,tr("File"), tr("No AGF file is loaded"));
        m_agfDir = "";
        updateStatusBar();
        return;
    }

    m_catalogList.clear();

    GlassCatalog* catalog;
    for(int i = 0; i < filePaths.size(); i++){
        catalog = new GlassCatalog;
        if(catalog->loadAGF(filePaths[i])){
            m_catalogList.append(catalog);
        }
    }

    // show path in statusbar
    QFileInfo finfo(filePaths.first());
    m_agfDir = finfo.absolutePath();
    updateStatusBar();

    QMessageBox::information(this,tr("Message"), tr("AGF files are successfully loaded"));
}

void MainWindow::loadXML()
{
    // clear before reload
    ui->mdiArea->closeAllSubWindows();

    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                                                          tr("select XML"),
                                                          QApplication::applicationDirPath(),
                                                          tr("XML files(*.xml);;All Files(*.*)"));
    if(filePaths.empty()){
        QMessageBox::warning(this,tr("File"), tr("No XML file is loaded"));
        m_agfDir = "";
        updateStatusBar();
        return;
    }

    m_catalogList.clear();

    GlassCatalog* catalog;
    for(int i = 0; i < filePaths.size(); i++){
        catalog = new GlassCatalog;
        if(catalog->loadXml(filePaths[i])){
            m_catalogList.append(catalog);
        }
    }

    // show path in statusbar
    QFileInfo finfo(filePaths.first());
    m_agfDir = finfo.absolutePath();
    updateStatusBar();

    QMessageBox::information(this,tr("Message"), tr("XML files are successfully loaded"));
}


void MainWindow::showGlassMap(QString xdataname, QString ydataname, QCPRange xrange, QCPRange yrange)
{
    if(m_catalogList.empty()){
        QMessageBox::warning(this,tr("File"), tr("No catalog has been loaded"));
        return;
    }

    GlassMapForm *subwindow = new GlassMapForm(m_catalogList, xdataname, ydataname, xrange, yrange, ui->mdiArea);
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
    showGlassMap("vd", "PCt",QCPRange(10,100), QCPRange(0.6,0.9));
}

template<class F> void MainWindow::showAnalysisForm()
{
    if(m_catalogList.empty()){
        QMessageBox::warning(this,tr("File"), tr("No catalog has been loaded"));
        return;
    }

    F *subwindow = new F(m_catalogList, ui->mdiArea);
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
    AboutDialog dlg(this);
    dlg.exec();
}
