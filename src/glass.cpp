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
    _name = "";

    _nd  = 0;
    _ne  = 0;
    _vd  = 0;
    _ve  = 0;
    _PgF = 0;
    _dPgF = 0;
    _status = "-";

    _comment = "";

    _lambdaMax = 0;
    _lambdaMin = 0;

    _dispersionData = new DispersionData;
    _transmittanceData = new TransmittanceData;
    _thermalData = new ThermalData;
}

Glass::~Glass()
{
    try {
        delete _dispersionData;
        _dispersionData = nullptr;
        delete _transmittanceData;
        _transmittanceData = nullptr;
        delete _thermalData;
        _thermalData = nullptr;
    } catch (...) {
        if(_dispersionData != nullptr)
            delete _dispersionData;
        if(_transmittanceData != nullptr)
            delete _transmittanceData;
        if(_thermalData != nullptr)
            delete _thermalData;
    }
}

double Glass::index(double lambdamicron)
{
    QList<double> c = _dispersionData->coefs;

    switch(_dispersionData->formulaIndex){

    // ---> Zemax AGF
    case 1:
        return Schott(lambdamicron,c);
    case 2:
        return Sellmeier1(lambdamicron,c);
    case 3:
        return Herzberger(lambdamicron,c);
    case 4:
        return Sellmeier2(lambdamicron,c);
    case 5:
        return Conrady(lambdamicron,c);
    case 6:
        return Sellmeier3(lambdamicron,c);
    case 7:
        return HandbookOfOptics1(lambdamicron,c);
    case 8:
        return HandbookOfOptics2(lambdamicron,c);
    case 9:
        return Sellmeier4(lambdamicron,c);
    case 10:
        return Extended1(lambdamicron,c);
    case 11:
        return Sellmeier5(lambdamicron,c);
    case 12:
        return Extended2(lambdamicron,c);
    case 13: // Unknown
        return 0;

    // ---> CODEV XML
    case 101:
        return Laurent(lambdamicron,c);
    case 102:
        return GlassManufacturerLaurent(lambdamicron,c);
    case 103:
        return GlassManufacturerSellmeier(lambdamicron,c);
    case 104:
        return StandardSellmeier(lambdamicron,c);
    case 105:
        return Cauchy(lambdamicron,c);
    case 106:
        return Hartman(lambdamicron,c);
    default:
        return 0;
    }

}

double Glass::index(QString spectral)
{
    double lambdamicron = SpectralLine::wavelength(spectral)/1000;

    return index(lambdamicron);
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

double Glass::transmittance(double lambdamicron, double thickness)
{
    double refThickness = _transmittanceData->thickness.first();
    QVector<double> sx, sy;

    for(int i = 0; i < _transmittanceData->size(); i++)
    {
        sx.append(_transmittanceData->wavelength.at(i));
        sy.append(pow(_transmittanceData->transmittance.at(i), thickness/refThickness)); // T^(t/ref_t)
    }

    tk::spline s;
    s.set_points(sx.toStdVector(), sy.toStdVector());
    return s(lambdamicron);
}

QVector<double> Glass::transmittance(QVector<double> x, double thickness)
{
    double refThickness = _transmittanceData->thickness.first();
    QVector<double> sx, sy;

    for(int i = 0; i < _transmittanceData->size(); i++)
    {
        sx.append(_transmittanceData->wavelength.at(i));
        sy.append(pow(_transmittanceData->transmittance.at(i), thickness/refThickness));
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


QString Glass::DispersionData::formulaName()
{
    switch(formulaIndex){
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

double Glass::dn_dT(double T, double lambdamicron)
{
    // should be implemented
    return 0;
}


double Glass::Pxy(QString x, QString y)
{
    return (index(x) - index(y)) / ( index("F") - index("C") );
}

double Glass::Pxy_(QString x, QString y)
{
    return (index(x) - index(y)) / ( index("F_") - index("C_") );
}

void Glass::computeProperties()
{
    _nd = index("d");
    _ne = index("e");
    _vd = (index("d") - 1)/(index("F") - index("C"));
    _ve = (index("e") - 1)/(index("F_") - index("C_"));
    _PgF = Pxy("g","F");
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
    if(index < MAX_COEF_COUNT){
        _dispersionData->coefs[index] = val;
    }
}

void Glass::setThermalCoef(int index, double val)
{
    _thermalData->coefs[index] = val;
}

void Glass::appendTransmittanceData(double lambdamicron, double trans, double thick)
{
    _transmittanceData->wavelength.append(lambdamicron);
    _transmittanceData->transmittance.append(trans);
    _transmittanceData->thickness.append(thick);
}


