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
#include <QtMath>

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
    double          Pxy(QString x, QString y) const;
    double          Pxy_(QString x, QString y) const;

    /** convenience function to get glass property */
    double          getValue(QString dname) const;
    double          nd() const;
    double          ne() const;
    double          vd() const;
    double          ve() const;
    double          PgF() const;
    double          PCt_() const;

    double BuchdahlDispCoef(int n) const;

    void   setName(QString str){ _name = str;}
    void   setSupplyer(QString str){ _supplyer = str;}
    void   setMIL(QString str) {_MIL = str;}
    void   setStatus(QString str);
    void   setStatus(int n);
    void   setComment(QString str){ _comment = str; }


    // extra data
    double lowTCE() const { return _lowTCE; }
    double highTCE() const { return _highTCE; }

    void   setLowTCE(double val) { _lowTCE = val; }
    void   setHighTCE(double val) { _highTCE = val; }


    // dispersion data
    int         formulaIndex() const {return _formulaIndex;};
    QString     formulaName() const;
    int         dispersionCoefCount() const { return _dispersionData.size(); }
    double      dispersionCoef(int n) const;

    void        setDispForm(int n){ _formulaIndex = n;}
    void        setDispCoef(int n, double val);


    // thermal data
    double          D0()  const { return _thermalData[0]; }
    double          D1()  const { return _thermalData[1]; }
    double          D2()  const { return _thermalData[2]; }
    double          E0()  const { return _thermalData[3]; }
    double          E1()  const { return _thermalData[4]; }
    double          Ltk() const { return _thermalData[5]; }
    double          T0()  const { return _thermalData[6]; }
    QVector<double> getThermalData() const { return _thermalData; }
    double          dn_dt_abs(double T, double lambdamicron) const;
    QVector<double> dn_dt_abs(QVector<double> vT, double lambdamicron) const;

    void            setThermalData(int n, double val);


    // other data
    double relCost() const { return _relCost; }
    double climateResist() const {return _climateResist;}
    double stainResist() const {return _stainResist;}
    double acidResist() const {return _acidResist;}
    double alkaliResist() const {return _alkaliResist;}
    double phosphateResist() const {return _phosphateResist;}

    void  setRelCost(double val) {_relCost = val;}
    void  setClimateResist(double val){_climateResist = val;}
    void  setStainResist(double val){_stainResist = val;}
    void  setAcidResist(double val){_acidResist = val;}
    void  setAlkaliResist(double val){_alkaliResist = val;}
    void  setPhosphateResist(double val){_phosphateResist = val;}

    // transmittance data
    double          transmittance(double lambdamicron, double thi = 25) const;
    QVector<double> transmittance(QVector<double> vLambdamicron, double thi = 25) const;
    double          lambdaMin() const {return _lambdaMin;}
    double          lambdaMax() const {return _lambdaMax;}
    void            getTransmittanceData(QList<double>& pvLambdamicron, QList<double>& pvTransmittance, QList<double>& pvThickness);

    void            appendTransmittanceData(double lambdamicron, double trans, double thick);
    void            setLambdaMin(double val){ _lambdaMin = val;}
    void            setLambdaMax(double val){ _lambdaMax = val;}


private:
    QString _name;
    QString _supplyer;
    QString _status;
    QString _MIL;
    QString _comment;

    // extra data
    double _lowTCE; // TCE: thermal coefficient of expansion
    double _highTCE;

    // dispersion data
    const int       _dispersion_data_size = 12;
    QVector<double> _dispersionData;
    int             _formulaIndex;

    // thermal data
    const int       _thermal_data_size = 7;
    QVector<double> _thermalData; //<D0> <D1> <D2> <E0> <E1> <Ltk> <temp>

    // other data
    // Many glasses does not have valid data in OD line, so their default value should be NaN for "no data".
    double _relCost;
    double _climateResist;
    double _stainResist;
    double _acidResist;
    double _alkaliResist;
    double _phosphateResist;

    // transmittance data
    double        _lambdaMax;
    double        _lambdaMin;
    QList<double> _wavelengthData; //micron
    QList<double> _transmittanceData;
    QList<double> _thicknessData;
};

#endif // GLASS_H
