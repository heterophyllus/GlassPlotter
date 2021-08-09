#include "property_plot_form.h"

PropertyPlotForm::PropertyPlotForm(QWidget *parent):
    QWidget(parent)
{
    // Attribute
    this->setAttribute(Qt::WA_DeleteOnClose, true);
}

void PropertyPlotForm::showContextMenu()
{
    QMenu contextMenu;
    QAction *action1 = contextMenu.addAction("Export");
    QObject::connect(action1, SIGNAL(triggered()), this, SLOT(exportImage()));

    contextMenu.exec(QCursor::pos());
}

void PropertyPlotForm::exportImage()
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

void PropertyPlotForm::setAxis()
{
    QCPRange xrange, yrange;
    xrange.lower = m_editXmin->text().toDouble();
    xrange.upper = m_editXmax->text().toDouble();
    yrange.lower = m_editYmin->text().toDouble();
    yrange.upper = m_editYmax->text().toDouble();

    m_customPlot->xAxis->setRange(xrange);
    m_customPlot->yAxis->setRange(yrange);
    updateAll();
}

void PropertyPlotForm::setDefault()
{
    m_customPlot->xAxis->setRange(m_defaultXrange);
    m_customPlot->yAxis->setRange(m_defaultYrange);

    m_editXmin->setText(QString::number(m_defaultXrange.lower));
    m_editXmax->setText(QString::number(m_defaultXrange.upper));
    m_editYmin->setText(QString::number(m_defaultYrange.lower));
    m_editYmax->setText(QString::number(m_defaultYrange.upper));
}

void PropertyPlotForm::setLegendVisible()
{
    m_customPlot->legend->setVisible(m_chkLegend->checkState());
    m_customPlot->replot();
}


void PropertyPlotForm::addTableItem(int row, int col, QString str)
{
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setText(str);
    m_plotDataTable->setItem(row,col,item);
}

QColor PropertyPlotForm::getColorFromIndex(int index, int maxIndex)
{
    QCPColorGradient colorgrad;
    colorgrad.loadPreset(QCPColorGradient::gpHues);
    QColor color = colorgrad.color(index, QCPRange(0, maxIndex));

    return color;
}

QVector<double> PropertyPlotForm::scaleVector(const QVector<double>& v, double scale)
{
    int len = v.size();
    QVector<double> sv(len);

    for(int i = 0; i < len; i++){
        sv[i] = v[i]*scale;
    }
    return sv;
}

QVector<double> PropertyPlotForm::getVectorFromRange(QCPRange range, double step)
{
    double xmin = range.lower;
    double xmax = range.upper;
    QVector<double> xdata;
    xdata.reserve(floor((xmax - xmin)/step) + 1 );
    double x = xmin;
    while(x <= xmax)
    {
        xdata.append(x);
        x += step;
    }
    return xdata;
}
