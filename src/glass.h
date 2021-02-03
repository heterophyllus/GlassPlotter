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
  * Glass property container class
  */



#ifndef GLASS_H
#define GLASS_H

#include <QString>
#include <QList>

#include "spline.h" // c++ cubic spline library, Tino Kluge (ttk448 at gmail.com), https://github.com/ttk592/spline
#include "spectralline.h"
#include "dispersionformula.h"

#ifndef SIZE_OF_DISPERSIONDATA
#define SIZE_OF_DISPERSIONDATA 12
#endif

#ifndef SIZE_OF_THERMALDATA
#define SIZE_OF_THERMALDATA 7
#endif

class Glass
{
public:
    Glass();
    ~Glass();

    double          index(double lambdamicron);
    double          index(QString spectral);
    QVector<double> index(QVector<double> wvlvec);
    QString         name() const { return _name;}
    QString         supplyer() const { return _supplyer;}
    QString         status() const { return _status;}
    QString         MIL() const {return _MIL;}
    QString         comment() const { return _comment; }

    // fundamental data
    double getValue(QString dname);
    double nd(){return index("d");}
    double ne(){return index("e");}
    double vd(){return (index("d") - 1)/(index("F") - index("C"));}
    double ve(){return (index("e") - 1)/(index("F_") - index("C_"));}
    double PgF(){return Pxy("g","F");}
    double Pxy(QString x, QString y){return (index(x) - index(y)) / ( index("F") - index("C") );}
    double Pxy_(QString x, QString y){return (index(x) - index(y)) / ( index("F_") - index("C_") );}

    void setName(QString str){ _name = str;}
    void setSupplyer(QString str){ _supplyer = str;}
    void setMIL(QString str) {_MIL = str;}
    void setStatus(QString str);
    void setStatus(int index);
    void setComment(QString str){ _comment = str; }

    // dispersion data
    int     formulaIndex() const {return _formulaIndex;};
    QString formulaName();
    int     dispersionCoefCount(){ return _dispersionData.size(); }
    double  dispersionCoef(int n) const { return _dispersionData[n]; }
    void    setDispForm(int n){ _formulaIndex = n;}
    void    setDispCoef(int index, double val);

    // transmittance data
    double          transmittance(double lambdamicron, double thickness = 25);
    QVector<double> transmittance(QVector<double> x, double thickness = 25);
    void            appendTransmittanceData(double lambdamicron, double trans, double thick);
    void            setLambdaMin(double value){ _lambdaMin = value;}
    void            setLambdaMax(double value){ _lambdaMax = value;}
    double          lambdaMin() const {return _lambdaMin;}
    double          lambdaMax() const {return _lambdaMax;}

    // thermal data
    double          D0(){  return _thermalData[0]; }
    double          D1(){  return _thermalData[1]; }
    double          D2(){  return _thermalData[2]; }
    double          E0(){  return _thermalData[3]; }
    double          E1(){  return _thermalData[4]; }
    double          Ltk(){ return _thermalData[5]; }
    double          T0(){  return _thermalData[6]; }
    double          dn_dt_abs(double T, double lambdamicron);
    QVector<double> dn_dt_abs(QVector<double> T, double lambdamicron);
    void            setThermalData(int index, double val);
    bool            hasThermalData = false;

private:
    QString _name;
    QString _supplyer;
    QString _status;
    QString _MIL;
    QString _comment;

    // dispersion data
    QList<double> _dispersionData;
    int           _formulaIndex;

    // transmittance data
    double        _lambdaMax;
    double        _lambdaMin;
    QList<double> _wavelengthData;
    QList<double> _transmittanceData;
    QList<double> _thicknessData;

    // thermal data
    QList<double> _thermalData; //<D0> <D1> <D2> <E0> <E1> <Ltk> <temp>
};


#endif // GLASS_H
