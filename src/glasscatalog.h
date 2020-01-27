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


#ifndef GLASSCATALOGUE_H
#define GLASSCATALOGUE_H

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

#include "glass.h"

class GlassCatalog
{
public:
    GlassCatalog();
    ~GlassCatalog();

    //getter
    QString supplyer(){
        return _supplyer;
    }
    int glassCount(){
        return _glasses.size();
    }
    Glass* glass(int index){
        return _glasses.at(index);
    }

    //setter
    void setSupplyer(QString str){
        _supplyer = str;
    }

    //void addGlass(Glass* g);
    void clear();

    bool loadAGF(QString AGFpath);

private:
    QString  _supplyer;
    QList<Glass*> _glasses;
};

#endif // GLASSCATALOGUE_H
