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


#include "glass.h"

Glass::Glass()
{
    _name     = "";
    _supplyer = "";
    _status   = "-";
    _comment  = "";
    _MIL      = "";

    _formulaIndex = 1;

    _lambdaMax = 0;
    _lambdaMin = 0;

    _dispersionData.clear();
    _wavelengthData.clear();
    _transmittanceData.clear();
    _thicknessData.clear();
    _thermalData.clear();
}

Glass::~Glass()
{
    _dispersionData.clear();
    _wavelengthData.clear();
    _transmittanceData.clear();
    _thicknessData.clear();
    _thermalData.clear();
}

double Glass::getValue(QString dname)
{
    if(dname == "nd"){
        //return nd();
        return index("d");
    }
    else if(dname == "ne"){
        //return ne();
        return index("e");
    }
    else if(dname == "vd"){
        //return vd();
        return (index("d") - 1)/(index("F") - index("C"));
    }
    else if(dname == "ve"){
        //return ve();
        return (index("e") - 1)/(index("F_") - index("C_"));
    }
    else if(dname == "PgF"){
        //return PgF();
        return (index("g") - index("F")) / ( index("F") - index("C") );
    }
    else if(dname == "PCt"){
        //return Pxy_("C","t");
        return (index("C") - index("t")) / ( index("F_") - index("C_") );
    }
    else{
        return 0;
    }
}

double Glass::index(double lambdamicron)
{
    QList<double> c = _dispersionData;

    switch(_formulaIndex){

    // ---> Zemax AGF
    case 1:
        return DispersionFormula::Schott(lambdamicron,c);
    case 2:
        return DispersionFormula::Sellmeier1(lambdamicron,c);
    case 3:
        return DispersionFormula::Herzberger(lambdamicron,c);
    case 4:
        return DispersionFormula::Sellmeier2(lambdamicron,c);
    case 5:
        return DispersionFormula::Conrady(lambdamicron,c);
    case 6:
        return DispersionFormula::Sellmeier3(lambdamicron,c);
    case 7:
        return DispersionFormula::HandbookOfOptics1(lambdamicron,c);
    case 8:
        return DispersionFormula::HandbookOfOptics2(lambdamicron,c);
    case 9:
        return DispersionFormula::Sellmeier4(lambdamicron,c);
    case 10:
        return DispersionFormula::Extended1(lambdamicron,c);
    case 11:
        return DispersionFormula::Sellmeier5(lambdamicron,c);
    case 12:
        return DispersionFormula::Extended2(lambdamicron,c);
    case 13: // Unknown
        return 0;

    // ---> CODEV XML
    case 101:
        return DispersionFormula::Laurent(lambdamicron,c);
    case 102:
        return DispersionFormula::GlassManufacturerLaurent(lambdamicron,c);
    case 103:
        return DispersionFormula::GlassManufacturerSellmeier(lambdamicron,c);
    case 104:
        return DispersionFormula::StandardSellmeier(lambdamicron,c);
    case 105:
        return DispersionFormula::Cauchy(lambdamicron,c);
    case 106:
        return DispersionFormula::Hartman(lambdamicron,c);
    default:
        return 0;
    }
    c.clear();
}

double Glass::index(QString spectral)
{
    return index(SpectralLine::wavelength(spectral)/1000); // unit:micron
}

QVector<double> Glass::index(QVector<double> wvlvec)
{
    QVector<double> y;
    y = wvlvec; // allocation

    for(int i = 0; i < wvlvec.size(); i++){
        y[i] = (index(wvlvec[i]));
    }
    return y;
}

void Glass::setStatus(QString str)
{
    _status = str;
}

void Glass::setStatus(int index)
{
    switch(index)
    {
    case 1:
        _status = "Preferred";
        break;
    case 2:
        _status = "Obsolete";
        break;
    case 3:
        _status = "Special";
        break;
    case 4:
        _status = "Melt";
        break;
    default:
        _status = "No Data";
    }
}


void Glass::setDispCoef(int index, double val)
{
    if(_dispersionData.isEmpty()){
        _dispersionData.append(val);
    }
    else if(_dispersionData.size() <= index){
        _dispersionData.append(val);
    }
    else{
        _dispersionData[index] = val;
    }
}


QString Glass::formulaName()
{
    switch(_formulaIndex){
    case 1:
        return "Schott";
    case 2:
        return "Sellmeier1";
    case 3:
        return "Herzberger";
    case 4:
        return "Sellmeier2";
    case 5:
        return "Conrady";
    case 6:
        return "Sellmeier3";
    case 7:
        return "Handbook of Optics1";
    case 8:
        return "Handbook of Optics2";
    case 9:
        return "Sellmeier4";
    case 10:
        return "Extended1";
    case 11:
        return "Sellmeier5";
    case 12:
        return "Extended2";
    case 13:
        return "Unknown";
    case 101:
        return "Laurent";
    case 102:
        return "Glass Manufacturer Laurent";
    case 103:
        return "Glass Manufacturer Sellmeier";
    case 104:
        return "Standard Sellmeier";
    case 105:
        return "Cauchy";
    case 106:
        return "Hartman";
    default:
        return "Unknown";
    }
}

double Glass::transmittance(double lambdamicron, double thickness)
{
    double refThickness = _thicknessData[0];
    QVector<double> sx, sy;

    for(int i = 0; i < _transmittanceData.size(); i++)
    {
        sx.append(_wavelengthData[i]);
        sy.append(pow(_transmittanceData[i], thickness/refThickness)); // T^(t/ref_t)
    }

    tk::spline s;
    s.set_points(sx.toStdVector(), sy.toStdVector());
    return s(lambdamicron);
}

QVector<double> Glass::transmittance(QVector<double> x, double thickness)
{
    double refThickness = _transmittanceData[0];
    QVector<double> sx, sy;

    for(int i = 0; i < _transmittanceData.size(); i++)
    {
        sx.append(_wavelengthData[i]);
        sy.append(pow(_transmittanceData[i], thickness/refThickness));
    }

    tk::spline s;
    s.set_points(sx.toStdVector(), sy.toStdVector());

    QVector<double> y;
    for(int i = 0; i < x.count(); i++)
    {
        y.append(s(x[i]));
    }
    return y;
}

void Glass::appendTransmittanceData(double lambdamicron, double trans, double thick)
{
    _wavelengthData.append(lambdamicron);
    _transmittanceData.append(trans);
    _thicknessData.append(thick);
}


double Glass::dn_dt_abs(double T, double lambdamicron)
{
    double dT = (T - T0());
    double n = index(lambdamicron);

    return (n*n-1)/(2*n) * ( D0() + 2*D1()*dT + 3*D2()*dT*dT + (E0() + 2*E1()*dT)/(lambdamicron*lambdamicron - Ltk()*Ltk()) );
}

QVector<double> Glass::dn_dt_abs(QVector<double> T, double lambdamicron)
{
    QVector<double> dndt = T;
    for(int i = 0; i < T.size(); i++){
        dndt[i] = dn_dt_abs(T[i],lambdamicron);
    }
    return dndt;
}


void Glass::setThermalData(int index, double val)
{
    if(_thermalData.isEmpty()){
        _thermalData.append(val);
    }
    else if(_thermalData.size() <= index){
        _thermalData.append(val);
    }
    else{
        _thermalData[index] = val;
    }
}



