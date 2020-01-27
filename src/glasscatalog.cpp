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

#include "glasscatalog.h"

GlassCatalog::GlassCatalog()
{
    _glasses.clear();
    _supplyer = "";
}

GlassCatalog::~GlassCatalog()
{
    _glasses.clear();
    _supplyer = "";
}

void GlassCatalog::clear()
{
    _glasses.clear();
    _supplyer = "";
}


bool GlassCatalog::loadAGF(QString AGFpath)
{
    qDebug("Loading : %s", AGFpath.toUtf8().data());

    QFile file(AGFpath);
    if (! file.open(QIODevice::ReadOnly)) {
        qDebug() << "File Open Error";
        return false;
    }

    QTextStream in(&file);
    QString linetext;
    QStringList lineparts;
    QList<double> dlist;

    Glass *g;


    _glasses.clear();
    _supplyer = QFileInfo(AGFpath).baseName();

    while (! in.atEnd()) {
        linetext = in.readLine();
        //qDebug() << linetext;

        if(linetext.startsWith("NM")){
            g = new Glass;
            _glasses.append(g);

            lineparts = linetext.simplified().split(" ");

            _glasses.last()->setName(lineparts[1]);
            _glasses.last()->setDispForm(lineparts[2].toInt());
            _glasses.last()->setNd(lineparts[4].toDouble());
            _glasses.last()->setVd(lineparts[5].toDouble());
            //_glasses.last().setExcludeSub();
        }
        else if (linetext.startsWith("ED")) {
            lineparts = linetext.simplified().split(" ");
            //g.setTCE(lineparts[1].toDouble());
            //g.setDensity(lineparts[3].toDouble());
            //_glasses.last()->setDeltaPgF(lineparts[4].toDouble());
            //g.setIgnoreThermalExp()
        }
        else if(linetext.startsWith("CD")){
            lineparts = linetext.simplified().split(" ");

            dlist.clear();
            for(int i = 1;i<lineparts.size();i++){
                dlist.push_back(lineparts[i].toDouble());
            }
            g->setDispCoef(dlist);

            //_glasses.last().printProperty();
        }

    }

    file.close();

    for(int i = 0; i<_glasses.size();i++)
    {
        _glasses[i]->computeProperties();
    }

    return true;
}
