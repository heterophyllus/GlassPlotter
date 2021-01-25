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

#include "glassmapform.h"
#include "ui_glassmapform.h"


GlassMapForm::GlassMapForm(QList<GlassCatalog*> catalogList, QString xdataname, QString ydataname, QCPRange xrange, QCPRange yrange, QMdiArea *parent) :
    QWidget(parent),
    ui(new Ui::GlassMapForm)
{
    if(catalogList.empty()){
        return;
    }

    m_catalogList = catalogList;
    m_parentMdiArea = parent;

    m_xDataName = xdataname;
    m_yDataName = ydataname;
    m_defaultXrange = xrange;
    m_defaultYrange = yrange;

    ui->setupUi(this);

    m_customPlot = ui->widget;
    m_customPlot->setInteraction(QCP::iRangeDrag, true);
    m_customPlot->setInteraction(QCP::iRangeZoom, true);
    m_customPlot->setInteraction(QCP::iSelectItems, true);
    m_customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    m_customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    m_customPlot->legend->setVisible(true);

    // user defined curve control
    curveGraph = m_customPlot->addGraph();
    checkBoxCurve = ui->checkBox_Curve;
    QObject::connect(ui->checkBox_Curve,SIGNAL(toggled(bool)), this, SLOT(update()));

    QObject::connect(ui->lineEdit_C0,SIGNAL(textEdited(QString)),this, SLOT(update()));
    QObject::connect(ui->lineEdit_C1,SIGNAL(textEdited(QString)),this, SLOT(update()));
    QObject::connect(ui->lineEdit_C2,SIGNAL(textEdited(QString)),this, SLOT(update()));
    QObject::connect(ui->lineEdit_C3,SIGNAL(textEdited(QString)),this, SLOT(update()));


    // glassmap control
    m_glassMapCtrlList.clear();
    GlassMapCtrl* gmc;
    for(int i = 0; i < catalogList.size(); i++)
    {
        gmc = new GlassMapCtrl(m_customPlot);
        setGlassmapData(gmc->glassmap,catalogList[i],xdataname,ydataname, QCPUtil::getColorFromIndex(i,catalogList.size()));
        m_glassMapCtrlList.append(gmc);
    }
    setUpScrollArea();


    // Legend
    m_checkBoxLegend = ui->checkBox_Legend;
    QObject::connect(m_checkBoxLegend,SIGNAL(toggled(bool)), this, SLOT(setLegendVisible()));

    // neighbors
    m_listWidgetNeighbors = ui->listWidget_Neighbors;
    QObject::connect(ui->pushButton_showDatasheet, SIGNAL(clicked()), this, SLOT(showGlassDataSheet()));

    // show plot control tab
    ui->toolBox->setCurrentIndex(0);

    // settings
    QDir qdir(QApplication::applicationDirPath());
    if(!qdir.exists("INI")) qdir.mkdir("INI");
    m_settingFile = QApplication::applicationDirPath() + "/INI/" + "Glassmap_" + xdataname + "_" + ydataname + ".ini";
    m_settings = new QSettings(m_settingFile, QSettings::IniFormat);
    m_settings->setIniCodec(QTextCodec::codecForName("UTF-8"));

    //mouse
    QObject::connect(m_customPlot, SIGNAL(itemClick(QCPAbstractItem*,QMouseEvent*)), this, SLOT(showNeighbors(QCPAbstractItem*,QMouseEvent*)));
    QObject::connect(m_customPlot,SIGNAL(mousePress(QMouseEvent*)),this, SLOT(clearNeighbors(QMouseEvent*)));

    // fitting
    QObject::connect(ui->pushButton_Fitting, SIGNAL(clicked()), this, SLOT(showCurveFittingDlg()));

    // reset view button
    QObject::connect(ui->pushButton_resetView, SIGNAL(clicked()), this, SLOT(resetView()));

    // preset
    QObject::connect(ui->pushButton_Preset, SIGNAL(clicked()), this, SLOT(showPresetDlg()));

    // window title
    this->setWindowTitle( xdataname + " - " + ydataname + " Plot");

    setDefault();
    update();
}

GlassMapForm::~GlassMapForm()
{
    delete m_settings;
    m_customPlot->clearPlottables();
    m_customPlot->clearItems();
    m_glassMapCtrlList.clear();
    m_customPlot = nullptr;

    delete ui;
}

void GlassMapForm::setUpScrollArea()
{
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QLabel *label;

    //Dynamically create checkboxes
    gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_PlotControl"));

    for(int i = 0; i < m_glassMapCtrlList.size(); i++){
        // supplyer name
        label = new QLabel(ui->scrollAreaWidgetContents);
        label->setObjectName("label_" + QString::number(i));
        label->setText(m_catalogList.at(i)->supplyer());
        gridLayout->addWidget(label, i, 0, 1, 1);
        m_glassMapCtrlList.at(i)->labelSupplyer = label;

        // plot on/off
        checkBox = new QCheckBox(ui->scrollAreaWidgetContents);
        checkBox->setObjectName("checkBox_plot_"+QString::number(i));
        checkBox->setText("Point");
        gridLayout->addWidget(checkBox, i, 1, 1, 1);
        QObject::connect(checkBox,SIGNAL(toggled(bool)), this, SLOT(update()));
        m_glassMapCtrlList.at(i)->checkBoxPlot = checkBox;

        // label on/off
        checkBox = new QCheckBox(ui->scrollAreaWidgetContents);
        checkBox->setObjectName("checkBox_label_"+QString::number(i));
        checkBox->setText("Label");
        QObject::connect(checkBox,SIGNAL(toggled(bool)), this, SLOT(update()));
        gridLayout->addWidget(checkBox, i, 2, 1, 1);
        m_glassMapCtrlList.at(i)->checkBoxLabel = checkBox;
    }
    ui->scrollArea->setWidgetResizable(true);
}

void GlassMapForm::setLegendVisible()
{
    m_customPlot->legend->setVisible(m_checkBoxLegend->checkState());
    m_customPlot->replot();
}

void GlassMapForm::showNeighbors(QCPAbstractItem* item, QMouseEvent *event)
{
    if(m_customPlot->selectedItems().size() > 0){
        QString glassName = item->objectName();
        Glass* targetGlass = getGlassFromName(glassName);

        //search neighbors
        GlassCatalog* cat;
        Glass* currentGlass;
        double dx,dy;
        for(int i = 0;i<m_catalogList.size();i++){

            if(m_glassMapCtrlList[i]->checkBoxPlot->checkState()){
                cat = m_catalogList[i];
                for(int j=0;j<cat->glassCount();j++){
                    currentGlass = cat->glass(j);

                    dx = (targetGlass->getValue(m_xDataName) - currentGlass->getValue(m_xDataName));
                    dy = (targetGlass->getValue(m_yDataName) - currentGlass->getValue(m_yDataName))*100;

                    if(abs(dx) < m_neighborThreshold && abs(dy) < m_neighborThreshold){
                        m_listWidgetNeighbors->addItem(currentGlass->name() + "_" + currentGlass->supplyer());
                    }
                }
            }
        }
    }
    m_listWidgetNeighbors->update();
}

void GlassMapForm::clearNeighbors(QMouseEvent* event)
{
    m_listWidgetNeighbors->clear();
    m_listWidgetNeighbors->update();
}

void GlassMapForm::showGlassDataSheet()
{
    if(m_listWidgetNeighbors->selectedItems().size() > 0){
        QString selectedText = m_listWidgetNeighbors->currentItem()->text();
        QStringList splitedText = selectedText.split("_");
        QString glassName = splitedText[0];
        Glass* glass = getGlassFromName(glassName);
        GlassDataSheetForm* subwindow = new GlassDataSheetForm(glass, m_parentMdiArea);
        m_parentMdiArea->addSubWindow(subwindow);
        subwindow->parentWidget()->setGeometry(0,10, this->width()*1/2,this->height()*3/4);
        subwindow->show();
    }
}

void GlassMapForm::showCurveFittingDlg()
{
    CurveFittingDialog* dlg = new CurveFittingDialog(m_catalogList, this);
    if(dlg->exec() == QDialog::Accepted)
    {
        //if(!dlg->calculateFitting(m_plotType)) return;
        QList<double> coefs = dlg->fittingResult();
        ui->lineEdit_C0->setText(QString::number(coefs[0]));
        ui->lineEdit_C1->setText(QString::number(coefs[1]));
        ui->lineEdit_C2->setText(QString::number(coefs[2]));
        ui->lineEdit_C3->setText(QString::number(coefs[3]));
        update();
    }
}


void GlassMapForm::setDefault()
{
    m_customPlot->xAxis->setLabel(m_xDataName);
    m_customPlot->yAxis->setLabel(m_yDataName);

    m_customPlot->xAxis->setRangeReversed(true);
    m_customPlot->xAxis->setRange(m_defaultXrange);
    m_customPlot->yAxis->setRange(m_defaultYrange);
}

void GlassMapForm::update()
{
    // check and update visibility
    for(int i = 0; i < m_glassMapCtrlList.size(); i++){
        m_glassMapCtrlList[i]->glassmap->setVisiblePointSeries( m_glassMapCtrlList[i]->checkBoxPlot->checkState() );
        m_glassMapCtrlList[i]->glassmap->setVisibleTextLabels( m_glassMapCtrlList[i]->checkBoxLabel->checkState() );
    }

    // replot user defined curve
    setCurveData();
    curveGraph->setVisible(checkBoxCurve->checkState());

    m_customPlot->replot();
}

void GlassMapForm::resetView()
{
    setDefault();
    m_customPlot->replot();
}

Glass* GlassMapForm::getGlassFromName(QString glassName)
{
    for(int i = 0; i < m_catalogList.size(); i++){
        if(m_catalogList.at(i)->hasGlass(glassName)){
            return m_catalogList.at(i)->glass(glassName);
        }
    }
    return nullptr;
}

void GlassMapForm::showPresetDlg()
{
    PresetDialog* dlg = new PresetDialog(m_settings,getCurveCoefs(),this);

    if(dlg->exec() == QDialog::Accepted){
        setCurveCoefsToUI(dlg->getCoefs());
        update();
    }
}


/****************************
 * Inner Class GlassMapCtrl
 * **************************/
GlassMapForm::GlassMapCtrl::GlassMapCtrl(QCustomPlot* customPlot)
{
    m_customPlot = customPlot;
    glassmap = new QCPScatterChart(m_customPlot);
}

GlassMapForm::GlassMapCtrl::~GlassMapCtrl()
{
    m_customPlot = nullptr;
}

void GlassMapForm::setGlassmapData(QCPScatterChart* glassmap,GlassCatalog* catalog, QString xlabel, QString ylabel, QColor color)
{
    QVector<double> x,y;
    QVector<QString> str;

    x.clear();
    y.clear();
    str.clear();

    for(int i = 0; i < catalog->glassCount(); i++)
    {
        x.append(catalog->glass(i)->getValue(xlabel));
        y.append(catalog->glass(i)->getValue(ylabel));
        str.append(catalog->glass(i)->name());
    }

    glassmap->setData(x,y,str);
    glassmap->setName(catalog->supplyer());
    glassmap->setColor(color);
}


void GlassMapForm::setCurveData()
{
    QVector<double> x(101),y(101);
    QList<double> coefs = getCurveCoefs();
    double xmin = m_customPlot->xAxis->range().lower;
    double xmax = m_customPlot->xAxis->range().upper;

    for(int i = 0; i < 101; i++)
    {
        x[i] = xmin + (xmax-xmin)*(double)i/100;

        y[i] = 0;
        for(int j = 0;j < coefs.size(); j++)
        {
            y[i] += coefs[j]*pow(x[i],j);
        }
    }

    curveGraph->setData(x,y);
    curveGraph->setName("curve");

    QPen pen;
    pen.setColor(Qt::black); //black
    curveGraph->setPen(pen);

}

QList<double> GlassMapForm::getCurveCoefs()
{
    QList<double> coefs;
    coefs.clear();

    coefs.append(ui->lineEdit_C0->text().toDouble());
    coefs.append(ui->lineEdit_C1->text().toDouble());
    coefs.append(ui->lineEdit_C2->text().toDouble());
    coefs.append(ui->lineEdit_C3->text().toDouble());

    return coefs;
}

void GlassMapForm::setCurveCoefsToUI(QList<double> coefs)
{
    ui->lineEdit_C0->setText(QString::number(coefs[0]));
    ui->lineEdit_C1->setText(QString::number(coefs[1]));
    ui->lineEdit_C2->setText(QString::number(coefs[2]));
    ui->lineEdit_C3->setText(QString::number(coefs[3]));
}
