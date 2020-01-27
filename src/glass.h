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


#ifndef GLASS_H
#define GLASS_H

#include <QString>
#include <QList>

#include "math.h"


class SpectralLine
{
public:
    SpectralLine();
    static const double C;
    static const double C_;
    static const double d;
    static const double e;
    static const double F;
    static const double F_;
    static const double g;

};

class Glass
{
public:
    Glass();
    ~Glass();

    double index(double lambdamicron);

    QString name()
    {
        return _Name;
    }
    double nd() const
    {
        return _nd;
    }
    double vd() const
    {
        return _vd;
    }
    double ne() const
    {
        return _ne;
    }
    double ve() const
    {
        return _ve;
    }
    double PgF() const
    {
        return _PgF;
    }

    void computeProperties();

    void setName(QString str);
    void setNd(double value);
    void setVd(double value);
    void setPgF(double value);
    void setExcludeSub(int value);
    void setDispForm(int value);
    //void setNIL(int value);
    //void setStatus(int value);
    //void setAvailability(int value);
    void setTCE(double value);
    //void setDensity(double value);
    void setDeltaPgF(double value);
    //void setIgnoreThermalExp(int value);
    //void setIgnoreThermalExp(bool value);
    void setDispCoef(QList<double> coefs);
    //void setthermalData(QList<double> list);

    // some function should be implemented here

    // dispersion formulas
    /*
    double Schott(double lambdamicron);   
    double Sellmeier1(double lambdamicron);  
    double Sellmeier2(double lambdamicron);   
    double Sellmeier3(double lambdamicron);
    double Sellmeier4(double lambdamicron);
    double Sellmeier5(double lambdamicron);
    double Herzberger(double lambdamicron);
    double HandbookOfOptics1(double lambdamicron);
    double HandbookOfOptics2(double lambdamicron);
    double Extended1(double lambdamicron);
    double Extended2(double lambdamicron);
    */

    // for debug
    void printProperty();

private:
    QString _Name;

    double _nC;
    double _nC_;
    double _nd;
    double _ne;
    double _nF;
    double _nF_;
    double _ng;

    double _vd;
    double _ve;
    double _PgF;

    // NM
    int    _dispform;
    int    _NIL;
    int    _exclude_sub;
    int    _status;
    int    _availability;

    // ED
    double _tce;
    double _density;
    double _dPgF;
    bool   _ignore_thermal_expand;

    // CD
    QList<double> _dispcoefs;

    // TD
    QList<double> _thermaldata;

    // OD
    double _relcost;
    double _cr;
    double _fr;
    double _sr;
    double _ar;
    double _pr;

    // LD
    double _lambdaMax;
    double _lambdaMin;

    // IT
    // shoud be implemented

};

#endif // GLASS_H
