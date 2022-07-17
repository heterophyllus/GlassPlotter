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
#include <QList>
#include <QMap>

#include "glass.h"

/** GlassCatalog Container Class */
class GlassCatalog
{
public:
    GlassCatalog();
    ~GlassCatalog();

    QString supplier() const {return supplier_;}

    Glass*  glass(int n) const;
    Glass*  glass(const QString& glassname) const;
    int glassCount() const{return glasses_.size();}
    bool hasGlass(const QString& glassname) const;

    /**
     * @brief Load glass data from Zemax AGF file
     * @param AGFpath AGF file path
     * @param parse_result Container for notable parse results
     * @return
     */
    bool loadAGF(const QString& AGFpath, QString& parse_result);


    /**
     * @brief Load glass data from CODEV Xml file
     * @param xmlpath Xml file path
     * @param parse_result Container for notable parse results
     * @return
     */
    bool loadXml(QString xmlpath, QString& parse_result);

    void clear();

private:
    QString       supplier_;
    QList<Glass*> glasses_;

    QMap<QString, int> name_to_int_map_;
};


#endif // GLASSCATALOGUE_H
