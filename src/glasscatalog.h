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

#ifndef GLASSCATALOGUE_H
#define GLASSCATALOGUE_H

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

#include "glass.h"
#include "pugixml.hpp" //https://pugixml.org

class GlassCatalog
{
public:
    GlassCatalog();
    ~GlassCatalog();

    /**
     * @brief glass supplyer name
     * @return
     */
    QString supplyer() const { return _supplyer;}

    /**
     * @brief get number of glasses
     * @return
     */
    int glassCount() const {return _glasses.size();}

    /**
     * @brief get Glass object at the index of the list
     * @param index number
     * @return
     */
    Glass* glass(int index) const {return _glasses.at(index);}

    /**
     * @brief get Glass object from the name
     * @param glassname name
     * @return
     */
    Glass* glass(QString glassname) const;


    /**
     * @brief Check if the specified glass is contained
     * @param glassname
     * @return
     */
    bool hasGlass(QString glassname);

    /**
     * @brief load AGF file
     * @param AGF file path
     * @return
     */
    bool loadAGF(QString AGFpath);

    /**
     * @brief load CodeV Xml
     * @param Xmlpath
     * @return
     * @note This function utilizes xml parser library by https://pugixml.org
     */
    bool loadXml(QString xmlpath);

private:
    QString  _supplyer;
    QList<Glass*> _glasses;
};

#endif // GLASSCATALOGUE_H
