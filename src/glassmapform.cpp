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


GlassMapForm::GlassMapForm(QList<GlassCatalog*> catalogList, int plotType, QMdiArea *parent) :
    QWidget(parent),
    ui(new Ui::GlassMapForm)
{
    if(catalogList.empty()){
        return;
    }

    m_catalogList = catalogList;
    m_parentMdiArea = parent;
    m_plotType = plotType;

    ui->setupUi(this);

    m_customPlot = ui->widget;
    m_customPlot->setInteraction(QCP::iRangeDrag, true);
    m_customPlot->setInteraction(QCP::iRangeZoom, true);
    m_customPlot->setInteraction(QCP::iSelectItems, true);
    m_customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    m_customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    m_customPlot->legend->setVisible(true);

    m_curveCtrl = new CurveCtrl(this);
    setUpCurveCtrl();

    GlassMapCtrl* entry;
    for(int i = 0; i < catalogList.size(); i++)
    {
        entry = new GlassMapCtrl(this);
        entry->catalog = catalogList.at(i);
        entry->setGlassMap(m_plotType, getColorFromIndex(i));
        m_glassMapCtrlList.append(entry);
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
    m_settingFile = QApplication::applicationDirPath() + "/INI/" + "Glassmap" + QString::number(plotType) + ".ini";
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


    setTitle();
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

void GlassMapForm::setTitle()
{
    switch (m_plotType) {
    case NdVd:
        this->setWindowTitle("Nd-Vd Plot");
        break;
    case NeVe:
        this->setWindowTitle("Ne - Ve Plot");
        break;
    case PgFVd:
        this->setWindowTitle("Vd - PgF Plot");
        break;
    case PCtVd:
        this->setWindowTitle("Vd - PCt Plot");
        break;
    default:
        this->setWindowTitle("Plot");
    }
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

void GlassMapForm::setUpCurveCtrl()
{
    QObject::connect(ui->checkBox_Curve,SIGNAL(toggled(bool)),
                         this, SLOT(update()));
    m_curveCtrl->checkBox = ui->checkBox_Curve;

    m_curveCtrl->lineEditList.clear();
    m_curveCtrl->lineEditList << ui->lineEdit_C0 << ui->lineEdit_C1 << ui->lineEdit_C2 << ui->lineEdit_C3;
    for(int i = 0; i < m_curveCtrl->lineEditList.size(); i++){
        QObject::connect(m_curveCtrl->lineEditList[i],SIGNAL(textEdited(QString)),
                             this, SLOT(update()));
    }
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

                    switch(m_plotType)
                    {
                    case NdVd:
                        dx = (targetGlass->vd() - currentGlass->vd());
                        dy = (targetGlass->nd() - currentGlass->nd())*100;
                        break;
                    case NeVe:
                        dx = (targetGlass->ve() - currentGlass->ve());
                        dy = (targetGlass->ne() - currentGlass->ne())*100;
                        break;
                    case PgFVd:
                        dx = (targetGlass->vd() - currentGlass->vd());
                        dy = (targetGlass->PgF() - currentGlass->PgF())*1000;
                        break;
                    case PCtVd:
                        dx = (targetGlass->vd() - currentGlass->vd());
                        dy = (targetGlass->Pxy("C","t") - currentGlass->Pxy("C","t"))*1000;
                        break;
                    default:
                        dx = 0;
                        dy = 0;
                    }

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
        if(!dlg->calculateFitting(m_plotType)) return;
        QList<double> coefs = dlg->fittingResult();
        ui->lineEdit_C0->setText(QString::number(coefs[0]));
        ui->lineEdit_C1->setText(QString::number(coefs[1]));
        ui->lineEdit_C2->setText(QString::number(coefs[2]));
        ui->lineEdit_C3->setText(QString::number(coefs[3]));
        update();
    }
}

QColor GlassMapForm::getColorFromIndex(int index)
{
    QCPColorGradient colorgrad;
    colorgrad.loadPreset(QCPColorGradient::gpHues);
    QColor color;
    color = colorgrad.color(index, QCPRange(0,m_catalogList.size()));

    return color;
}

void GlassMapForm::setDefault()
{
    switch (m_plotType) {
    case NdVd:
        m_customPlot->xAxis->setLabel("vd");
        m_customPlot->yAxis->setLabel("nd");
        break;
    case NeVe:
        m_customPlot->xAxis->setLabel("ve");
        m_customPlot->yAxis->setLabel("ne");
        break;
    case PgFVd:
        m_customPlot->xAxis->setLabel("vd");
        m_customPlot->yAxis->setLabel("PgF");
        break;
    case PCtVd:
        m_customPlot->xAxis->setLabel("vd");
        m_customPlot->yAxis->setLabel("PCt");
        break;
    }


    // Axis
    QCPRange xrange, yrange;

     switch(m_plotType)
     {
     case NdVd:
         xrange.lower = 10;
         xrange.upper = 100;
         yrange.lower = 1.4;
         yrange.upper = 2.1;
         break;
     case NeVe:
         xrange.lower = 10;
         xrange.upper = 100;
         yrange.lower = 1.4;
         yrange.upper = 2.1;
         break;
     case PgFVd:
         xrange.lower = 10;
         xrange.upper = 100;
         yrange.lower = 0.5;
         yrange.upper = 0.7;
         break;
     case PCtVd:
         xrange.lower = 10;
         xrange.upper = 100;
         yrange.lower = 0.6;
         yrange.upper = 0.9;
         break;
     default:
         break;
     }

     m_customPlot->xAxis->setRangeReversed(true);
     m_customPlot->xAxis->setRange(xrange);
     m_customPlot->yAxis->setRange(yrange);
}

void GlassMapForm::update()
{
    for(int i = 0; i < m_glassMapCtrlList.size(); i++){
        m_glassMapCtrlList.at(i)->update();
    }

    // replot curve
    m_curveCtrl->setData();
    m_curveCtrl->update();

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
    PresetDialog* dlg = new PresetDialog(m_settings,m_curveCtrl->getCoefs(),this);

    if(dlg->exec() == QDialog::Accepted){
        QList<double> coefs = dlg->getCoefs();
        for(int i = 0; i<coefs.size(); i++){
            m_curveCtrl->lineEditList[i]->setText(QString::number(coefs[i]));
        }
        update();
    }
}


/****************************
 * Inner Class GlassMapCtrl
 * **************************/
GlassMapForm::GlassMapCtrl::GlassMapCtrl(GlassMapForm* super)
{
    m_customPlot = super->m_customPlot;
    glassmap = new QCPScatterChart(m_customPlot);
}

GlassMapForm::GlassMapCtrl::~GlassMapCtrl()
{
    try{
        delete catalog;
        catalog = nullptr;
        delete glassmap;
        glassmap = nullptr;
    } catch (...) {
        catalog = nullptr;
        glassmap = nullptr;
    }
    m_customPlot = nullptr;
}

void GlassMapForm::GlassMapCtrl::setGlassMap(int plotType, QColor color)
{
    QVector<double> x,y;
    QVector<QString> str;

        x.clear();
        y.clear();
        str.clear();

        switch(plotType)
        {
        case NdVd: //vd-nd
            for(int i = 0; i < catalog->glassCount(); i++)
            {
                x.append(catalog->glass(i)->vd());
                y.append(catalog->glass(i)->nd());
                str.append(catalog->glass(i)->name());
            }
            break;
        case NeVe:
            for(int i = 0; i < catalog->glassCount(); i++)
            {
                x.append(catalog->glass(i)->ve());
                y.append(catalog->glass(i)->ne());
                str.append(catalog->glass(i)->name());
            }
            break;
        case PgFVd:
            for(int i = 0; i < catalog->glassCount(); i++)
            {
                x.append(catalog->glass(i)->vd());
                y.append(catalog->glass(i)->PgF());
                str.append(catalog->glass(i)->name());
            }
            break;
        case PCtVd:
            for(int i = 0; i < catalog->glassCount(); i++)
            {
                x.append(catalog->glass(i)->vd());
                y.append(catalog->glass(i)->Pxy("C","t"));
                str.append(catalog->glass(i)->name());
            }

        }

        glassmap->setData(x,y,str);
        glassmap->setName(catalog->supplyer());
        glassmap->setColor(color);
}

void GlassMapForm::GlassMapCtrl::setVisible(bool pointstate, bool labelstate)
{
    glassmap->setVisiblePointSeries(pointstate);
    glassmap->setVisibleTextLabels(labelstate);
}

void GlassMapForm::GlassMapCtrl::update()
{
    setVisible(checkBoxPlot->checkState(), checkBoxLabel->checkState());
}


/****************************
 * Inner Class CurveCtrl
 * **************************/
GlassMapForm::CurveCtrl::CurveCtrl(GlassMapForm* super)
{
    m_customPlot = super->m_customPlot;
    graph = m_customPlot->addGraph();
    checkBox = super->ui->checkBox_Curve;
    lineEditList.clear();
}

GlassMapForm::CurveCtrl::~CurveCtrl()
{
    m_customPlot->removeGraph(graph);
    lineEditList.clear();
    checkBox = nullptr;
    m_customPlot = nullptr;
}

void GlassMapForm::CurveCtrl::setData()
{
    if(!graph) return;

    double c;

    QVector<double> x(101),y(101);
    double xmin, xmax;

    xmin = 10;
    xmax = 100;

    for(int i = 0; i < 101; i++)
    {
        x[i] = xmin + (xmax-xmin)*(double)i/100;

        y[i] = 0;
        for(int j = 0;j < lineEditList.size(); j ++)
        {
            c = lineEditList[j]->text().toDouble();
            y[i] += c*pow(x[i],j);
        }
    }

    graph->setData(x,y);
    graph->setName("curve");

    QPen pen;
    pen.setColor(Qt::black); //black
    graph->setPen(pen);

}

void GlassMapForm::CurveCtrl::setVisible(bool state)
{
    graph->setVisible(state);
}

void GlassMapForm::CurveCtrl::update()
{
    setData();
    setVisible(checkBox->isChecked());
}

QList<double> GlassMapForm::CurveCtrl::getCoefs()
{
    QList<double> coefs;

    for(int i = 0; i < lineEditList.size(); i++)
    {
        coefs.append(lineEditList[i]->text().toDouble());
    }
    return coefs;
}
