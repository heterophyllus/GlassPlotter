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

#include "dispersionplotform.h"
#include "ui_dispersionplotform.h"

DispersionPlotForm::DispersionPlotForm(QList<GlassCatalog*> catalogList, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DispersionPlotForm)
{
    ui->setupUi(this);

    m_catalogList = catalogList;
    m_plottedGraphList.clear();
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

    m_curveCtrl = new CurveCtrl(this);
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

    for(int i = 0; i < m_plottedGraphList.size(); i++){
        m_customPlot->removeGraph(m_plottedGraphList[i]->graph);
    }

    delete ui;
}


DispersionPlotForm::CurveCtrl::CurveCtrl(DispersionPlotForm *super)
{
    m_super = super;
    m_super -> m_customPlot = super->m_customPlot;
    graph = m_super->m_customPlot->addGraph();
    graph->setName("curve");

    coefs.clear();
    for(int i = 0; i < 5; i++) coefs.append(0);
}

void DispersionPlotForm::CurveCtrl::setData()
{
    if(!graph) return;

    coefs[0] = m_super->ui->lineEdit_C0->text().toDouble();
    coefs[1] = m_super->ui->lineEdit_C1->text().toDouble();
    coefs[2] = m_super->ui->lineEdit_C2->text().toDouble();
    coefs[3] = m_super->ui->lineEdit_C3->text().toDouble();
    coefs[4] = m_super->ui->lineEdit_C4->text().toDouble();

    xdata.clear();
    ydata.clear();

    double x,y;
    double lambdamin = m_super->m_xrange.lower;
    double lambdamax = m_super->m_xrange.upper;
    double lambdanano = lambdamin;

    while(lambdanano < lambdamax)
    {
        x = lambdanano;
        xdata.append(x);

        y = 0;
        for(int j = 0;j < coefs.size(); j ++)
        {
            y += coefs[j]*pow(x,j);
        }
        ydata.append(y);

        lambdanano += m_super->plotStep;
    }


    graph->setData(xdata,ydata);
    graph->setName("curve");

    QPen pen;
    pen.setColor(Qt::black); //black
    graph->setPen(pen);

}

void DispersionPlotForm::CurveCtrl::setVisible(bool state)
{
    graph->setVisible(state);
}


DispersionPlotForm::PlottedGraph::PlottedGraph(DispersionPlotForm *super)
{
    m_super = super;
}

DispersionPlotForm::PlottedGraph::~PlottedGraph()
{
    m_super = nullptr;
}

void DispersionPlotForm::PlottedGraph::setData(QCPRange xrange)
{
    xdata.clear();
    ydata.clear();

    double x,y;

    double lambdamin = xrange.lower;
    double lambdamax = xrange.upper;
    double lambdanano = lambdamin;

    while(lambdanano < lambdamax)
    {
        x = lambdanano;
        y = glass->index(lambdanano/1000);

        xdata.append(x);
        ydata.append(y);
        lambdanano += m_super->plotStep;
    }

    graph->setData(xdata,ydata);
    graph->setName(glass->name() + "_" + glass->supplyer() );
    //graph->setName(glass->name());
    graph->setVisible(true);
}

void DispersionPlotForm::PlottedGraph::setColor(int index)
{
    QCPColorGradient colorgrad;
    colorgrad.loadPreset(QCPColorGradient::gpHues);
    QColor color = colorgrad.color(index, QCPRange(0, m_super->maxGraphCount));

    QPen pen;
    pen.setWidth(2);
    pen.setColor(color);
    graph->setPen(pen);
}

void DispersionPlotForm::addGraph()
{
    if(m_customPlot->graphCount() >= maxGraphCount+1) // 5 glass + 1 curve
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
        QCPGraph* newGraph = m_customPlot->addGraph();

        PlottedGraph *plottedGraph = new PlottedGraph(this);
        plottedGraph->name = newGlass->name() + "_" + newGlass->supplyer();
        plottedGraph->glass = newGlass;
        plottedGraph->graph = newGraph;
        m_plottedGraphList.append(plottedGraph);

        updateAll();
    }

}


void DispersionPlotForm::updateAll()
{

    // plot
    for(int i = 0; i < m_plottedGraphList.size(); i++)
    {
        m_plottedGraphList[i]->setData(m_xrange);
        m_plottedGraphList[i]->setColor(i);
    }

    m_curveCtrl->setData();
    m_curveCtrl->setVisible(m_checkBox->checkState());

    m_customPlot->replot();

    if(m_customPlot->graphCount() == 1){ // only user defined curve
        m_table->clear();
        return;
    }

    // table
    int rowCount = m_plottedGraphList[0]->xdata.size();
    int columnCount = m_plottedGraphList.size() + 1+1; // lambda + glasses
    m_table->clear();
    m_table->setRowCount(rowCount);
    m_table->setColumnCount(columnCount);

    QStringList header = QStringList() << "WVL";
    for(int j = 0;j<m_plottedGraphList.size();j++)
    {
        header << m_plottedGraphList[j]->name;
    }
    header << "curve";

    m_table->setHorizontalHeaderLabels(header);

    QTableWidgetItem* item;
    for(int i = 0; i< rowCount; i++)
    {
        // wavelength
        item = new QTableWidgetItem;
        item->setText(QString::number(m_plottedGraphList[0]->xdata[i]));
        m_table->setItem(i, 0, item);

        // refractive indices
        for(int j = 1; j<columnCount-1; j++)
        {
            item = new QTableWidgetItem;
            item->setText( QString::number(m_plottedGraphList[j-1]->ydata[i]) );
            m_table->setItem(i, j, item);
        }

        // curve
        int j = columnCount-1;
        if(m_checkBox->checkState()){
            item = new QTableWidgetItem;
            item->setText( QString::number(m_curveCtrl->ydata[i]) );
            m_table->setItem(i, j, item);
        }

    }

}


void DispersionPlotForm::deleteGraph()
{
    if(m_customPlot->selectedGraphs().size() > 0)
    {
        QCPGraph* selectedGraph = m_customPlot->selectedGraphs().first();
        QString graphName = selectedGraph->name();

        for(int i = 0;i < m_plottedGraphList.size(); i++){
            if(m_plottedGraphList.at(i)->name == graphName){
                m_customPlot->removeGraph(m_plottedGraphList[i]->graph);
                m_plottedGraphList.removeAt(i);
                break;
            }
        }

        updateAll();
    }
}

void DispersionPlotForm::setDefault()
{
    m_xrange = QCPRange(300,1000);
    m_yrange = QCPRange(0.9,2.1);

    ui->lineEdit_Xmin->setText(QString::number(m_xrange.lower));
    ui->lineEdit_Xmax->setText(QString::number(m_xrange.upper));
    ui->lineEdit_Ymin->setText(QString::number(m_yrange.lower));
    ui->lineEdit_Ymax->setText(QString::number(m_yrange.upper));

    m_customPlot->xAxis->setRange(m_xrange);
    m_customPlot->yAxis->setRange(m_yrange);
}

void DispersionPlotForm::setAxis()
{
    m_xrange.lower = ui->lineEdit_Xmin->text().toDouble();
    m_xrange.upper = ui->lineEdit_Xmax->text().toDouble();
    m_yrange.lower = ui->lineEdit_Ymin->text().toDouble();
    m_yrange.upper = ui->lineEdit_Ymax->text().toDouble();

    m_customPlot->xAxis->setRange(m_xrange);
    m_customPlot->yAxis->setRange(m_yrange);

    updateAll();
}

void DispersionPlotForm::clearAll()
{
    for(int i = 0; i < m_plottedGraphList.size();i++){
        try {
            delete m_plottedGraphList[i];
        } catch (...) {
            m_plottedGraphList[i] = nullptr;
        }
    }
    m_plottedGraphList.clear();
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
