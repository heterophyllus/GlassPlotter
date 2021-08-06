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
    _status   = "";
    _comment  = "";
    _MIL      = "";

    _lowTCE  = NAN;
    _highTCE = NAN;

    _relCost         = NAN;
    _climateResist   = NAN;
    _stainResist     = NAN;
    _acidResist      = NAN;
    _alkaliResist    = NAN;
    _phosphateResist = NAN;

    _formulaIndex = 1;
    _dispersionData = QVector<double>(_dispersion_data_size, 0.0);

    _thermalData = QVector<double>(_thermal_data_size, NAN);

    _lambdaMax = 0;
    _lambdaMin = 0;
}


Glass::~Glass()
{
    _formula_func_ptr = nullptr;
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
    else if(dname == "PCt_"){
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

// useful aliases
double Glass::nd() const
{
    return index("d");
}

double Glass::ne() const
{
    return index("e");
}

double Glass::vd() const
{
    return (index("d") - 1)/(index("F") - index("C"));
}

double Glass::ve() const
{
    return (index("e") - 1)/(index("F_") - index("C_"));
}

double Glass::Pxy(QString x, QString y) const
{
    return (index(x) - index(y)) / ( index("F") - index("C") );
}

double Glass::Pxy_(QString x, QString y) const
{
    return (index(x) - index(y)) / ( index("F_") - index("C_") );
}

double Glass::PgF() const
{
    return Pxy("g","F");
}

double Glass::PCt_() const
{
    return Pxy_("C","t");
}


double Glass::index(double lambdamicron) const
{
    if(_formula_func_ptr){
        return _formula_func_ptr(lambdamicron, _dispersionData);
    }else{
        return 1.0;
    }
}


double Glass::index(QString spectral) const
{
    if(_formula_func_ptr){
        double lambdamicron = SpectralLine::wavelength(spectral)/1000.0;
        return _formula_func_ptr(lambdamicron, _dispersionData);
    }else {
        return 1.0;
    }
}

QVector<double> Glass::index(const QVector<double>& vLambdamicron) const
{
    int ndata = vLambdamicron.size();
    QVector<double> vInd(ndata);

    for(int i = 0; i < ndata; i++){
        //vInd[i] = index(vLambdamicron[i]);
        vInd[i] = _formula_func_ptr(vLambdamicron[i], _dispersionData);
    }

    return vInd;
}


double Glass::BuchdahlDispCoef(int n) const
{
    Q_ASSERT(n <= 1);

    double wd = SpectralLine::d/1000.0;
    double wF = SpectralLine::F/1000.0;
    double wC = SpectralLine::C/1000.0;
    double nd = index(wd);
    double nF = index(wF);
    double nC = index(wC);

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
        _status = "-";
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

void Glass::setDispForm(int n)
{
    _formulaIndex = n;

    switch (n) {
    // -----> Zemax AGF
    case 1:
        _formula_func_ptr = &(DispersionFormula::Schott);
        break;
    case 2:
        _formula_func_ptr = &(DispersionFormula::Sellmeier1);
        break;
    case 3:
        _formula_func_ptr = &(DispersionFormula::Herzberger);
        break;
    case 4:
        _formula_func_ptr = &(DispersionFormula::Sellmeier2);
        break;
    case 5:
        _formula_func_ptr = &(DispersionFormula::Conrady);
        break;
    case 6:
        _formula_func_ptr = &(DispersionFormula::Sellmeier3);
        break;
    case 7:
        _formula_func_ptr = &(DispersionFormula::HandbookOfOptics1);
        break;
    case 8:
        _formula_func_ptr = &(DispersionFormula::HandbookOfOptics2);
        break;
    case 9:
        _formula_func_ptr = &(DispersionFormula::Sellmeier4);
        break;
    case 10:
        _formula_func_ptr = &(DispersionFormula::Extended1);
        break;
    case 11:
        _formula_func_ptr = &(DispersionFormula::Sellmeier5);
        break;
    case 12:
        _formula_func_ptr = &(DispersionFormula::Extended2);
        break;
    case 13: // Unknown
        _formula_func_ptr = nullptr;
        break;

    // -----> CodeV XML
    case 101:
        _formula_func_ptr = &(DispersionFormula::Laurent);
        break;
    case 102:
        _formula_func_ptr = &(DispersionFormula::GlassManufacturerLaurent);
        break;
    case 103:
        _formula_func_ptr = &(DispersionFormula::GlassManufacturerSellmeier);
        break;
    case 104:
        _formula_func_ptr = &(DispersionFormula::StandardSellmeier);
        break;
    case 105:
        _formula_func_ptr = &(DispersionFormula::Cauchy);
        break;
    case 106:
        _formula_func_ptr = &(DispersionFormula::Hartman);
        break;

    default:
        _formula_func_ptr = nullptr;
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

QVector<double> Glass::transmittance(const QVector<double>& vLambdamicron, double thi) const
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

