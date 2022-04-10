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

#include <QFileInfo>

#include "glass_catalog_manager.h"

QList<GlassCatalog*> GlassCatalogManager::m_catalogList;

GlassCatalogManager::GlassCatalogManager()
{

}

GlassCatalogManager::~GlassCatalogManager()
{
    if(!m_catalogList.isEmpty())
    {
        for (auto &cat: m_catalogList) {
            delete cat;
        }
        m_catalogList.clear();
    }
}


QList<GlassCatalog*>& GlassCatalogManager::catalogList()
{
    return m_catalogList;
}

bool GlassCatalogManager::isEmpty()
{
    return m_catalogList.isEmpty();
}

Glass* GlassCatalogManager::find(QString fullName)
{
    QStringList splitedText  = fullName.split("_");
    QString     productName  = splitedText[0];
    QString     supplierName = splitedText[1];

    int catalogCount = m_catalogList.size();
    for(int i = 0; i < catalogCount; i++) {
        if(m_catalogList[i]->supplier() == supplierName){
            if(m_catalogList[i]->hasGlass(productName)){
                return m_catalogList[i]->glass(productName);
            }
        }
    }

    return nullptr;
}

void GlassCatalogManager::loadCatalogFiles(const QStringList &catalogFilePaths, QString& parseResult)
{
    if(catalogFilePaths.empty()) {
        return;
    }

    // clear old catalogs
    if(!m_catalogList.isEmpty())
    {
        for (auto &cat: m_catalogList) {
            delete cat;
        }
        m_catalogList.clear();
    }

    // load catalogs
    GlassCatalog* catalog;
    QString parse_result, parse_result_all;

    QFileInfo finfo;
    finfo.setFile(catalogFilePaths.first());
    QString ext = finfo.suffix().toLower(); // .agf, .xml

    for(int i = 0; i < catalogFilePaths.size(); i++){
        catalog = new GlassCatalog;
        parse_result.clear();

        bool ok;
        if(ext == "agf"){
            ok = catalog->loadAGF(catalogFilePaths[i], parse_result);
        }else{
            ok = catalog->loadXml(catalogFilePaths[i], parse_result);
        }

        if(ok){
            m_catalogList.append(catalog);
            parse_result_all += parse_result;
        }
        else{
            parse_result_all += ("Catalog loading error:" + catalogFilePaths[i] + "\n");
            try {
                delete catalog;
            }  catch (...) {
                return;
            }
            continue;
        }
    }

    catalog = nullptr;

    parseResult = parse_result_all;

}
