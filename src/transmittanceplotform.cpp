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


#include "transmittanceplotform.h"
#include "ui_transmittanceplotform.h"

TransmittancePlotForm::TransmittancePlotForm(QList<GlassCatalog*> catalogList, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransmittancePlotForm)
{
    ui->setupUi(this);

    m_catalogList = catalogList;

    m_customPlot = ui->widget;
    m_customPlot->setInteractions(QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    m_customPlot->xAxis->setLabel("Wavelength(nm)");
    m_customPlot->yAxis->setLabel("Internal Transmittance");
    m_customPlot->legend->setVisible(true);
    m_customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom); // Legend position

    QObject::connect(ui->pushButton_AddGraph,   SIGNAL(clicked()),     this, SLOT(addGraph()));
    QObject::connect(ui->pushButton_DeleteGraph,SIGNAL(clicked()),     this, SLOT(deleteGraph()));
    QObject::connect(ui->pushButton_SetAxis,    SIGNAL(clicked()),     this, SLOT(setAxis()));
    QObject::connect(ui->pushButton_Clear,      SIGNAL(clicked()),     this, SLOT(clearAll()));
    QObject::connect(ui->checkBox_Legend,       SIGNAL(toggled(bool)), this, SLOT(setLegendVisible()));

    m_table = ui->tableWidget;
    setDefault();
}

TransmittancePlotForm::~TransmittancePlotForm()
{
    delete ui;
}

QVector<double> TransmittancePlotForm::scaleVector(QVector<double> v, double scale)
{
    QVector<double> vv;
    vv = v;

    for(int i = 0; i < v.size(); i++){
        vv[i] = v[i]*scale;
    }
    return vv;
}

QCPRange TransmittancePlotForm::currentXrange()
{
    return m_customPlot->xAxis->range();
}

QCPRange TransmittancePlotForm::currentYrange()
{
    return m_customPlot->yAxis->range();
}


QVector<double> TransmittancePlotForm::getVectorFromRange(QCPRange range)
{
    QVector<double> xdata;
    double lambdamin = range.lower;
    double lambdamax = range.upper;
    double lambdanano = lambdamin;
    while(lambdanano < lambdamax)
    {
        xdata.append(lambdanano/1000);
        lambdanano += plotStep;
    }
    return xdata;
}


QColor TransmittancePlotForm::getColorFromIndex(int i)
{
    QCPColorGradient colorgrad;
    colorgrad.loadPreset(QCPColorGradient::gpHues);
    QColor color = colorgrad.color(i, QCPRange(0, maxGraphCount));

    return color;
}

void TransmittancePlotForm::setColorToGraph(QCPGraph* graph, QColor color)
{
    QPen pen;
    pen.setWidth(2);
    pen.setColor(color);
    graph->setPen(pen);
}

void TransmittancePlotForm::addGraph()
{
    if(m_customPlot->graphCount() >= maxGraphCount) // 5 glass
    {
        QString message = "Up to " + QString::number(maxGraphCount) + " graphs can be plotted";
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


void TransmittancePlotForm::updateAll()
{
    m_customPlot->clearGraphs();
    m_table->clear();

    double thickness = ui->lineEdit_Thickness->text().toDouble();

    QVector<double> xdata = getVectorFromRange(currentXrange());
    QVector<double> ydata;
    QCPGraph* graph;
    QCPItemTracer* upperTracer;
    QCPItemTracer* lowerTracer;

    int i,j;
    int rowCount = xdata.size();
    int columnCount = m_glassList.size() + 1+1; // lambda + glasses
    m_table->setRowCount(rowCount);
    m_table->setColumnCount(columnCount);

    QStringList header = QStringList() << "WVL";
    QTableWidgetItem* item;

    Glass* currentGlass;

    // replot all graphs and recreate tables
    for(i = 0; i < m_glassList.size(); i++)
    {
        currentGlass = m_glassList[i];

        // graphs
        ydata = currentGlass->transmittance(xdata, thickness);
        graph = m_customPlot->addGraph();
        graph->setName(currentGlass->name() + "_" + currentGlass->supplyer());
        graph->setData(scaleVector(xdata,1000), ydata);
        setColorToGraph(graph, getColorFromIndex(i));
        graph->setVisible(true);

        // tracer
        upperTracer = new QCPItemTracer(m_customPlot);
        upperTracer->setGraph(graph);
        upperTracer->setInterpolating(true);
        upperTracer->setStyle(QCPItemTracer::tsCircle);
        upperTracer->setSize(7);
        upperTracer->setPen(graph->pen());
        upperTracer->setGraphKey(currentGlass->lambdaMax());
        upperTracer->updatePosition();

        lowerTracer = new QCPItemTracer(m_customPlot);
        lowerTracer->setGraph(graph);
        lowerTracer->setInterpolating(true);
        lowerTracer->setStyle(QCPItemTracer::tsCircle);
        lowerTracer->setSize(7);
        lowerTracer->setPen(graph->pen());
        lowerTracer->setGraphKey(currentGlass->lambdaMin());
        lowerTracer->updatePosition();

        // table
        header << currentGlass->name();
        for(j = 0; j< rowCount; j++)
        {
            // wavelength
            item = new QTableWidgetItem;
            item->setText(QString::number( scaleVector(xdata, 1000).at(j) ) );
            m_table->setItem(j, 0, item);

            // refractive indices
            item = new QTableWidgetItem;
            item->setText( QString::number(ydata[j]) );
            m_table->setItem(j, i+1, item);
        }
    }

    m_customPlot->replot();
    m_table->setHorizontalHeaderLabels(header);

}
void TransmittancePlotForm::deleteGraph()
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

void TransmittancePlotForm::setDefault()
{
    QCPRange xrange = QCPRange(300,2000);
    QCPRange yrange = QCPRange(0.0,1.2);

    m_customPlot->xAxis->setRange(xrange);
    m_customPlot->yAxis->setRange(yrange);

    ui->lineEdit_Xmin->setText(QString::number(xrange.lower));
    ui->lineEdit_Xmax->setText(QString::number(xrange.upper));
    ui->lineEdit_Ymin->setText(QString::number(yrange.lower));
    ui->lineEdit_Ymax->setText(QString::number(yrange.upper));

    ui->lineEdit_Thickness->setText(QString::number(25));
}

void TransmittancePlotForm::setAxis()
{
    QCPRange xrange, yrange;
    xrange.lower = ui->lineEdit_Xmin->text().toDouble();
    xrange.upper = ui->lineEdit_Xmax->text().toDouble();
    yrange.lower = ui->lineEdit_Ymin->text().toDouble();
    yrange.upper = ui->lineEdit_Ymax->text().toDouble();

    m_customPlot->xAxis->setRange(xrange);
    m_customPlot->yAxis->setRange(yrange);

    updateAll();
    m_customPlot->replot();
}

void TransmittancePlotForm::clearAll()
{
    m_glassList.clear();
    m_customPlot->clearGraphs();
    m_customPlot->clearItems();
    m_customPlot->replot();
    m_table->clear();
    m_table->update();
}

void TransmittancePlotForm::setLegendVisible()
{
    m_customPlot->legend->setVisible(ui->checkBox_Legend->checkState());
    m_customPlot->replot();
}
