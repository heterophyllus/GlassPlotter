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

#include "spline.h" // c++ cubic spline library, Tino Kluge (ttk448 at gmail.com), https://github.com/ttk592/spline
#include "spectral_line.h"
#include "dispersion_formula.h"

#include "Eigen/Dense"

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

    _dispersionData = QVector<double>(_dispersion_data_size, 0.0);

    _wavelengthData.clear();
    _transmittanceData.clear();
    _thicknessData.clear();

    _thermalData = QVector<double>(_thermal_data_size, 0.0);
}


Glass::~Glass()
{
    _dispersionData.clear();
    _wavelengthData.clear();
    _transmittanceData.clear();
    _thicknessData.clear();
    _thermalData.clear();
}

double Glass::getValue(QString dname) const
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
        // return vd();
        return (index("d") - 1)/(index("F") - index("C"));
    }
    else if(dname == "ve"){
        // return ve();
        return (index("e") - 1)/(index("F_") - index("C_"));
    }
    else if(dname == "PgF"){
        // return PgF();
        return (index("g") - index("F")) / ( index("F") - index("C") );
    }
    else if(dname == "PCt"){
        return (index("C") - index("t")) / ( index("F_") - index("C_") );
    }
    else if(dname == "eta1"){ // Buchdahl dispersion coefficients
        return BuchdahlDispCoef(0);
    }
    else if(dname == "eta2"){ // Buchdahl dispersion coefficients
        return BuchdahlDispCoef(1);
    }
    else{
        return 0;
    }
}

double Glass::index(double lambdamicron) const
{
    switch(_formulaIndex){

    // Zemax AGF
    case 1:
        return DispersionFormula::Schott(lambdamicron,_dispersionData);
    case 2:
        return DispersionFormula::Sellmeier1(lambdamicron,_dispersionData);
    case 3:
        return DispersionFormula::Herzberger(lambdamicron,_dispersionData);
    case 4:
        return DispersionFormula::Sellmeier2(lambdamicron,_dispersionData);
    case 5:
        return DispersionFormula::Conrady(lambdamicron,_dispersionData);
    case 6:
        return DispersionFormula::Sellmeier3(lambdamicron,_dispersionData);
    case 7:
        return DispersionFormula::HandbookOfOptics1(lambdamicron,_dispersionData);
    case 8:
        return DispersionFormula::HandbookOfOptics2(lambdamicron,_dispersionData);
    case 9:
        return DispersionFormula::Sellmeier4(lambdamicron,_dispersionData);
    case 10:
        return DispersionFormula::Extended1(lambdamicron,_dispersionData);
    case 11:
        return DispersionFormula::Sellmeier5(lambdamicron,_dispersionData);
    case 12:
        return DispersionFormula::Extended2(lambdamicron,_dispersionData);
    case 13: // Unknown
        return 0;

    // CODEV XML
    case 101:
        return DispersionFormula::Laurent(lambdamicron,_dispersionData);
    case 102:
        return DispersionFormula::GlassManufacturerLaurent(lambdamicron,_dispersionData);
    case 103:
        return DispersionFormula::GlassManufacturerSellmeier(lambdamicron,_dispersionData);
    case 104:
        return DispersionFormula::StandardSellmeier(lambdamicron,_dispersionData);
    case 105:
        return DispersionFormula::Cauchy(lambdamicron,_dispersionData);
    case 106:
        return DispersionFormula::Hartman(lambdamicron,_dispersionData);
    default:
        return 0;
    }

}

double Glass::index(QString spectral) const
{
    return index(SpectralLine::wavelength(spectral)/1000.0); // unit:micron
}

QVector<double> Glass::index(QVector<double> vLambdamicron) const
{
    int ndata = vLambdamicron.size();
    QVector<double> vInd(ndata);

    for(int i = 0; i < ndata; i++){
        vInd[i] = index(vLambdamicron[i]);
    }

    return vInd;
}

double Glass::BuchdahlDispCoef(int n) const
{
    /* This is a test function.
     *
     * This function calculates Buchdahl dispersion coefficients.
     * The implementation referred:
     * https://github.com/mjhoptics/opticalglass/blob/master/opticalglass/buchdahl.py
     *
     * Input:
     *      -n : coefficients index (0,1)
     */

    Q_ASSERT(n <= 1);

    double nd = index(SpectralLine::d/1000.0);
    double nF = index(SpectralLine::F/1000.0);
    double nC = index(SpectralLine::C/1000.0);
    double wd = SpectralLine::d/1000.0;
    double wF = SpectralLine::F/1000.0;
    double wC = SpectralLine::C/1000.0;

    double omegaF = ( wF-wd )/( 1 + 2.5*(wF-wd) );
    double omegaC = ( wC-wd )/( 1 + 2.5*(wC-wd) );

    Eigen::MatrixXd A(2,2);
    A(0,0) = omegaF;
    A(0,1) = omegaF*omegaF;
    A(1,0) = omegaC;
    A(1,1) = omegaC*omegaC;

    Eigen::VectorXd b(2);
    b(0) = nF-nd;
    b(1) = nC-nd;

    Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);

    x /= (nd-1);

    return (double)x(n);
}

void Glass::setStatus(QString str)
{
    _status = str;
}

void Glass::setStatus(int n)
{
    switch(n)
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


double Glass::dispersionCoef(int n) const
{
    Q_ASSERT(_dispersion_data_size == _dispersionData.size());

    if( n < _dispersionData.size() ){
        return _dispersionData[n];
    }
    return 0;
}

void Glass::setDispCoef(int n, double val)
{
    Q_ASSERT(_dispersion_data_size == _dispersionData.size());

    if( n < _dispersionData.size() ){
        _dispersionData[n] = val;
    }
}


QString Glass::formulaName() const
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

double Glass::transmittance(double lambdamicron, double thi) const
{
    Q_ASSERT( (_wavelengthData.size() > 0) && (_transmittanceData.size() > 0) && (_thicknessData.size() > 0) );

    double ref_thi   = _thicknessData[0];
    int    dataCount = _transmittanceData.size();
    QVector<double> qvx(dataCount), qvy(dataCount);

    for(int i = 0; i < dataCount; i++)
    {
        qvx[i] = _wavelengthData[i];
        qvy[i] = pow(_transmittanceData[i], thi/ref_thi); // T^(t/ref_t)
    }

    tk::spline s;
    std::vector<double> sx(qvx.begin(), qvx.end());
    std::vector<double> sy(qvy.begin(), qvy.end());
    s.set_points(sx,sy);

    return s(lambdamicron);
}

QVector<double> Glass::transmittance(QVector<double> vLambdamicron, double thi) const
{
    Q_ASSERT( (_wavelengthData.size() > 0) && (_transmittanceData.size() > 0) && (_thicknessData.size() > 0) );

    // Spline interpolation is rewritten here to avoid being called many times.
    double ref_thi = _thicknessData[0];
    int  dataCount = _transmittanceData.size();
    QVector<double> qvx(dataCount), qvy(dataCount);

    for(int i = 0; i < dataCount; i++)
    {
        qvx[i] = _wavelengthData[i];
        qvy[i] = pow(_transmittanceData[i], thi/ref_thi); // T^(t/ref_t)
    }

    tk::spline s;
    std::vector<double> sx(qvx.begin(), qvx.end());
    std::vector<double> sy(qvy.begin(), qvy.end());
    s.set_points(sx,sy);

    qvx.clear();
    qvy.clear();

    // return vector output
    dataCount = vLambdamicron.size();
    QVector<double> y(dataCount);
    for(int i = 0; i < dataCount; i++)
    {
        y[i] = s(vLambdamicron[i]);
    }

    return y;
}

void Glass::getTransmittanceData(QList<double>& pvLambdamicron, QList<double>& pvTransmittance, QList<double>& pvThickness)
{
    pvLambdamicron  = _wavelengthData;
    pvTransmittance = _transmittanceData;
    pvThickness     = _thicknessData;
}

void Glass::appendTransmittanceData(double lambdamicron, double trans, double thick)
{
    _wavelengthData.append(lambdamicron);
    _transmittanceData.append(trans);
    _thicknessData.append(thick);
}


double Glass::dn_dt_abs(double T, double lambdamicron) const
{
    double dT = T - T0();
    double n  = index(lambdamicron);

    return (n*n-1)/(2*n) * ( D0() + 2*D1()*dT + 3*D2()*dT*dT + (E0() + 2*E1()*dT)/(lambdamicron*lambdamicron - Ltk()*Ltk()) );
}

QVector<double> Glass::dn_dt_abs(QVector<double> vT, double lambdamicron) const
{
    int ndata = vT.size();
    QVector<double> vDndt(ndata);

    for(int i = 0; i < ndata; i++){
        vDndt[i] = dn_dt_abs(vT[i],lambdamicron);
    }

    return vDndt;
}


void Glass::setThermalData(int n, double val)
{
    Q_ASSERT( _thermal_data_size == _thermalData.size() );

    if( n < _thermalData.size() ){
        _thermalData[n] = val;
    }
}

