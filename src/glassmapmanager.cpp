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

#include "glassmapmanager.h"

GlassMapManager::GlassMapManager(QCustomPlot* customPlot)
{
    _customPlot = customPlot;

    QCPRange xrange, yrange;

    // vd-nd
    xrange.lower = 10;
    xrange.upper = 100;
    yrange.lower = 1.4;
    yrange.upper = 2.1;
    _defaultRanges[0].first  = xrange;
    _defaultRanges[0].second = yrange;

    // ve-ne
    xrange.lower = 10;
    xrange.upper = 100;
    yrange.lower = 1.4;
    yrange.upper = 2.1;
    _defaultRanges[1].first  = xrange;
    _defaultRanges[1].second = yrange;

    //vd-PgF
    yrange.lower = 0.5;
    yrange.upper = 0.7;
    _defaultRanges[2].first  = xrange;
    _defaultRanges[2].second = yrange;

    _pCurveGraph = _customPlot->addGraph();

    _customPlot->legend->setVisible(false);
}


int GlassMapManager::catalogCount()
{
    return _catalogs.count();
}

GlassCatalog* GlassMapManager::catalog(int index)
{
    return _catalogs[index];
}

bool GlassMapManager::readAllAGF(QString agfdir)
{
    GlassCatalog *catalog;
    _catalogs.clear();

    QStringList nameFilters;
    nameFilters.append("*.AGF");

    QDir dir(agfdir);
    QStringList filelist = dir.entryList(nameFilters, QDir::Files);
    if(filelist.empty())
    {
        qDebug() << "AGF dir is empty";
        return false;
    }

    QString fullpath;
    for (int i = 0; i < filelist.size(); i++) {
        fullpath = dir.filePath(filelist[i]);
        catalog = new GlassCatalog;
        if(!catalog->loadAGF(fullpath))
        {
            delete catalog;
            return false;
        }else{
            _catalogs.append(catalog);
        }
    }

    return true;
}

QColor GlassMapManager::getColor(QString supplyer)
{
    //get index
    int index = 0;
    for(int i = 0;i < _catalogs.size(); i++)
    {
        if(_catalogs[i]->supplyer() == supplyer)
        {
            index = i;
            break;
        }
    }

    //get color from colormap
    QCPColorGradient colorgrad;
    colorgrad.loadPreset(QCPColorGradient::gpHues);
    QColor color;
    color = colorgrad.color(index, QCPRange(0,_catalogs.count()));

    return color;
}
void GlassMapManager::createGlassMapList(int plotType)
{
    GlassCatalog *catalog;
    QCPScatterChart *chart;

    QVector<double> x,y;
    QVector<QString> str;

    for(int j = 0; j < _catalogs.count();j++)
    {
        catalog = _catalogs[j];

        x.clear();
        y.clear();
        str.clear();

        switch(plotType)
        {
        case 0: //vd-nd
            _customPlot->xAxis->setLabel("vd");
            _customPlot->yAxis->setLabel("nd");
            for(int i = 0; i < catalog->glassCount(); i++)
            {
                x.append(catalog->glass(i)->vd());           
                y.append(catalog->glass(i)->nd());
                str.append(catalog->glass(i)->name());

                //qDebug("%s: %s: %f, %f",catalog->supplyer().toUtf8().data(),str.last().toUtf8().data(), x.last(),y.last());
            }
            break;
        case 1:
            _customPlot->xAxis->setLabel("ve");
            _customPlot->yAxis->setLabel("ne");
            for(int i = 0; i < catalog->glassCount(); i++)
            {
                x.append(catalog->glass(i)->ve());
                y.append(catalog->glass(i)->ne());
                str.append(catalog->glass(i)->name());

                //qDebug("%s: %s: %f, %f",catalog->supplyer().toUtf8().data(),str.last().toUtf8().data(), x.last(),y.last());
            }
            break;
        case 2: //ve-ne
            _customPlot->xAxis->setLabel("vd");
            _customPlot->yAxis->setLabel("PgF");
            for(int i = 0; i < catalog->glassCount(); i++)
            {
                x.append(catalog->glass(i)->vd());
                y.append(catalog->glass(i)->PgF());
                str.append(catalog->glass(i)->name());

                //qDebug("%s: %s: %f, %f",catalog->supplyer().toUtf8().data(),str.last().toUtf8().data(), x.last(),y.last());
            }
            break;
        }

        chart = new QCPScatterChart(_customPlot);
        chart->setData(x,y,str);
        chart->setName(catalog->supplyer());
        chart->setColor(getColor(catalog->supplyer()));
        _glassmaps.insert(catalog->supplyer(),chart);
        //delete chart;
    }

}

void GlassMapManager::clearGlassMapList()
{
    //_customPlot->clearGraphs();
    for(int i = 0; i < _catalogs.size(); i++)
    {
        _customPlot->removeGraph(_glassmaps[_catalogs[i]->supplyer()]->pointSeries());
    }

    _customPlot->clearItems();
    _glassmaps.clear();
}

void GlassMapManager::setCurveCoefs(QList<double> coefs)
{
    QVector<double> x(101),y(101);
    double xmin, xmax;

    xmin = _defaultRanges[_currentPlotType].first.lower;
    xmax = _defaultRanges[_currentPlotType].first.upper;

    for(int i = 0; i<101; i++)
    {
        x[i] = xmin + (xmax-xmin)*(double)i/100;

        y[i] = 0;
        for(int j = 0;j < coefs.count(); j ++)
        {
            y[i] += coefs[j]*pow(x[i],j);
        }
    }

    _pCurveGraph->setData(x,y);
    _pCurveGraph->setName("curve");

    QPen pen;
    pen.setColor(Qt::black); //black
    _pCurveGraph->setPen(pen);

}

void GlassMapManager::setChartVisible(QString supplyer,bool pointstate, bool labelstate)
{
    _glassmaps[supplyer]->setVisiblePointSeries(pointstate);
    _glassmaps[supplyer]->setVisibleTextLabels(labelstate);
}
void GlassMapManager::setCurveVisible(bool state)
{
    _pCurveGraph->setVisible(state);
}

void GlassMapManager::setAxis(QCPRange xrange, QCPRange yrange)
{
    _customPlot->xAxis->setRange(xrange);
    _customPlot->yAxis->setRange(yrange);
}

void GlassMapManager::resetAxis(int plotType)
{
    QCPRange xrange = _defaultRanges[plotType].first;
    QCPRange yrange = _defaultRanges[plotType].second;

    _customPlot->xAxis->setRangeReversed(true);
    _customPlot->xAxis->setRange(xrange);
    _customPlot->yAxis->setRange(yrange);
}

void GlassMapManager::replot()
{
    _customPlot->replot();
}
