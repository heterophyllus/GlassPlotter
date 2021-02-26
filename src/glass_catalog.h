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


/**
  * GlassCatalog Container Class
  * */

#pragma once

#ifndef GLASSCATALOGUE_H
#define GLASSCATALOGUE_H

#include <QString>
#include <QList>
#include <QMap>

class Glass;

class GlassCatalog
{
public:
    GlassCatalog();
    ~GlassCatalog();

    QString supplyer() const { return _supplyer;}

    Glass*  glass(int n) const;
    Glass*  glass(QString glassname) const;
    int     glassCount() const {return _glasses.size();}
    bool    hasGlass(QString glassname) const;

    bool loadAGF(QString AGFpath, QString& parse_result);
    bool loadXml(QString xmlpath, QString& parse_result);

    void clear();

private:
    QString       _supplyer;
    QList<Glass*> _glasses;

    QMap<QString, int> _name_to_int_map;
};

#endif // GLASSCATALOGUE_H
