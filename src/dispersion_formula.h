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


#ifndef DISPERSION_FORMULA_H
#define DISPERSION_FORMULA_H

#include <QVector>
#include <QStringList>

class DispersionFormula
{
public:
    static double Schott(double lambdamicron, QVector<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) );
    }
    static double Sellmeier1(double lambdamicron, QVector<double> c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[1]) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[3]) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[5]) );
    }
    static double Sellmeier2(double lambdamicron, QVector<double> c){
        return sqrt( 1 + c[0] + c[1]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[2]) + c[3]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[4]) );
    }
    static double Sellmeier3(double lambdamicron, QVector<double> c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[1]) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[3]) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[5]) + c[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[7]) );
    }
    static double Sellmeier4(double lambdamicron, QVector<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[2]) + c[3]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[4]) );
    }
    static double Sellmeier5(double lambdamicron, QVector<double> c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[1]) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[3]) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[5]) + c[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[7]) + c[8]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[9]) );
    }
    static double Herzberger(double lambdamicron, QVector<double> c){
        double L = 1/(pow(lambdamicron,2)-0.028);
        return ( c[0] + c[1]*L + c[2]*pow(L,2) + c[3]*pow(lambdamicron,2) + c[4]*pow(lambdamicron,4) + c[5]*pow(lambdamicron,6) );
    }
    static double HandbookOfOptics1(double lambdamicron, QVector<double> c){
        return sqrt( c[0] + c[1]/(pow(lambdamicron,2)-c[2]) - c[3]*pow(lambdamicron,2) );
    }
    static double HandbookOfOptics2(double lambdamicron, QVector<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[2]) - c[3]*pow(lambdamicron,2) );
    }
    static double Extended1(double lambdamicron, QVector<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,-10) + c[7]*pow(lambdamicron,-12) );
    }
    static double Extended2(double lambdamicron, QVector<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,4) + c[7]*pow(lambdamicron,6) );
    }
    static double Conrady(double lambdamicron, QVector<double> c){
        return ( c[0] + c[1]/lambdamicron + c[2]/pow(lambdamicron,3.5) );
    }
    static double Laurent(double lambdamicron, QVector<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,-10) + c[7]*pow(lambdamicron,-12) + c[8]*pow(lambdamicron,-14) + c[9]*pow(lambdamicron,-16) + c[10]*pow(lambdamicron,-18) + c[11]*pow(lambdamicron,-20) );
    }
    static double GlassManufacturerLaurent(double lambdamicron, QVector<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,4) );
    }
    static double GlassManufacturerSellmeier(double lambdamicron, QVector<double> c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[1]) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[3]) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[5]) + c[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[7]) + c[8]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[9]) + c[10]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[11]) );
    }
    static double StandardSellmeier(double lambdamicron, QVector<double> c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[1],2)) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[3],2)) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[5],2)) + c[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[7],2)) + c[8]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[9],2)) + c[10]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[11],2)) );
    }
    static double Cauchy(double lambdamicron, QVector<double> c){
        return c[0] + c[1]*pow(lambdamicron,-2) + c[2]*pow(lambdamicron,-4);
    }
    static double Hartman(double lambdamicron, QVector<double> c){
        return c[0] + c[1]/pow((c[2]-lambdamicron), 1.2);
    }

    /**
     * @brief get dispersion formula name list
     */
    static QStringList formulaNames()
    {
        QStringList names;
        names.reserve(18);

        // Zemax AGF
        names.append("Schott");
        names.append("Sellmeier1");
        names.append("Herzberger");
        names.append("Sellmeier2");
        names.append("Conrady");
        names.append("Sellmeier3");
        names.append("HandbookOfOptics1");
        names.append("HandbookOfOptics2");
        names.append("Sellmeier4");
        names.append("Extended1");
        names.append("Sellmeier5");
        names.append("Extended2");

        // CODEV XML
        names.append("Laurent");
        names.append("Glass Manufacturer Laurent");
        names.append("Glass Manufacturer Sellmeier");
        names.append("Standard Sellmeier");
        names.append("Cauchy");
        names.append("Hartman");

        return names;
    }
};

#endif // DISPERSION_FORMULA_H
