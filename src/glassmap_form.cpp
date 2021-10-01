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

#include "glassmap_form.h"
#include "ui_glassmap_form.h"

#include "glass_datasheet_form.h"
#include "curve_fitting_dialog.h"
#include "preset_dialog.h"

GlassMapForm::GlassMapForm(const QList<GlassCatalog*> *catalogListPtr, QString xdataname, QString ydataname, QCPRange xrange, QCPRange yrange, bool xreversed, QMdiArea *parent) :
    QWidget(parent),
    ui(new Ui::GlassMapForm),
    m_parentMdiArea(parent),
    m_catalogListPtr(catalogListPtr),
    m_xDataName(xdataname),
    m_yDataName(ydataname),
    m_defaultXrange(xrange),
    m_defaultYrange(yrange),
    m_xReversed(xreversed)
{
    if(catalogListPtr->empty()){
        return;
    }

    ui->setupUi(this);

    // plot widget
    m_customPlot = ui->widget;
    m_customPlot->setInteraction(QCP::iRangeDrag, true);
    m_customPlot->setInteraction(QCP::iRangeZoom, true);
    m_customPlot->setInteraction(QCP::iSelectItems, true);
    m_customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    m_customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    m_customPlot->legend->setVisible(true);

    // user defined curve control
    m_checkBoxCurve = ui->checkBox_Curve;
    QObject::connect(ui->checkBox_Curve,SIGNAL(toggled(bool)), this, SLOT(update()));

    m_lineEditList = QList<QLineEdit*>() << ui->lineEdit_C0 << ui->lineEdit_C1 << ui->lineEdit_C2 << ui->lineEdit_C3;
    for(int i = 0; i < m_lineEditList.size(); i++){
        QObject::connect(m_lineEditList[i],SIGNAL(textEdited(QString)),this, SLOT(update()));
    }


    // glassmap control
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

    // mouse
    QObject::connect(m_customPlot, SIGNAL(itemClick(QCPAbstractItem*,QMouseEvent*)), this, SLOT(showNeighbors(QCPAbstractItem*)));
    QObject::connect(m_customPlot,SIGNAL(mousePress(QMouseEvent*)),this, SLOT(clearNeighbors()));

    // context menu
    m_customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_customPlot, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu()));

    // fitting
    QObject::connect(ui->pushButton_Fitting, SIGNAL(clicked()), this, SLOT(showCurveFittingDlg()));

    // reset view button
    QObject::connect(ui->pushButton_resetView, SIGNAL(clicked()), this, SLOT(setDefault()));

    // preset
    QObject::connect(ui->pushButton_Preset, SIGNAL(clicked()), this, SLOT(showPresetDlg()));

    // window title
    this->setWindowTitle( xdataname + " - " + ydataname + " Plot");

    // Attribute
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    setDefault();
}

GlassMapForm::~GlassMapForm()
{
    try {
        delete m_settings;
    }  catch (...) {
        m_settings = nullptr;
    }
    m_settings = nullptr;

    m_customPlot->clearGraphs();
    m_customPlot->clearPlottables();
    m_customPlot->clearItems();
    m_customPlot = nullptr;

    m_glassMapCtrlList.clear();
    m_lineEditList.clear();

    m_catalogListPtr = nullptr;

    delete ui;
}

GlassMapForm::GlassMapCtrl::GlassMapCtrl(QCheckBox* p, QCheckBox* l)
{
    checkBoxPlot  = p;
    checkBoxLabel = l;
}

void GlassMapForm::showContextMenu()
{
    QMenu contextMenu;
    QAction *action1 = contextMenu.addAction("Export");
    QObject::connect(action1, SIGNAL(triggered()), this, SLOT(exportImage()));

    contextMenu.exec(QCursor::pos());
}


void GlassMapForm::exportImage()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save as"),"",tr("PNG file(*.png);;All Files(*.*)"));
    if(filePath.isEmpty()){
        return;
    }

    if(m_customPlot->savePng(filePath)){
        QMessageBox::information(this, "Success", "The image was successfully exported");
    }else{
        QMessageBox::warning(this, "Error", "Failed to export image");
    }
}

void GlassMapForm::setUpScrollArea()
{
    QGridLayout *gridLayout;
    QCheckBox *checkBox1, *checkBox2;
    QLabel *label;

    m_glassMapCtrlList.clear();

    //Dynamically create checkboxes
    gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_PlotControl"));

    for(int i = 0; i < m_catalogListPtr->size(); i++)
    {
        // supplyer name
        label = new QLabel(ui->scrollAreaWidgetContents);
        label->setObjectName("label_" + QString::number(i));
        label->setText(m_catalogListPtr->at(i)->supplyer());
        gridLayout->addWidget(label, i, 0, 1, 1);

        // plot on/off
        checkBox1 = new QCheckBox(ui->scrollAreaWidgetContents);
        checkBox1->setObjectName("chkPlot_"+QString::number(i));
        checkBox1->setText("P"); // point
        gridLayout->addWidget(checkBox1, i, 1, 1, 1);
        QObject::connect(checkBox1,SIGNAL(toggled(bool)), this, SLOT(update()));

        // label on/off
        checkBox2 = new QCheckBox(ui->scrollAreaWidgetContents);
        checkBox2->setObjectName("chkLabel_"+QString::number(i));
        checkBox2->setText("T"); // text label
        gridLayout->addWidget(checkBox2, i, 2, 1, 1);
        QObject::connect(checkBox2,SIGNAL(toggled(bool)), this, SLOT(update()));

        m_glassMapCtrlList.append(GlassMapCtrl(checkBox1,checkBox2));
    }

    ui->scrollArea->setWidgetResizable(true);
}

void GlassMapForm::setLegendVisible()
{
    m_customPlot->legend->setVisible(m_checkBoxLegend->checkState());
    m_customPlot->replot();
}

void GlassMapForm::showNeighbors(QCPAbstractItem* item)
{
    if(m_customPlot->selectedItems().isEmpty()){
        return;
    }

    // mouse-selected glass
    QString glassName = item->objectName();
    Glass* targetGlass = getGlassFromName(glassName);

    double xThreshold = (m_customPlot->xAxis->range().upper - m_customPlot->xAxis->range().lower)/10;
    double yThreshold = (m_customPlot->yAxis->range().upper - m_customPlot->yAxis->range().lower)/10;

    for(int i = 0; i < m_catalogListPtr->size(); i++){

        // Glasses in currently visible catalogs will be listed.
        if(m_glassMapCtrlList[i].checkBoxPlot->checkState())
        {
            GlassCatalog* cat = m_catalogListPtr->at(i);
            for(int j = 0; j < cat->glassCount(); j++)
            {
                Glass* g = cat->glass(j);

                double dx = (targetGlass->getValue(m_xDataName) - g->getValue(m_xDataName));
                double dy = (targetGlass->getValue(m_yDataName) - g->getValue(m_yDataName));

                if(abs(dx) < xThreshold && abs(dy) < yThreshold){
                    m_listWidgetNeighbors->addItem(g->name() + "_" + g->supplyer());
                }
            }
        }
    }

    m_listWidgetNeighbors->update();
}

void GlassMapForm::clearNeighbors()
{
    m_listWidgetNeighbors->clear();
    m_listWidgetNeighbors->update();
}

void GlassMapForm::showGlassDataSheet()
{
    if(m_listWidgetNeighbors->selectedItems().size() > 0)
    {
        QString     selectedText = m_listWidgetNeighbors->currentItem()->text();
        QStringList splitedText  = selectedText.split("_");  // format: glassname_catalogname
        QString     glassName    = splitedText[0];

        GlassDataSheetForm* subwindow = new GlassDataSheetForm(getGlassFromName(glassName), m_parentMdiArea);
        m_parentMdiArea->addSubWindow(subwindow);
        subwindow->parentWidget()->setGeometry(0,10, this->width()*1/2,this->height()*3/4);
        subwindow->show();
    }
}

void GlassMapForm::showCurveFittingDlg()
{
    CurveFittingDialog* dlg = new CurveFittingDialog(m_catalogListPtr, m_xDataName, m_yDataName, this);
    if(dlg->exec() == QDialog::Accepted)
    {
        QList<double> coefs = {0.0, 0.0, 0.0, 0.0};

        if(dlg->getFittingResult(coefs))
        {
            for(int i = 0; i < m_lineEditList.size(); i++){
                m_lineEditList[i]->setText(QString::number(coefs[i]));
            }
            this->update();
        }
        else{
            QMessageBox::warning(this,tr("File"), tr("Fitting calculation failed"));
        }
        coefs.clear();
    }

    delete dlg;
    dlg = nullptr;
}


void GlassMapForm::setDefault()
{
    m_customPlot->xAxis->setLabel(m_xDataName);
    m_customPlot->xAxis->setRange(m_defaultXrange);
    m_customPlot->xAxis->setRangeReversed(m_xReversed);

    m_customPlot->yAxis->setLabel(m_yDataName);
    m_customPlot->yAxis->setRange(m_defaultYrange);

    m_customPlot->replot();
}

void GlassMapForm::update()
{
    // delete all graphs and items
    m_customPlot->clearGraphs();
    m_customPlot->clearItems();
    m_customPlot->clearPlottables();

    int catalogCount = m_catalogListPtr->size();
    QCPScatterChart* glassmap;
    bool plot_on, label_on;

    // replot all glassmaps
    for(int i = 0; i < catalogCount; i++)
    {
        plot_on  = m_glassMapCtrlList[i].checkBoxPlot->checkState();
        label_on = m_glassMapCtrlList[i].checkBoxLabel->checkState();

        if(plot_on || label_on){
            glassmap = new QCPScatterChart(m_customPlot);
            setGlassmapData(glassmap, m_catalogListPtr->at(i), m_xDataName, m_yDataName, getColorFromIndex(i,catalogCount));
            glassmap->setVisiblePointSeries(plot_on);
            glassmap->setVisibleTextLabels(label_on);
        }
    }

    // replot user defined curve
    if(m_checkBoxCurve->checkState()){
        QCPGraph* curveGraph = m_customPlot->addGraph();
        setCurveData(curveGraph, getCurveCoefs());
        curveGraph->setVisible(true);
    }

    m_customPlot->replot();
}


Glass* GlassMapForm::getGlassFromName(QString glassName)
{
    for(auto &cat : *m_catalogListPtr){
        if(cat->hasGlass(glassName)){
            return cat->glass(glassName);
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

    delete dlg;
    dlg = nullptr;
}


void GlassMapForm::setGlassmapData(QCPScatterChart* glassmap,GlassCatalog* catalog, QString xlabel, QString ylabel, QColor color)
{
    int glassCount = catalog->glassCount();

    QVector<double> x, y;
    QVector<QString> s;
    x.reserve(glassCount);
    y.reserve(glassCount);
    s.reserve(glassCount);

    Glass* g;

    for(int i = 0; i < glassCount; i++)
    {
        g = catalog->glass(i);
        if("Unknown" == g->formulaName()){
            continue;
        }else{
            x.append(g->getValue(xlabel));
            y.append(g->getValue(ylabel));
            s.append(g->name());
        }
    }

    glassmap->setData(x,y,s);
    glassmap->setName(catalog->supplyer());
    glassmap->setColor(color);
}


void GlassMapForm::setCurveData(QCPGraph* curveGraph, const QList<double>& coefs)
{
    constexpr int dataCount = 100;
    QVector<double> x(dataCount),y(dataCount);

    double xmin = std::min(10.0,  m_customPlot->xAxis->range().lower);
    double xmax = std::max(100.0, m_customPlot->xAxis->range().upper);

    for(int i = 0; i < dataCount; i++)
    {
        x[i] = xmin + (xmax-xmin)*(double)i/dataCount;
        y[i] = 0;
        for(int j = 0;j < coefs.size(); j++){
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
    Q_ASSERT(4 == m_lineEditList.size());

    QList<double> coefs = {0,0,0,0};

    for(int i = 0; i < m_lineEditList.size(); i++){
        coefs[i] = m_lineEditList[i]->text().toDouble();
    }
    return coefs;
}

void GlassMapForm::setCurveCoefsToUI(const QList<double>& coefs)
{
    for(int i = 0; i < m_lineEditList.size(); i++){
        m_lineEditList[i]->setText(QString::number(coefs[i]));
    }
}

QColor GlassMapForm::getColorFromIndex(int index, int maxIndex)
{
    QCPColorGradient colorgrad;
    colorgrad.loadPreset(QCPColorGradient::gpHues);
    QColor color = colorgrad.color(index, QCPRange(0, maxIndex));

    return color;
}
