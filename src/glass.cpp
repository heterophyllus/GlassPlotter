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

SpectralLine::SpectralLine()
{

}

const double SpectralLine::C = 656.2725;
const double SpectralLine::C_= 643.847;
const double SpectralLine::d = 587.5618;
const double SpectralLine::e = 546.074;
const double SpectralLine::F = 486.1327;
const double SpectralLine::F_= 479.9914;
const double SpectralLine::g = 435.8343;

Glass::Glass()
{
    _Name = "";

    _nC  = 0;
    _nC_ = 0;
    _nd  = 0;
    _ne  = 0;
    _nF  = 0;
    _nF_ = 0;
    _ng  = 0;

    _vd  = 0;
    _ve  = 0;
    _PgF = 0;

    // NM
    _dispform = 1;
    _NIL      = 0;
    _exclude_sub = 0;
    _status = 0;
    _availability = 0;

    // ED
    _tce = 0;
    _density = 0;
    _dPgF = 0;
    _ignore_thermal_expand = false;

    // CD
    _dispcoefs.clear();

    // TD
    _thermaldata.clear();

    // OD
    _relcost = 0;
    _cr = 0;
    _fr = 0;
    _sr = 0;
    _ar = 0;
    _pr = 0;

    // LD
    _lambdaMax = 800;
    _lambdaMin = 200;

    // IT
    // shoud be implemented

}

Glass::~Glass()
{
    _dispcoefs.clear();
    _thermaldata.clear();
}

double Glass::index(double lambdamicron)
{
    switch(_dispform){
    case 1: //Schott
        return sqrt( _dispcoefs[0] + _dispcoefs[1]*pow(lambdamicron,2) + _dispcoefs[2]*pow(lambdamicron,-2) + _dispcoefs[3]*pow(lambdamicron,-4) + _dispcoefs[4]*pow(lambdamicron,-6) + _dispcoefs[5]*pow(lambdamicron,-8) );
    case 2: //Sellmeier 1
        return sqrt( 1 + _dispcoefs[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[1]) + _dispcoefs[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[3]) + _dispcoefs[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[5]) );
    case 3: //Herzberger
        double L;
        L = 1/(pow(lambdamicron,2)-0.028);
        return _dispcoefs[0] + _dispcoefs[1]*L + _dispcoefs[2]*pow(L,2) + _dispcoefs[3]*pow(lambdamicron,2) + _dispcoefs[4]*pow(lambdamicron,4) + _dispcoefs[5]*pow(lambdamicron,6);
    case 4: //Sellmeier 2
        return sqrt( 1 + _dispcoefs[0] + _dispcoefs[1]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(_dispcoefs[2],2)) + _dispcoefs[3]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(_dispcoefs[4],2)) );
    case 5: //Conrady
        return ( _dispcoefs[0] + _dispcoefs[1]/lambdamicron + _dispcoefs[2]/pow(lambdamicron,3.5) );
    case 6: //Sellmeier 3
        return sqrt( 1 + _dispcoefs[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[1]) + _dispcoefs[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[3]) + _dispcoefs[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[5]) + _dispcoefs[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[7]) );
    case 7: //Handbook Of Optics 1
        return sqrt( _dispcoefs[0] + _dispcoefs[1]/(pow(lambdamicron,2)-_dispcoefs[2]) - _dispcoefs[3]*pow(lambdamicron,2) );
    case 8: //Handbook Of Optics 2
        return sqrt( _dispcoefs[0] + _dispcoefs[1]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[2]) - _dispcoefs[3]*pow(lambdamicron,2) );
    case 9: //Sellmeier 4
        return sqrt( 1 + _dispcoefs[0] + _dispcoefs[1]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[2]) + _dispcoefs[3]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[4]) );
    case 10: //Extended 1
        return sqrt( _dispcoefs[0] + _dispcoefs[1]*pow(lambdamicron,2) + _dispcoefs[2]*pow(lambdamicron,-2) + _dispcoefs[3]*pow(lambdamicron,-4) + _dispcoefs[4]*pow(lambdamicron,-6) + _dispcoefs[5]*pow(lambdamicron,-8) + _dispcoefs[6]*pow(lambdamicron,-10) + _dispcoefs[7]*pow(lambdamicron,-12) );
    case 11: //Sellmeier 5
        return sqrt( 1 + _dispcoefs[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[1]) + _dispcoefs[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[3]) + _dispcoefs[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[5]) + _dispcoefs[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[7]) + _dispcoefs[8]*pow(lambdamicron,2)/(pow(lambdamicron,2)-_dispcoefs[9]) );
    case 12: //Extended 2
        return sqrt( _dispcoefs[0] + _dispcoefs[1]*pow(lambdamicron,2) + _dispcoefs[2]*pow(lambdamicron,-2) + _dispcoefs[3]*pow(lambdamicron,-4) + _dispcoefs[4]*pow(lambdamicron,-6) + _dispcoefs[5]*pow(lambdamicron,-8) + _dispcoefs[6]*pow(lambdamicron,4) + _dispcoefs[7]*pow(lambdamicron,6) );
    default:
        return 0;
    }
}

void Glass::setName(QString str)
{
    _Name = str;
}
void Glass::setNd(double value)
{
    _nd = value;
}
void Glass::setVd(double value)
{
    _vd = value;
}
void Glass::setPgF(double value)
{
    _PgF = value;
}

void Glass::computeProperties()
{
    _nC  = index( (SpectralLine::C)  *pow(10,-3));
    _nC_ = index( (SpectralLine::C_) *pow(10,-3));
    _ne  = index( (SpectralLine::e)  *pow(10,-3));
    _nF  = index( (SpectralLine::F)  *pow(10,-3));
    _nF_ = index( (SpectralLine::F_) *pow(10,-3));
    _ng  = index( (SpectralLine::g)  *pow(10,-3));

    _ve  = (_ne - 1)/(_nF_-_nC_);
    _PgF = (_ng-_nF)/(_nF-_nC);
}

void Glass::setDispForm(int value)
{
    _dispform = value;
}

void Glass::setTCE(double value)
{
    _tce = value;
}

void Glass::setDeltaPgF(double value)
{
    _dPgF = value;
}

void Glass::setDispCoef(QList<double> coefs)
{
    _dispcoefs = coefs;
}

void Glass::printProperty()
{
    qDebug("%s : Nd= %f, Vd= %f, Pgf= %f", _Name.toUtf8().data(), _nd, _vd, _PgF);
    qDebug("C:%f, F:%f, g:%f", _nC, _nF, _ng);
    qDebug("dispform: %d", _dispform);
    qDebug("disp coefs");
    for(int i = 0; i < _dispcoefs.size(); i++){
        qDebug("   C[%d]=%f", i, _dispcoefs.at(i));
    }
}
