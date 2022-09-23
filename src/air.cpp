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

#include "air.h"
#include "math.h"

double Air::refractive_index_abs(double lambdamicron, double T, double P)
{
    constexpr double P0 = 101325.0;
    constexpr double Tref = 15;
    double nref = refractive_index_15degC_1atm(lambdamicron);
    double num = nref - 1.0;
    double denom = 1.0 + (T-Tref)*(3.4785*pow(10,-3));

    return ( 1.0 + (num/denom)*(P/P0) );
}

double Air::refractive_index_15degC_1atm(double lambdamicron)
{
    constexpr double term1 = 6432.8;
    double term2 = 2949810.0*pow(lambdamicron, 2)/( 146.0*pow(lambdamicron,2) - 1.0 );
    double term3 = 25540.0*pow(lambdamicron,2)/( 41.0*pow(lambdamicron,2) - 1.0 );
    double nref = 1.0 + (term1 + term2 + term3)*pow(10, -8);

    return nref;
}


