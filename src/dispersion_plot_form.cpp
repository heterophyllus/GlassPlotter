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

#include "dispersion_plot_form.h"
#include "ui_dispersion_plot_form.h"

#include "qcputil.h"
#include "dispersion_formula.h"
#include "glass.h"
#include "glass_catalog.h"
#include "glass_selection_dialog.h"


DispersionPlotForm::DispersionPlotForm(QList<GlassCatalog*> catalogList, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DispersionPlotForm)
{
    ui->setupUi(this);
    this->setWindowTitle("Dispersion Plot");

    // plot
    m_catalogList = catalogList;
    m_customPlot = ui->widget;
    m_customPlot->setInteractions(QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    m_customPlot->xAxis->setLabel("Wavelength(micron)");
    m_customPlot->yAxis->setLabel("Refractive Index");
    m_customPlot->legend->setVisible(true);

    // plot data table
    m_tablePlotData = ui->tableWidget;

    // buttons
    QObject::connect(ui->pushButton_AddGraph,   SIGNAL(clicked()), this, SLOT(addGraph()));
    QObject::connect(ui->pushButton_DeleteGraph,SIGNAL(clicked()), this, SLOT(deleteGraph()));
    QObject::connect(ui->pushButton_SetAxis,    SIGNAL(clicked()), this, SLOT(setAxis()));
    QObject::connect(ui->pushButton_Clear,      SIGNAL(clicked()), this, SLOT(clearAll()));

    // legend on/off
    QObject::connect(ui->checkBox_Legend,       SIGNAL(toggled(bool)), this, SLOT(setLegendVisible()));

    // user defined curve on/off
    m_checkBox = ui->checkBox_Curve;
    QObject::connect(ui->checkBox_Curve,        SIGNAL(toggled(bool)), this, SLOT(updateAll()));

    // select formula for user defined curve
    QStringList formulaNames;
    formulaNames.append("Polynomial");
    formulaNames.append(DispersionFormula::formulaNames());

    m_comboBoxFormula = ui->comboBox_Formula;
    m_comboBoxFormula->addItems(formulaNames);
    QObject::connect(m_comboBoxFormula,       SIGNAL(currentIndexChanged(int)), this, SLOT(updateAll()));

    // table widget to list up the coefficients
    m_tableCoefs = ui->tableWidget_Coefs;
    m_tableCoefs->setColumnCount(1);
    m_tableCoefs->setRowCount(12);
    m_tableCoefs->setHorizontalHeaderLabels(QStringList() << "val");
    QStringList vHeaderLabels;
    QTableWidgetItem* item;
    for(int i = 0; i < 12; i++){
        vHeaderLabels << "C" + QString::number(i);
        item = new QTableWidgetItem;
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
        m_tableCoefs->setItem(i,0,item);
        QObject::connect(m_tableCoefs,       SIGNAL(cellChanged(int,int)), this, SLOT(updateAll()));
    }
    m_tableCoefs->setVerticalHeaderLabels(vHeaderLabels);
    m_tableCoefs->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    //m_tableCoefs->resizeColumnsToContents();
    int whole_width = m_tableCoefs->width();
    m_tableCoefs->setColumnWidth(0, whole_width);
    m_tableCoefs->update();

    // plot step
    ui->lineEdit_PlotStep->setValidator(new QDoubleValidator(0, 100, 5, this));

    setDefault();
}

DispersionPlotForm::~DispersionPlotForm()
{
    m_catalogList.clear();
    m_glassList.clear();
    m_customPlot->clearGraphs();
    m_customPlot->clearItems();
    m_customPlot = nullptr;
    m_checkBox = nullptr;
    m_tablePlotData->clear();
    m_tableCoefs->clear();

    delete ui;
}

void DispersionPlotForm::addTableItem(int row, int col, QString str)
{
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setText(str);
    m_tablePlotData->setItem(row,col,item);
}


void DispersionPlotForm::setColorToGraph(QCPGraph* graph, QColor color)
{
    QPen pen;
    pen.setWidth(2);
    pen.setColor(color);
    graph->setPen(pen);
}


QVector<double> DispersionPlotForm::computeUserDefined(QVector<double> xdata)
{
    // get coefficients
    QVector<double> coefs(12);
    for(int i = 0; i < 12; i++){
        coefs[i] = m_tableCoefs->item(i,0)->text().toDouble();
    }


    // compute y data
    int npts = xdata.size();
    QVector<double> ydata(npts);

    int formulaNumber = m_comboBoxFormula->currentIndex();

    if(formulaNumber == 0) // polynomial
    {
        double x,y;
        for(int i = 0; i < npts; i++)
        {
            x = xdata[i];
            y = 0;
            for(int j = 0;j < coefs.size(); j ++)
            {
                y += coefs[j]*pow(x,j);
            }
            ydata[i] = y;
        }

    }
    else if(formulaNumber < 13) // formula in Zemax format
    {
        /*
         * This implementation is a bit tricky.
         *
         * A "dummy" glass object is created in order to utilize dispersion curve
         * without calling directly their definition.
         */

        Glass dummyGlass;
        dummyGlass.setDispForm(formulaNumber);
        for(int i = 0; i < m_tableCoefs->rowCount(); i++) {
            dummyGlass.setDispCoef(i,m_tableCoefs->item(i,0)->text().toDouble());
        }

        ydata = dummyGlass.index(xdata);
    }
    else // formula in CODEV format
    {
        Glass dummyGlass;
        dummyGlass.setDispForm(formulaNumber - 12 + 100 );
        for(int i = 0; i < m_tableCoefs->rowCount(); i++) {
            dummyGlass.setDispCoef(i,m_tableCoefs->item(i,0)->text().toDouble());
        }

        ydata = dummyGlass.index(xdata);
    }

    return ydata;
}


void DispersionPlotForm::addGraph()
{
    if(m_customPlot->graphCount() >= m_maxGraphCount+1) // 5 glass + 1 curve
    {
        QString message = "Up to " + QString::number(m_maxGraphCount) + " graphs can be plotted";
        QMessageBox::information(this,tr("Error"), message);
        return;
    }

    GlassSelectionDialog *dlg = new GlassSelectionDialog(m_catalogList, this);
    if(dlg->exec() == QDialog::Accepted)
    {
        // get glass
        int catalogIndex  = dlg->getCatalogIndex();
        QString glassName = dlg->getGlassName();
        Glass* newGlass   = m_catalogList.at(catalogIndex)->glass(glassName);

        // check dispersion formula of the glass
        if("Unknown" == newGlass->formulaName()){
            QMessageBox::information(this,tr("Error"), "Unknown dispersion formula");
        }
        else{
            m_glassList.append(newGlass);
            updateAll();
        }
        newGlass = nullptr;
    }

    try {
        delete dlg;
    }  catch (...) {
        dlg = nullptr;
    }
    dlg = nullptr;

}

void DispersionPlotForm::updateAll()
{
    m_customPlot->clearGraphs();
    m_tablePlotData->clear();

    double          plotStep      = ui->lineEdit_PlotStep->text().toDouble();
    QVector<double> vLambdamicron = QCPUtil::getVectorFromRange(m_customPlot->xAxis->range(), plotStep);
    int             dataCount     = vLambdamicron.size();
    QVector<double> ydata;
    QCPGraph*       graph;

    Glass* currentGlass;

    int rowCount    = dataCount;
    int columnCount = m_glassList.size() + 1+1; // wvl + glasses + curve
    m_tablePlotData->setRowCount(rowCount);
    m_tablePlotData->setColumnCount(columnCount);

    QStringList header = QStringList() << "WVL";


    // replot all graphs and recreate tables
    int digit = ui->spinBox_Digit->value();

    int i,j;
    for(i = 0; i < m_glassList.size(); i++)
    {
        currentGlass = m_glassList[i];

        // graphs
        ydata = currentGlass->index(vLambdamicron);
        graph = m_customPlot->addGraph();
        graph->setName(currentGlass->name() + "_" + currentGlass->supplyer());
        graph->setData(vLambdamicron, ydata);
        setColorToGraph(graph, QCPUtil::getColorFromIndex(i, m_maxGraphCount));
        graph->setVisible(true);

        // table
        header << currentGlass->name();
        for(j = 0; j< rowCount; j++)
        {
            addTableItem(j, 0,   QString::number(vLambdamicron[j], 'f', digit) );  // wavelength
            addTableItem(j, i+1, QString::number(ydata[j],         'f', digit) );  // refractive index
        }
    }

    // user defined curve
    header << "curve";
    if(m_checkBox->checkState())
    {
        ydata = computeUserDefined(vLambdamicron);
        graph = m_customPlot->addGraph();
        graph->setName("User Defined Curve");
        graph->setData(vLambdamicron, ydata);
        graph->setVisible(true);
        setColorToGraph(graph,Qt::black);

        for(i = 0; i < dataCount; i++)
        {
            addTableItem(i, columnCount-1, QString::number(ydata[i], 'f', digit) );
        }
    }

    m_tablePlotData->setHorizontalHeaderLabels(header);

    m_customPlot->replot();
}

void DispersionPlotForm::deleteGraph()
{
    if(m_customPlot->selectedGraphs().size() > 0)
    {
        QCPGraph* selectedGraph = m_customPlot->selectedGraphs().at(0);
        QString graphName = selectedGraph->name();
        QStringList glass_supplyer = graphName.split("_");

        int glassCount = m_glassList.size();

        for(int i = 0;i < glassCount; i++)
        {
            if(m_glassList[i]->name() == glass_supplyer[0] && m_glassList[i]->supplyer() == glass_supplyer[1]){
                m_glassList.removeAt(i);
                break;
            }
        }
        updateAll();
    }
}

void DispersionPlotForm::setDefault()
{
    QCPRange xrange = QCPRange(0.3,1.0); // micron
    QCPRange yrange = QCPRange(0.9,2.1);
    m_customPlot->xAxis->setRange(xrange);
    m_customPlot->yAxis->setRange(yrange);

    ui->lineEdit_Xmin->setText(QString::number(xrange.lower));
    ui->lineEdit_Xmax->setText(QString::number(xrange.upper));
    ui->lineEdit_Ymin->setText(QString::number(yrange.lower));
    ui->lineEdit_Ymax->setText(QString::number(yrange.upper));

    ui->lineEdit_PlotStep->setText(QString::number(0.005));
}

void DispersionPlotForm::setAxis()
{
    QCPRange xrange, yrange;
    xrange.lower = ui->lineEdit_Xmin->text().toDouble();
    xrange.upper = ui->lineEdit_Xmax->text().toDouble();
    yrange.lower = ui->lineEdit_Ymin->text().toDouble();
    yrange.upper = ui->lineEdit_Ymax->text().toDouble();

    m_customPlot->xAxis->setRange(xrange);
    m_customPlot->yAxis->setRange(yrange);
    updateAll();
}

void DispersionPlotForm::clearAll()
{
    m_glassList.clear();
    m_customPlot->clearGraphs();
    m_customPlot->clearItems();
    m_customPlot->clearPlottables();
    m_customPlot->replot();
    m_tablePlotData->clear();

    m_checkBox->setCheckState(Qt::Unchecked);
    for(int i = 0;i < m_tableCoefs->rowCount(); i++){
        QObject::disconnect(m_tableCoefs,       SIGNAL(cellChanged(int,int)), this, SLOT(updateAll()));
        m_tableCoefs->item(i,0)->setText("");
        QObject::connect(m_tableCoefs,       SIGNAL(cellChanged(int,int)), this, SLOT(updateAll()));
    }
}

void DispersionPlotForm::setLegendVisible()
{
    m_customPlot->legend->setVisible(ui->checkBox_Legend->checkState());
    m_customPlot->replot();
}
