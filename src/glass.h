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

#pragma once

#ifndef GLASS_H
#define GLASS_H

#include <QString>
#include <QList>
#include <QVector>

class Glass
{
public:
    Glass();
    ~Glass();

    // fundamental data
    double          index(double lambdamicron) const;
    double          index(QString spectral) const;
    QVector<double> index(QVector<double> vLambdamicron) const;
    QString         name() const { return _name;}
    QString         supplyer() const { return _supplyer;}
    QString         status() const { return _status;}
    QString         MIL() const {return _MIL;}
    QString         comment() const { return _comment; }

    double getValue(QString dname) const;
    double nd() const {return index("d");}
    double ne() const {return index("e");}
    double vd() const {return (index("d") - 1)/(index("F") - index("C"));}
    double ve() const {return (index("e") - 1)/(index("F_") - index("C_"));}
    double Pxy(QString x, QString y) const {return (index(x) - index(y)) / ( index("F") - index("C") );}
    double Pxy_(QString x, QString y) const {return (index(x) - index(y)) / ( index("F_") - index("C_") );}
    double PgF() const {return Pxy("g","F");}

    void setName(QString str){ _name = str;}
    void setSupplyer(QString str){ _supplyer = str;}
    void setMIL(QString str) {_MIL = str;}
    void setStatus(QString str);
    void setStatus(int n);
    void setComment(QString str){ _comment = str; }


    // dispersion data
    int     formulaIndex() const {return _formulaIndex;};
    QString formulaName() const;
    int     dispersionCoefCount() const { return _dispersionData.size(); }
    double  dispersionCoef(int n) const { return _dispersionData[n]; }

    void    setDispForm(int n){ _formulaIndex = n;}
    void    setDispCoef(int n, double val);


    // transmittance data
    double          transmittance(double lambdamicron, double thi = 25) const;
    QVector<double> transmittance(QVector<double> vLambdamicron, double thi = 25) const;
    double          lambdaMin() const {return _lambdaMin;}
    double          lambdaMax() const {return _lambdaMax;}

    void            appendTransmittanceData(double lambdamicron, double trans, double thick);
    void            setLambdaMin(double val){ _lambdaMin = val;}
    void            setLambdaMax(double val){ _lambdaMax = val;}


    // thermal data
    double          D0() const {  return _thermalData[0]; }
    double          D1() const {  return _thermalData[1]; }
    double          D2() const {  return _thermalData[2]; }
    double          E0() const {  return _thermalData[3]; }
    double          E1() const {  return _thermalData[4]; }
    double          Ltk() const{  return _thermalData[5]; }
    double          T0() const {  return _thermalData[6]; }
    double          dn_dt_abs(double T, double lambdamicron) const;
    QVector<double> dn_dt_abs(QVector<double> vT, double lambdamicron) const;
    void            setThermalData(int n, double val);
    bool            hasThermalData = false;

private:
    QString _name;
    QString _supplyer;
    QString _status;
    QString _MIL;
    QString _comment;

    // dispersion data
    const int       _dispersion_data_size = 12;
    QVector<double> _dispersionData;
    int             _formulaIndex;

    // transmittance data
    double        _lambdaMax;
    double        _lambdaMin;
    QList<double> _wavelengthData; //micron
    QList<double> _transmittanceData;
    QList<double> _thicknessData;

    // thermal data
    const int       _thermal_data_size = 7;
    QVector<double> _thermalData; //<D0> <D1> <D2> <E0> <E1> <Ltk> <temp>

};

#endif // GLASS_H
