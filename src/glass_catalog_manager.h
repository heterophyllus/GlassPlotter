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


#ifndef GLASSCATALOGMANAGER_H
#define GLASSCATALOGMANAGER_H

#include <QString>
#include <QList>
#include <QStringList>

#include "glass_catalog.h"

/** top level management class */
class GlassCatalogManager
{
public:
    GlassCatalogManager();
    ~GlassCatalogManager();

    static QList<GlassCatalog*>& catalogList();
    static bool isEmpty();
    static Glass* find(QString fullName);
    static void loadCatalogFiles(const QStringList& catalogFilePaths, QString& parseResult);

private:
    static QList<GlassCatalog*> m_catalogList;
};

#endif
