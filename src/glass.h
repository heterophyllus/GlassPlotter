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

#include "math.h"
#include "spline.h" // c++ cubic spline library, Tino Kluge (ttk448 at gmail.com), https://github.com/ttk592/spline
#include "spectralline.h"


class Glass
{
public:
    Glass();
    ~Glass();

    /**
     * @brief Inner class to contain dispersion data
     */
    class DispersionData
    {
    public:

        DispersionData(){
            coefs.clear();
            for(int i = 0;i<12;i++) coefs.append(0.0); //initialize
        };
        ~DispersionData(){
            coefs.clear();
        };

        /**
         * @brief index value to express dispersion formula
         */
        int formulaIndex;

        /**
         * @brief coefficients of dispersion formula
         */
        QList<double> coefs;

        /**
         * @brief coefficients of dispersion formula
         * @param n coefficient index number
         * @return coefficient value
         */
        double coef(int n) const {
            return coefs[n];
        }

        QString formulaName();
    };


    void setDispForm(int n){ _dispersionData->formulaIndex = n;}
    void setDispCoef(int index, double val);

    QVector<double> index(QVector<double> wvlvec);

    DispersionData* dispersion(){return _dispersionData;}


    /**
     * @brief The inner class to contain Transmittance Data
     */
    class TransmittanceData
    {
    public:
        TransmittanceData(){};
        ~TransmittanceData(){
            wavelength.clear();
            transmittance.clear();
            thickness.clear();
        }

        QVector<double> wavelength;
        QVector<double> transmittance;
        QVector<double> thickness;

        int size(){
          return wavelength.size();
        }
    };


    double transmittance(double lambdamicron, double thickness = 25);
    QVector<double> transmittance(QVector<double> x, double thickness = 25);


    /**
     * @brief The inner class to contain Thermal Data
     */
    class ThermalData
    {
    public:
        ThermalData(){
            coefs.clear();
            for(int i = 0;i<6;i++) coefs.append(0.0); //initialize
        }

        /**
         * @brief coefficients of dn/dT
         */
        QVector<double> coefs; //<D0> <D1> <D2> <E0> <E1> <Ltk>

        double coef(int n){return coefs.at(n);}
        double D0(){ return coefs[0]; };
        double D1(){ return coefs[1]; };
        double D2(){ return coefs[2]; };
        double E0(){ return coefs[3]; };
        double E1(){ return coefs[4]; };
        double Ltk(){ return coefs[5]; };

        bool hasData = true;
    };
    void setThermalCoef(int index, double val);
    double dn_dT(double T, double lambdamicron);
    ThermalData* thermalData(){ return _thermalData; }

    /**
     * @brief get refractive index at the wavelength
     * @param lambdamicron wavelength in micron
     * @return refractive index
     */
    double index(double lambdamicron);

    /**
     * @brief get refractive index at the wavelength
     * @param spectral Fraunhofer line name
     * @return refractive index
     */
    double index(QString spectral);

    QString name() const { return _name;}
    QString supplyer() const { return _supplyer;}
    QString status() const { return _status;}
    QString MIL() const {return _MIL;}

    double nd() const { return _nd;}
    double ne() const { return _ne;}
    double vd() const { return _vd;}
    double ve() const { return _ve;}
    double PgF() const {return _PgF;}

    /**
     * @brief Pxy: partial dispersion between x and y
     * @param x Fraunhofer line
     * @param y Fraunhofer line
     * @return
     */
    double Pxy(QString x, QString y);
    double Pxy_(QString x, QString y);


    double lambdaMin() const {return _lambdaMin;}
    double lambdaMax() const {return _lambdaMax;}
    QString comment() const { return _comment; }

    double getValue(QString dataName);

    void computeProperties();

    void setName(QString str){ _name = str;}
    void setSupplyer(QString str){ _supplyer = str;}
    void setMIL(QString str) {_MIL = str;}
    void setStatus(QString str);
    void setStatus(int index);

    void setNd(double value){ _nd = value;}
    void setVd(double value){ _vd = value;}
    void setPgF(double value){_PgF = value;}
    void setdPgF(double value){_dPgF = value;}

    void setComment(QString str){ _comment = str; }

    void setLambdaMin(double value){ _lambdaMin = value;}
    void setLambdaMax(double value){ _lambdaMax = value;}


    /**
     * @brief append Transmittance Data expressed in IT line
     * @param lambdamicron wavelength in micron
     * @param trans transmittance
     * @param thick thickness
     */
    void appendTransmittanceData(double lambdamicron, double trans, double thick);


    // dispersion formulas

    static double Schott(double lambdamicron, QList<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) );
    }
    static double Sellmeier1(double lambdamicron, QList<double> c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[1]) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[3]) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[5]) );
    }
    static double Sellmeier2(double lambdamicron, QList<double> c){
        return sqrt( 1 + c[0] + c[1]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[2]) + c[3]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[4]) );
    }
    static double Sellmeier3(double lambdamicron, QList<double> c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[1]) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[3]) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[5]) + c[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[7]) );
    }
    static double Sellmeier4(double lambdamicron, QList<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[2]) + c[3]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[4]) );
    }
    static double Sellmeier5(double lambdamicron, QList<double> c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[1]) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[3]) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[5]) + c[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[7]) + c[8]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[9]) );
    }
    static double Herzberger(double lambdamicron, QList<double> c){
        double L = 1/(pow(lambdamicron,2)-0.028);
        return ( c[0] + c[1]*L + c[2]*pow(L,2) + c[3]*pow(lambdamicron,2) + c[4]*pow(lambdamicron,4) + c[5]*pow(lambdamicron,6) );
    }
    static double HandbookOfOptics1(double lambdamicron, QList<double> c){
        return sqrt( c[0] + c[1]/(pow(lambdamicron,2)-c[2]) - c[3]*pow(lambdamicron,2) );
    }
    static double HandbookOfOptics2(double lambdamicron, QList<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[2]) - c[3]*pow(lambdamicron,2) );
    }
    static double Extended1(double lambdamicron, QList<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,-10) + c[7]*pow(lambdamicron,-12) );
    }
    static double Extended2(double lambdamicron, QList<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,4) + c[7]*pow(lambdamicron,6) );
    }
    static double Conrady(double lambdamicron, QList<double> c){
        return ( c[0] + c[1]/lambdamicron + c[2]/pow(lambdamicron,3.5) );
    }
    static double Laurent(double lambdamicron, QList<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,-10) + c[7]*pow(lambdamicron,-12) + c[8]*pow(lambdamicron,-14) + c[9]*pow(lambdamicron,-16) + c[10]*pow(lambdamicron,-18) + c[11]*pow(lambdamicron,-20) );
    }
    static double GlassManufacturerLaurent(double lambdamicron, QList<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,4) );
    }
    static double GlassManufacturerSellmeier(double lambdamicron, QList<double> c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[1]) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[3]) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[5]) + c[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[7]) + c[8]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[9]) + c[10]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[11]) );
    }
    static double StandardSellmeier(double lambdamicron, QList<double> c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[1],2)) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[3],2)) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[5],2)) + c[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[7],2)) + c[8]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[9],2)) + c[10]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[11],2)) );
    }
    static double Cauchy(double lambdamicron, QList<double> c){
        return c[0] + c[1]*pow(lambdamicron,-2) + c[2]*pow(lambdamicron,-4);
    }
    static double Hartman(double lambdamicron, QList<double> c){
        return c[0] + c[1]/pow((c[2]-lambdamicron), 1.2);
    }


private:

    //fundamentals
    QString _name;
    QString _supplyer;
    QString _status;
    QString _MIL;
    double _nd;
    double _ne;   
    double _vd;
    double _ve;   
    double _PgF;
    double _dPgF;

    QString _comment;

    double _lambdaMax;
    double _lambdaMin;

    DispersionData* _dispersionData;
    TransmittanceData* _transmittanceData;
    ThermalData* _thermalData;
};

#endif // GLASS_H
