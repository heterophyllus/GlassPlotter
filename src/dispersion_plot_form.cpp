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
#include "ui_dispersionplotform.h"

DispersionPlotForm::DispersionPlotForm(QList<GlassCatalog*> catalogList, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DispersionPlotForm)
{
    ui->setupUi(this);
    this->setWindowTitle("Dispersion Plot");

    m_catalogList = catalogList;
    m_customPlot = ui->widget;
    m_customPlot->setInteractions(QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    m_customPlot->xAxis->setLabel("Wavelength(nm)");
    m_customPlot->yAxis->setLabel("Refractive Index");
    m_customPlot->legend->setVisible(true);

    m_table = ui->tableWidget;

    QObject::connect(ui->pushButton_AddGraph,   SIGNAL(clicked()), this, SLOT(addGraph()));
    QObject::connect(ui->pushButton_DeleteGraph,SIGNAL(clicked()), this, SLOT(deleteGraph()));
    QObject::connect(ui->pushButton_SetAxis,    SIGNAL(clicked()), this, SLOT(setAxis()));
    QObject::connect(ui->pushButton_Clear,      SIGNAL(clicked()), this, SLOT(clearAll()));
    QObject::connect(ui->checkBox_Legend,       SIGNAL(toggled(bool)), this, SLOT(setLegendVisible()));

    m_checkBox = ui->checkBox_Curve;
    QObject::connect(ui->checkBox_Curve,       SIGNAL(toggled(bool)), this, SLOT(updateAll()));
    QObject::connect(ui->lineEdit_C0,          SIGNAL(textEdited(QString)), this, SLOT(updateAll()));
    QObject::connect(ui->lineEdit_C1,          SIGNAL(textEdited(QString)), this, SLOT(updateAll()));
    QObject::connect(ui->lineEdit_C2,          SIGNAL(textEdited(QString)), this, SLOT(updateAll()));
    QObject::connect(ui->lineEdit_C3,          SIGNAL(textEdited(QString)), this, SLOT(updateAll()));
    QObject::connect(ui->lineEdit_C4,          SIGNAL(textEdited(QString)), this, SLOT(updateAll()));

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
    m_table = nullptr;

    delete ui;
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
    QVector<double> coefs(5);
    coefs[0] = ui->lineEdit_C0->text().toDouble();
    coefs[1] = ui->lineEdit_C1->text().toDouble();
    coefs[2] = ui->lineEdit_C2->text().toDouble();
    coefs[3] = ui->lineEdit_C3->text().toDouble();
    coefs[4] = ui->lineEdit_C4->text().toDouble();

    double x,y;
    QVector<double> ydata;
    ydata.clear();

    for(int i = 0; i < xdata.size(); i++)
    {
        x = xdata[i];
        y = 0;
        for(int j = 0;j < coefs.size(); j ++)
        {
            y += coefs[j]*pow(x,j);
        }
        ydata.append(y);
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
        int catalogIndex = dlg->getCatalogIndex();
        QString glassName = dlg->getGlassName();
        Glass* newGlass = m_catalogList.at(catalogIndex)->glass(glassName);

        m_glassList.append(newGlass);

        updateAll();
    }

}

void DispersionPlotForm::updateAll()
{
    m_customPlot->clearGraphs();
    m_table->clear();

    QVector<double> xdata = QCPUtil::getVectorFromRange(m_customPlot->xAxis->range(), m_plotStep); // unit:nm
    QVector<double> ydata;
    QCPGraph* graph;

    Glass* currentGlass;

    int rowCount = xdata.size();
    int columnCount = m_glassList.size() + 1+1; // lambda + glasses
    m_table->setRowCount(rowCount);
    m_table->setColumnCount(columnCount);

    QStringList header = QStringList() << "WVL";
    QTableWidgetItem* item;

    int i,j;


    // replot all graphs and recreate tables
    for(i = 0; i < m_glassList.size(); i++)
    {
        currentGlass = m_glassList[i];

        // graphs
        ydata = currentGlass->index(QCPUtil::scaleVector(xdata,1/1000));
        graph = m_customPlot->addGraph();
        graph->setName(currentGlass->name() + "_" + currentGlass->supplyer());
        graph->setData(xdata, ydata);
        setColorToGraph(graph, QCPUtil::getColorFromIndex(i, m_maxGraphCount));
        graph->setVisible(true);

        // table
        header << currentGlass->name();
        for(j = 0; j< rowCount; j++)
        {
            // wavelength
            item = new QTableWidgetItem;
            item->setText(QString::number( QCPUtil::scaleVector(xdata, 1000).at(j) ) );
            m_table->setItem(j, 0, item);

            // refractive indices
            item = new QTableWidgetItem;
            item->setText( QString::number(ydata[j]) );
            m_table->setItem(j, i+1, item);
        }
    }

    // user defined curve
    header << "curve";
    m_table->setHorizontalHeaderLabels(header);

    ydata = computeUserDefined(QCPUtil::scaleVector(xdata,1/1000));
    graph = m_customPlot->addGraph();
    graph->setName("User Defined Curve");
    graph->setData(xdata, ydata);
    graph->setVisible(m_checkBox->checkState());
    setColorToGraph(graph,Qt::black);

    if(m_checkBox->checkState()){
        for(i = 0; i < xdata.size(); i++)
        {
            item = new QTableWidgetItem;
            item->setText( QString::number(ydata[i]) );
            m_table->setItem(i, columnCount-1, item);
        }
    }

    m_customPlot->replot();

}

void DispersionPlotForm::deleteGraph()
{
    if(m_customPlot->selectedGraphs().size() > 0)
    {
        QCPGraph* selectedGraph = m_customPlot->selectedGraphs().first();
        QString graphName = selectedGraph->name();
        QStringList glass_supplyer = graphName.split("_");

        for(int i = 0;i < m_glassList.size(); i++){
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
    QCPRange xrange = QCPRange(300,1000);
    QCPRange yrange = QCPRange(0.9,2.1);
    m_customPlot->xAxis->setRange(xrange);
    m_customPlot->yAxis->setRange(yrange);

    ui->lineEdit_Xmin->setText(QString::number(xrange.lower));
    ui->lineEdit_Xmax->setText(QString::number(xrange.upper));
    ui->lineEdit_Ymin->setText(QString::number(yrange.lower));
    ui->lineEdit_Ymax->setText(QString::number(yrange.upper));
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
    m_customPlot->replot();
    m_table->clear();
    m_table->update();
}

void DispersionPlotForm::setLegendVisible()
{
    m_customPlot->legend->setVisible(ui->checkBox_Legend->checkState());
    m_customPlot->replot();
}
