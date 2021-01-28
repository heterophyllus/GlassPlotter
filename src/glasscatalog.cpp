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
    if(_glasses.size() > 0){
        for(int i = 0; i < _glasses.size(); i++){
            try {
                delete _glasses[i];
            } catch (...) {
                if(_glasses[i] != nullptr)
                    delete _glasses[i];
            }
        }
        _glasses.clear();
    }
    _supplyer = "";
}

Glass* GlassCatalog::glass(QString glassname) const
{
    for(int i = 0; i < _glasses.count(); i++)
    {
        if(glassname == _glasses[i]->name()){
            return _glasses[i];
        }
    }
    return nullptr;
}

bool GlassCatalog::hasGlass(QString glassname)
{
    if(_glasses.count() > 0)
    {
        for(int i = 0; i < _glasses.count(); i++)
        {
            if(glassname == _glasses[i]->name()){
                return true;
            }
        }
    }
    return false;
}

bool GlassCatalog::loadAGF(QString AGFpath)
{
    QFile file(AGFpath);
    if (! file.open(QIODevice::ReadOnly)) {
        qDebug() << "AGF File Open Error : " << AGFpath;
        return false;
    }

    int linecount = 0;
    QTextStream in(&file);
    QString linetext;
    QStringList lineparts;

    Glass *g;

    _glasses.clear();
    _supplyer = QFileInfo(AGFpath).baseName();

    while (! in.atEnd()) {
        linetext = in.readLine();
        linecount++;

        if(linetext.startsWith("NM")){
            lineparts = linetext.simplified().split(" ");
            g = new Glass;
            _glasses.append(g);
            _glasses.last()->setName(lineparts[1]);
            _glasses.last()->setSupplyer(_supplyer);
            _glasses.last()->setDispForm(lineparts[2].toInt());
            _glasses.last()->setMIL(lineparts[3]);

            if(lineparts.size() > 7){
                _glasses.last()->setStatus(lineparts[7].toUInt());
            }
        }
        else if (linetext.startsWith("GC")){
            _glasses.last()->setComment(linetext.remove(0,2).simplified());
        }
        else if(linetext.startsWith("CD")){ // CD <dispersion coefficients 1 - 10>
            lineparts = linetext.simplified().split(" ");
            for(int i = 1;i<lineparts.size();i++){
                _glasses.last()->setDispCoef(i-1,lineparts[i].toDouble());
            }
        }
        else if(linetext.startsWith("TD")){ // TD <D0> <D1> <D2> <E0> <E1> <Ltk> <Temp>
            lineparts = linetext.simplified().split(" ");
            if(lineparts.size() > 7){
                _glasses.last()->hasThermalData = true;
                for(int i = 1;i<8;i++){
                    _glasses.last()->setThermalData(i-1, lineparts[i].toDouble());
                }
            }else{
                _glasses.last()->hasThermalData = false;
            }
        }
        else if(linetext.startsWith("LD")){
            lineparts = linetext.simplified().split(" ");
            _glasses.last()->setLambdaMin(lineparts[1].toDouble()*1000); // micron
            _glasses.last()->setLambdaMax(lineparts[2].toDouble()*1000);
        }
        else if(linetext.startsWith("IT")){
            lineparts = linetext.simplified().split(" ");
            if(lineparts.size() < 4){
                continue; //eg. NIHON_KESSHO_KOGAKU CaF2
            }
            else{
                _glasses.last()->appendTransmittanceData(lineparts[1].toDouble(), lineparts[2].toDouble(), lineparts[3].toDouble());
            }
        }
    }

    file.close();

    return true;
}

bool GlassCatalog::loadXml(QString xmlpath)
{
    pugi::xml_document doc;
    if(!doc.load_file(xmlpath.toUtf8().data())) {
        qDebug() << "XML file open error : " << xmlpath;
        return false;
    }

    _supplyer = doc.first_child().first_child().child_value();

    pugi::xml_node node_glasses = doc.child("Catalog").child("Glasses");

    int k;
    Glass *g;
    _glasses.clear();
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
        }


        // dispersion coefs
        k = 0;
        for(pugi::xml_node_iterator dc_it = glass_it->child("DispersionCoefficients").begin(); dc_it != glass_it->child("DispersionCoefficients").end(); dc_it++)
        {
            g->setDispCoef(k,dc_it->text().as_double());
            k++;
        }

        // transmittance
        g->setLambdaMin(glass_it->child("LowWavelength").text().as_double());
        g->setLambdaMax(glass_it->child("HighWavelength").text().as_double());

        for(pugi::xml_node_iterator td_it = glass_it->child("TransmissionCurves").child("Curve").begin(); td_it != glass_it->child("TransmissionCurves").child("Curve").end(); td_it++)
        {
            double t=10;
            QString nodename = td_it->name();
            if(nodename.compare("Thickness")==0) {
                t = td_it->text().as_double();
            }
            else if(nodename.compare("Transmission")==0){
                double w = td_it->child("Wavelength").text().as_double();
                double v = td_it->child("Value").text().as_double();
                g->appendTransmittanceData(w/1000, v, t);
            }
        }

        // DnDt data
        g->hasThermalData = true;
        g->setThermalData( 0, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("DnDt_D0").text().as_double() );
        g->setThermalData( 1, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("DnDt_D1").text().as_double() );
        g->setThermalData( 2, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("DnDt_D2").text().as_double() );
        g->setThermalData( 3, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("DnDt_E0").text().as_double() );
        g->setThermalData( 4, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("DnDt_E1").text().as_double() );
        g->setThermalData( 5, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("Lambda").text().as_double() );
        g->setThermalData( 6, glass_it->child("DnDtData").child("DnDtForCategory").child("DnDtConstants").child("Temperature").text().as_double() );

        _glasses.append(g);
    }
    g = nullptr;

    return true;

}
