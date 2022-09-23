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

#include <QDebug>
#include "glass.h"

#include "spline.h" // c++ cubic spline library, Tino Kluge (ttk448 at gmail.com), https://github.com/ttk592/spline
#include "spectral_line.h"
#include "dispersion_formula.h"
#include "air.h"
#include "Eigen/Dense"

double Glass::T_ = 25;

Glass::Glass()
{    
    product_name_ = "";
    supplier_ = "";
    status_   = "";
    comment_  = "";
    MIL_      = "";

    lowTCE_  = NAN;
    highTCE_ = NAN;

    rel_cost_         = NAN;
    climate_resist_   = NAN;
    stain_resist_     = NAN;
    acid_resist_      = NAN;
    alkali_resist_    = NAN;
    phosphate_resist_ = NAN;

    formula_index_ = 1;
    dispersion_data_ = QVector<double>(dispersion_data_size_, 0.0);

    hasThermalData_ = false;
    thermal_data_ = QVector<double>(thermal_data_size_, NAN);
    Tref_ = 25;

    lambda_max_ = 0;
    lambda_min_ = 0;

}


Glass::~Glass()
{
    formula_func_ptr_ = nullptr;
    dispersion_data_.clear();
    wavelength_data_.clear();
    transmittance_data_.clear();
    thickness_data_.clear();
    thermal_data_.clear();
}


void Glass::setCurrentTemperature(double t)
{
    T_ = t;
}

double Glass::relative_wavelength(double lambdainput) const
{
    constexpr double P = 101325.0;
    double n_air_system = Air::refractive_index_abs(lambdainput, T_, P);
    double n_air_ref    = Air::refractive_index_abs(lambdainput, Tref_, P);
    double lambda_rel   = lambdainput*(n_air_system/n_air_ref);

    return lambda_rel;
}

double Glass::getValue(const QString& dname) const
{
    if(dname == "nd"){
        return refractiveIndex("d");
    }
    else if(dname == "ne"){
        return refractiveIndex("e");
    }
    else if(dname == "vd"){
        return (refractiveIndex("d") - 1)/(refractiveIndex("F") - refractiveIndex("C"));
    }
    else if(dname == "ve"){
        return (refractiveIndex("e") - 1)/(refractiveIndex("F_") - refractiveIndex("C_"));
    }
    else if(dname == "PgF"){
        return (refractiveIndex("g") - refractiveIndex("F")) / ( refractiveIndex("F") - refractiveIndex("C") );
    }
    else if(dname == "PCt_"){
        return (refractiveIndex("C") - refractiveIndex("t")) / ( refractiveIndex("F_") - refractiveIndex("C_") );
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


double Glass::Pxy(const QString& x, const QString& y) const
{
    double nx = refractiveIndex(x);
    double ny = refractiveIndex(y);
    double nF = refractiveIndex("F");
    double nC = refractiveIndex("C");

    return ( nx - ny )/( nF- nC);
}

double Glass::Pxy_(const QString& x, const QString& y) const
{
    double nx  = refractiveIndex(x);
    double ny  = refractiveIndex(y);
    double nF_ = refractiveIndex("F_");
    double nC_ = refractiveIndex("C_");

    return ( nx - ny )/( nF_- nC_);
}

double Glass::refractiveIndex(double lambdamicron) const
{
    double lambda_rel = relative_wavelength(lambdamicron);
    return refractiveIndex_rel(lambda_rel, T_);
    //return refractiveIndex_rel(lambdamicron, T_);
}

double Glass::refractiveIndex(const QString& spectral) const
{
    return refractiveIndex(SpectralLine::wavelength(spectral)/1000.0);
}

QVector<double> Glass::refractiveIndex(const QVector<double> &vLambdamicron) const
{
    return refractiveIndex_rel(vLambdamicron, T_);
}

double Glass::refractiveIndex_rel_Tref(double lambdamicron) const
{
    if(formula_func_ptr_){
        return formula_func_ptr_(lambdamicron, dispersion_data_);
    }else{
        return NAN;
    }
}

double Glass::refractiveIndex_abs_Tref(double lambdamicron) const
{
    constexpr double P = 101325.0;
    double n_air_T0 = Air::refractive_index_abs(lambdamicron, Tref_, P);
    double n_rel_T0 = refractiveIndex_rel_Tref(lambdamicron);
    double n_abs_T0 = n_rel_T0*n_air_T0;

    return n_abs_T0;
}

double Glass::refractiveIndex_abs(double lambdamicron, double T) const
{
    double dn = 0.0;
    if(hasThermalData_){
        dn = delta_n_abs(T, lambdamicron);
    }

    double n_abs_T0 = refractiveIndex_abs_Tref(lambdamicron);
    double n_abs = n_abs_T0 + dn;

    return n_abs;
}

double Glass::refractiveIndex_rel(double lambdamicron, double T) const
{
    double n_abs = refractiveIndex_abs(lambdamicron, T);
    double n_air = Air::refractive_index_abs(lambdamicron, T);
    double n_rel = n_abs/n_air;

    return n_rel;
}


QVector<double> Glass::refractiveIndex_rel_Tref(const QVector<double>& vLambdamicron) const
{
    const int dataCount = vLambdamicron.size();
    QVector<double> n_rel_Tref_v(dataCount);

    for(int i = 0; i < dataCount; i++) {
        n_rel_Tref_v[i] = refractiveIndex_rel_Tref(vLambdamicron[i]);
    }

    return n_rel_Tref_v;
}


QVector<double> Glass::refractiveIndex_rel(const QVector<double> &vLambdamicron, double T) const
{
    const int dataCount = vLambdamicron.size();
    QVector<double> n_rel_v(dataCount);

    for(int i = 0; i < dataCount; i++){
        n_rel_v[i] = refractiveIndex_rel(vLambdamicron[i], T);
    }

    return n_rel_v;
}

double Glass::BuchdahlDispCoef(int n) const
{
    Q_ASSERT(n <= 1);

    double wd = SpectralLine::d/1000.0;
    double wF = SpectralLine::F/1000.0;
    double wC = SpectralLine::C/1000.0;
    double nd = refractiveIndex(wd);
    double nF = refractiveIndex(wF);
    double nC = refractiveIndex(wC);

    double omegaF = ( wF-wd )/( 1 + 2.5*(wF-wd) );
    double omegaC = ( wC-wd )/( 1 + 2.5*(wC-wd) );

    Eigen::Matrix2d A(2,2);
    A(0,0) = omegaF;
    A(0,1) = omegaF*omegaF;
    A(1,0) = omegaC;
    A(1,1) = omegaC*omegaC;

    Eigen::Vector2d b(2);
    b(0) = nF-nd;
    b(1) = nC-nd;

    Eigen::Vector2d x = A.colPivHouseholderQr().solve(b);

    x /= (nd-1);

    return (double)x(n);
}


void Glass::setStatus(int n)
{
    switch(n)
    {
    case 1:
        status_ = "Preferred";
        break;
    case 2:
        status_ = "Obsolete";
        break;
    case 3:
        status_ = "Special";
        break;
    case 4:
        status_ = "Melt";
        break;
    default:
        status_ = "-";
    }
}



void Glass::setDispCoef(int n, double val)
{
    Q_ASSERT(dispersion_data_size_ == dispersion_data_.size());

    if( n < dispersion_data_.size() ){
        dispersion_data_[n] = val;
    }
}

void Glass::setDispForm(int n)
{
    formula_index_ = n;

    switch (n) {
    // -----> Zemax AGF
    case 1:
        formula_func_ptr_ = &(DispersionFormula::Schott);
        formula_name_ = "Schott";
        break;
    case 2:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier1);
        formula_name_ = "Sellmeier1";
        break;
    case 3:
        formula_func_ptr_ = &(DispersionFormula::Herzberger);
        formula_name_ = "Herzberger";
        break;
    case 4:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier2);
        formula_name_ = "Sellmeier2";
        break;
    case 5:
        formula_func_ptr_ = &(DispersionFormula::Conrady);
        formula_name_ = "Conrady";
        break;
    case 6:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier3);
        formula_name_ = "Sellmeier3";
        break;
    case 7:
        formula_func_ptr_ = &(DispersionFormula::HandbookOfOptics1);
        formula_name_ = "Handbook of Optics1";
        break;
    case 8:
        formula_func_ptr_ = &(DispersionFormula::HandbookOfOptics2);
        formula_name_ = "Handbook of Optics2";
        break;
    case 9:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier4);
        formula_name_ = "Sellmeier4";
        break;
    case 10:
        formula_func_ptr_ = &(DispersionFormula::Extended1);
        formula_name_ = "Extended1";
        break;
    case 11:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier5);
        formula_name_ = "Sellmeier5";
        break;
    case 12:
        formula_func_ptr_ = &(DispersionFormula::Extended2);
        formula_name_ = "Extended2";
        break;
    case 13: // Unknown
        if(supplier_.contains("hikari", Qt::CaseInsensitive)){
            formula_func_ptr_ = &(DispersionFormula::Nikon_Hikari);
            formula_name_ = "Nikon Hikari";
        }else{
            formula_func_ptr_ = nullptr;
            formula_name_ = "Unknown";
        }
        break;

    // -----> CodeV XML
    case 101:
        formula_func_ptr_ = &(DispersionFormula::Laurent);
        formula_name_ = "Laurent";
        break;
    case 102:
        formula_func_ptr_ = &(DispersionFormula::GlassManufacturerLaurent);
        formula_name_ = "Glass Manufacturer Laurent";
        break;
    case 103:
        formula_func_ptr_ = &(DispersionFormula::GlassManufacturerSellmeier);
        formula_name_ = "Glass Manufacturer Sellmeier";
        break;
    case 104:
        formula_func_ptr_ = &(DispersionFormula::StandardSellmeier);
        formula_name_ = "Standard Sellmeier";
        break;
    case 105:
        formula_func_ptr_ = &(DispersionFormula::Cauchy);
        formula_name_ = "Cauchy";
        break;
    case 106:
        formula_func_ptr_ = &(DispersionFormula::Hartman);
        formula_name_ = "Hartman";
        break;

    default:
        formula_func_ptr_ = nullptr;
        formula_name_ = "Unknown";
    }

}


double Glass::transmittance(double lambdamicron, double thi) const
{
    Q_ASSERT( (wavelength_data_.size() > 0) && (transmittance_data_.size() > 0) && (thickness_data_.size() > 0) );

    double ref_thi   = thickness_data_[0];
    int    dataCount = transmittance_data_.size();
    QVector<double> qvx(dataCount), qvy(dataCount);

    for(int i = 0; i < dataCount; i++)
    {
        qvx[i] = wavelength_data_[i];
        qvy[i] = pow(transmittance_data_[i], thi/ref_thi); // T^(t/ref_t)
    }

    tk::spline s;
    std::vector<double> sx(qvx.begin(), qvx.end());
    std::vector<double> sy(qvy.begin(), qvy.end());
    s.set_points(sx,sy);

    return s(lambdamicron);
}

QVector<double> Glass::transmittance(const QVector<double>& vLambdamicron, double thi) const
{
    Q_ASSERT( (wavelength_data_.size() > 0) && (transmittance_data_.size() > 0) && (thickness_data_.size() > 0) );

    // Spline interpolation is rewritten here to avoid being called many times.
    double ref_thi = thickness_data_[0];
    int  dataCount = transmittance_data_.size();
    QVector<double> qvx(dataCount), qvy(dataCount);

    for(int i = 0; i < dataCount; i++)
    {
        qvx[i] = wavelength_data_[i];
        qvy[i] = pow(transmittance_data_[i], thi/ref_thi); // T^(t/ref_t)
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
    pvLambdamicron  = wavelength_data_;
    pvTransmittance = transmittance_data_;
    pvThickness     = thickness_data_;
}

void Glass::appendTransmittanceData(double lambdamicron, double trans, double thick)
{
    wavelength_data_.append(lambdamicron);
    transmittance_data_.append(trans);
    thickness_data_.append(thick);
}


double Glass::dn_dt_abs(double T, double lambdamicron) const
{
    double dT = T - Tref_;
    double n  = refractiveIndex_abs_Tref(lambdamicron);

    return (n*n-1)/(2*n) * ( D0() + 2*D1()*dT + 3*D2()*dT*dT + (E0() + 2*E1()*dT)/(lambdamicron*lambdamicron - Ltk()*Ltk()) );
}

QVector<double> Glass::dn_dt_abs(const QVector<double>& vT, double lambdamicron) const
{
    int ndata = vT.size();
    QVector<double> vDndt(ndata);

    for(int i = 0; i < ndata; i++){
        vDndt[i] = dn_dt_abs(vT[i],lambdamicron);
    }

    return vDndt;
}


double Glass::delta_n_abs(double T, double lambdamicron) const
{
    double dT = T - Tref_;
    double Stk = (Ltk() > 0.0) - (Ltk() < 0.0);
    double n  = refractiveIndex_rel_Tref(lambdamicron);

    // Schott technical document
    return (n*n-1)/(2*n) * ( D0()*dT+ D1()*dT*dT + D2()*dT*dT*dT + (E0()*dT + E1()*dT*dT)/(lambdamicron*lambdamicron - Ltk()*Ltk()) );

    // Zemax manual
    //return (n*n-1)/(2*n) * ( D0()*dT+ D1()*dT*dT + D2()*dT*dT*dT + (E0()*dT + E1()*dT*dT)/(lambdamicron*lambdamicron - Stk*Ltk()*Ltk()) );
}

void Glass::setThermalData(int n, double val)
{
    Q_ASSERT( thermal_data_size_ == thermal_data_.size() );

    if( n < thermal_data_.size() ){
        thermal_data_[n] = val;

        if(n == 6){
            Tref_ = thermal_data_[6];
        }
    }
}

