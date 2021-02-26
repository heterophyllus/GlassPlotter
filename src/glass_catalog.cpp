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

#include "glass_catalog.h"

#include "glass.h"
#include "pugixml.hpp" //https://pugixml.org

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

GlassCatalog::GlassCatalog()
{
    _glasses.clear();
    _name_to_int_map.clear();
    _supplyer = "";
}


GlassCatalog::~GlassCatalog()
{
    this->clear();
}

void GlassCatalog::clear()
{
    if(!_glasses.isEmpty()){
        for(auto &g:_glasses){
            try {
                delete g;
            } catch (...) {
                g = nullptr;
            }
            g = nullptr;
        }
        _glasses.clear();
    }
    _supplyer = "";
    _name_to_int_map.clear();
}

Glass* GlassCatalog::glass(int n) const
{
    if(n < _glasses.size()){
        return _glasses[n];
    }
    return nullptr;
}

Glass* GlassCatalog::glass(QString glassname) const
{
    if(_name_to_int_map.contains(glassname)){
        return _glasses[_name_to_int_map[glassname]];
    }
    return nullptr;
}

bool GlassCatalog::hasGlass(QString glassname) const
{
    return _name_to_int_map.contains(glassname);
}

/**
 * @brief Load glass data from Zemax AGF file
 * @param AGFpath AGF file path
 * @param parse_result Container for notable parse results
 * @return
 */
bool GlassCatalog::loadAGF(QString AGFpath, QString& parse_result)
{
    QFile file(AGFpath);
    if (! file.open(QIODevice::ReadOnly)) {
        return false;
    }

    // parse result
    QString filename = QFileInfo(AGFpath).fileName();

    this->clear();

    int linecount = 0;
    QTextStream in(&file);
    QString linetext;
    QStringList lineparts;

    _supplyer = QFileInfo(AGFpath).baseName();

    Glass *g;
    int glassNumber = 0;
    while (! in.atEnd())
    {
        linetext = in.readLine();
        linecount++;

        //NM <glass name> <dispersion formula #> <MIL#> <N(d)> <V(d)> <Exclude Sub> <status> <melt freq>
        if(linetext.startsWith("NM"))
        {
            lineparts = linetext.simplified().split(" ");
            g = new Glass;
            _glasses.append(g);
            _glasses.last()->setName(lineparts[1]);
            _glasses.last()->setSupplyer(_supplyer);
            _glasses.last()->setDispForm(lineparts[2].toInt());
            _glasses.last()->setMIL(lineparts[3]);

            _name_to_int_map.insert(_glasses.last()->name(),glassNumber);
            glassNumber += 1;

            if(lineparts.size() > 7){
                _glasses.last()->setStatus(lineparts[7].toUInt());
            }

            if(_glasses.last()->formulaIndex() == 13){
                parse_result += filename + "(" + QString::number(linecount) + "): " + _glasses.last()->name() + ": " + "Unknown dispersion formula\n";
            }
        }

         // GC <Individual Glass Comment>
        else if (linetext.startsWith("GC"))
        {
            _glasses.last()->setComment(linetext.remove(0,2).simplified());
        }

        // ED <TCE (-30 to 70)> <TCE (100 to 300)> <density> <dPgF> <Ignore Thermal Exp>
        else if(linetext.startsWith("ED"))
        {
            lineparts = linetext.simplified().split(" ");
            _glasses.last()->setLowTCE(lineparts[1].toDouble());
            _glasses.last()->setHighTCE(lineparts[2].toDouble());
        }

        // CD <dispersion coefficients 1 - 10>
        else if(linetext.startsWith("CD"))
        {
            lineparts = linetext.simplified().split(" ");
            for(int i = 1;i<lineparts.size();i++){
                _glasses.last()->setDispCoef(i-1,lineparts[i].toDouble());
            }
        }

        // TD <D0> <D1> <D2> <E0> <E1> <Ltk> <Temp>
        else if(linetext.startsWith("TD"))
        {
            lineparts = linetext.simplified().split(" ");
            if(lineparts.size() == 8){
                for(int i = 1;i<8;i++){
                    _glasses.last()->setThermalData(i-1, lineparts[i].toDouble());
                }
            }else{
                parse_result += filename + "(" + QString::number(linecount) + "): " + _glasses.last()->name() + ": " + "Invalid line(TD)\n";
            }
        }

        // OD <rel cost> <CR> <FR> <SR> <AR> <PR>
        else if(linetext.startsWith("OD"))
        {
            lineparts = linetext.simplified().split(" ");
            if(lineparts.size() == 7)
            {
                /*For these values, -1 should be specified if the data is not available.
                  Some manufactureres use "-" instead of "-1.00000".*/

                double dval;
                bool ok;

                dval = lineparts[1].toDouble(&ok);
                if(ok && (dval != -1.0)){
                    _glasses.last()->setRelCost(dval);
                }

                dval = lineparts[2].toDouble(&ok);
                if(ok && (dval != -1.0)){
                    _glasses.last()->setClimateResist(dval);
                }

                dval = lineparts[3].toDouble(&ok);
                if(ok && (dval != -1.0)){
                    _glasses.last()->setStainResist(dval);
                }

                dval = lineparts[4].toDouble(&ok);
                if(ok && (dval != -1.0)){
                    _glasses.last()->setAcidResist(dval);
                }

                dval = lineparts[5].toDouble(&ok);
                if(ok && (dval != -1.0)){
                    _glasses.last()->setAlkaliResist(dval);
                }

                dval = lineparts[6].toDouble(&ok);
                if(ok && (dval != -1.0)){
                    _glasses.last()->setPhosphateResist(dval);
                }
            }
            else{
                parse_result += filename + "(" + QString::number(linecount) + "): " + _glasses.last()->name() + ": " + "Invalid line(OD)\n";
            }

        }

        // LD <min lambda> <max lambda>
        else if(linetext.startsWith("LD"))
        {
            lineparts = linetext.simplified().split(" ");
            _glasses.last()->setLambdaMin(lineparts[1].toDouble()); // micron
            _glasses.last()->setLambdaMax(lineparts[2].toDouble());
        }

        // IT <lambda> <transmission> <thickness>
        else if(linetext.startsWith("IT"))
        {
            lineparts = linetext.simplified().split(" ");
            if(lineparts.size() == 4){
                _glasses.last()->appendTransmittanceData(lineparts[1].toDouble(), lineparts[2].toDouble(), lineparts[3].toDouble());
            }
            else{
                parse_result += filename + "(" + QString::number(linecount) + "): " + _glasses.last()->name() + ": " + "Invalid line(IT)\n";
            }
        }
    }

    file.close();

    return true;
}

/**
 * @brief Load glass data from CODEV Xml file
 * @param xmlpath Xml file path
 * @param parse_result Container for notable parse results
 * @return
 */
bool GlassCatalog::loadXml(QString xmlpath, QString& parse_result)
{
    pugi::xml_document doc;
    if(!doc.load_file(xmlpath.toUtf8().data())) {
        return false;
    }

    // parse result
    QString filename = QFileInfo(xmlpath).fileName();

    this->clear();

    _supplyer = doc.first_child().first_child().child_value();

    pugi::xml_node node_glasses = doc.child("Catalog").child("Glasses");

    int k;
    Glass *g;
    int glassNumber = 0;

    for (pugi::xml_node_iterator glass_it = node_glasses.begin(); glass_it != node_glasses.end(); glass_it++ )
    {
        g = new Glass;
        g->setSupplyer(_supplyer);
        g->setName(glass_it->child("GlassName").child_value());
        g->setMIL(glass_it->child("NumericName").child_value());

        // dispersion formula
        QString eqname = glass_it->child("EquationType").child_value();
        if(eqname.compare("Laurent")==0){
            g->setDispForm(101);
        }
        else if(eqname.compare("Glass Manufacturer Laurent")==0){
            g->setDispForm(102);
        }
        else if(eqname.compare("Glass Manufacturer Sellmeier")==0){
            g->setDispForm(103);
        }
        else if(eqname.compare("Standard Sellmeier")==0){
            g->setDispForm(104);
        }
        else if(eqname.compare("Cauchy")==0){
            g->setDispForm(105);
        }
        else if(eqname.compare("Hartman")==0){
            g->setDispForm(106);
        }
        else{
            g->setDispForm(13); //unknown
            parse_result += filename + ": " + g->name() + ": " + "Unknown dispersion formula\n";
        }


        // dispersion coefficients
        k = 0;
        for(pugi::xml_node_iterator dc_it = glass_it->child("DispersionCoefficients").begin(); dc_it != glass_it->child("DispersionCoefficients").end(); dc_it++)
        {
            g->setDispCoef(k,dc_it->text().as_double());
            k++;
        }

        // high/low TCE(CTE)
        if(glass_it->child("LowCTE")){
            g->setLowTCE(glass_it->child("LowCTE").child("Value").text().as_double());
        }
        else{
            parse_result += filename + ": " + g->name() + ": " + "Not found LowCTE\n";
        }
        if(glass_it->child("HighCTE")){
            g->setHighTCE(glass_it->child("HighCTE").child("Value").text().as_double());
        }
        else{
            parse_result += filename + ": " + g->name() + ": " + "Not found HighCTE\n";
        }

        // Manufacturer's properties
        bool hasAcidResist = false;
        bool hasClimateResist = false;
        bool hasStainResist = false;
        bool hasAlkaliResist = false;
        for(pugi::xml_node_iterator mp_it = glass_it->child("ManufacturersProperties").begin(); mp_it != glass_it->child("ManufacturersProperties").end(); mp_it++)
        {
            QString propertyname = QString(mp_it->child("Name").text().as_string());
            if(propertyname.compare("Acid_resist") == 0){
                hasAcidResist = true;
                g->setAcidResist(mp_it->child("Value").text().as_double());
            }
            else if(propertyname.compare("Climatic_resist") == 0){
                hasClimateResist = true;
                g->setClimateResist(mp_it->child("Value").text().as_double());
            }
            else if(propertyname.compare("Stain_resist") == 0){
                hasStainResist = true;
                g->setStainResist(mp_it->child("Value").text().as_double());
            }
            else if(propertyname.compare("Alkali_resist") == 0){
                hasAlkaliResist = true;
                g->setAlkaliResist(mp_it->child("Value").text().as_double());
            }
        }
        // append parse result of manufacturer property
        if(!hasAcidResist){
            parse_result += filename + ": " + g->name() + ": " + "Not found Acid Resist\n";
        }
        if(!hasClimateResist){
            parse_result += filename + ": " + g->name() + ": " + "Not found Climate Resist\n";
        }
        if(!hasStainResist){
            parse_result += filename + ": " + g->name() + ": " + "Not found Stain Resist\n";
        }
        if(!hasAlkaliResist){
            parse_result += filename + ": " + g->name() + ": " + "Not found Alkali Resist\n";
        }

        // transmittance
        g->setLambdaMin(glass_it->child("LowWavelength").text().as_double());
        g->setLambdaMax(glass_it->child("HighWavelength").text().as_double());

        for(pugi::xml_node_iterator td_it = glass_it->child("TransmissionCurves").child("Curve").begin(); td_it != glass_it->child("TransmissionCurves").child("Curve").end(); td_it++)
        {
            double t = 10;
            QString nodename = td_it->name();
            if(nodename.compare("Thickness")==0) {
                t = td_it->text().as_double();
            }

            if(nodename.compare("Transmission")==0){
                double w = td_it->child("Wavelength").text().as_double();
                double v = td_it->child("Value").text().as_double();
                g->appendTransmittanceData(w/1000.0, v, t);
            }
        }

        // DnDt data
        if(glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants"))
        {
            g->setThermalData( 0, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("DnDt_D0").text().as_double() );
            g->setThermalData( 1, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("DnDt_D1").text().as_double() );
            g->setThermalData( 2, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("DnDt_D2").text().as_double() );
            g->setThermalData( 3, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("DnDt_E0").text().as_double() );
            g->setThermalData( 4, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("DnDt_E1").text().as_double() );
            g->setThermalData( 5, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("Lambda").text().as_double() );
            g->setThermalData( 6, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("Temperature").text().as_double() );
        }
        else{
            parse_result += filename + ": " + g->name() + ": " + "Not found DnDtConstants\n";
        }

        // append to list
        _glasses.append(g);

        // add glass name to map
        _name_to_int_map.insert(g->name(), glassNumber);
        glassNumber += 1;
    }

    g = nullptr;

    return true;
}
