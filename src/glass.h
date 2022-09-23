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
#include <QVector>
#include <QtMath>

class Glass
{
public:
    Glass();
    ~Glass();

    static void setCurrentTemperature(double t);

    double relative_wavelength(double lambdainput) const;

    // fundamental data
    double          refractiveIndex(double lambdamicron) const;
    double          refractiveIndex(const QString& spectral) const;
    QVector<double> refractiveIndex(const QVector<double>& vLambdamicron) const;

    inline QString  fullName() const;
    inline QString  productName() const;
    inline QString  supplier() const;
    inline QString  status() const;
    inline QString  MIL() const;
    inline QString  comment() const;

    double Pxy(const QString& x, const QString& y) const;
    double Pxy_(const QString& x, const QString& y) const;


    /** convenience function to get glass property */
    double getValue(const QString& dname) const;

    double BuchdahlDispCoef(int n) const;

    inline void setName(const QString& str);
    inline void setSupplier(const QString& str);
    inline void setMIL(const QString& str);
    inline void setStatus(const QString& str);

    void setStatus(int n);
    inline void setComment(const QString& str);


    // extra data
    inline double lowTCE() const;
    inline double highTCE() const;

    inline void setLowTCE(double val);
    inline void setHighTCE(double val);

    // dispersion data
    inline int formulaIndex() const;
    inline QString formulaName() const;
    inline int dispersionCoefCount() const;
    inline double dispersionCoef(int n) const;

    void  setDispForm(int n);
    void  setDispCoef(int n, double val);


    // thermal data
    inline bool hasThermalData() const;
    inline double D0()  const;
    inline double D1()  const;
    inline double D2()  const;
    inline double E0()  const;
    inline double E1()  const;
    inline double Ltk() const;
    inline double Tref()  const;

    inline QVector<double> getThermalData() const;
    double          dn_dt_abs(double T, double lambdamicron) const;
    QVector<double> dn_dt_abs(const QVector<double>& vT, double lambdamicron) const;

    double delta_n_abs(double T, double lambdamicron) const;

    inline void setHasThermalData(bool state);
    void setThermalData(int n, double val);

    // other data
    inline double relCost() const;
    inline double climateResist() const;
    inline double stainResist() const ;
    inline double acidResist() const;
    inline double alkaliResist() const;
    inline double phosphateResist() const ;

    inline void  setRelCost(double val);
    inline void  setClimateResist(double val);
    inline void  setStainResist(double val);
    inline void  setAcidResist(double val);
    inline void  setAlkaliResist(double val);
    inline void  setPhosphateResist(double val);

    // transmittance data
    double          transmittance(double lambdamicron, double thi = 25) const;
    QVector<double> transmittance(const QVector<double>& vLambdamicron, double thi = 25) const;

    inline double  lambdaMin() const;
    inline double  lambdaMax() const;
    void   getTransmittanceData(QList<double>& pvLambdamicron, QList<double>& pvTransmittance, QList<double>& pvThickness);

    void  appendTransmittanceData(double lambdamicron, double trans, double thick);
    inline void  setLambdaMin(double val);
    inline void  setLambdaMax(double val);


private:
    double          refractiveIndex_abs_Tref(double lambdamicron) const;
    double          refractiveIndex_rel_Tref(double lambdamicron) const;
    double          refractiveIndex_abs(double lambdamicron, double T) const;
    double          refractiveIndex_rel(double lambdamicron, double T) const;
    QVector<double> refractiveIndex_rel_Tref(const QVector<double>& vLambdamicron) const;
    QVector<double> refractiveIndex_rel(const QVector<double>& vLambdamicron, double T) const;

    /** current temperature */
    static double T_;

    QString product_name_;
    QString supplier_;
    QString status_;
    QString MIL_;
    QString comment_;

    // extra data
    double lowTCE_; // TCE: thermal coefficient of expansion
    double highTCE_;

    // dispersion data
    const int       dispersion_data_size_ = 12;
    QVector<double> dispersion_data_;
    int             formula_index_;
    QString         formula_name_;
    double (*formula_func_ptr_)(double, const QVector<double>&);

    // thermal data
    bool            hasThermalData_;
    const int       thermal_data_size_ = 7;
    QVector<double> thermal_data_; //<D0> <D1> <D2> <E0> <E1> <Ltk> <temp>
    double          Tref_;

    // other data
    // Many glasses does not have valid data in OD line, so their default value should be NaN for "no data".
    double rel_cost_;
    double climate_resist_;
    double stain_resist_;
    double acid_resist_;
    double alkali_resist_;
    double phosphate_resist_;

    // transmittance data
    double        lambda_max_;
    double        lambda_min_;
    QList<double> wavelength_data_; //micron
    QList<double> transmittance_data_;
    QList<double> thickness_data_;
};

//************************************************************************************************************
// getter
QString Glass::fullName() const
{
    return product_name_ + "_" + supplier_;
}

QString Glass::productName() const
{
    return product_name_;
}

QString Glass::supplier() const
{
    return supplier_;
}

QString Glass::status() const
{
    return status_;
}

QString Glass::MIL() const
{
    return MIL_;
}

QString Glass::comment() const
{
    return comment_;
}

double Glass::lowTCE() const
{
    return lowTCE_;
}

double Glass::highTCE() const
{
    return highTCE_;
}

int Glass::formulaIndex() const
{
    return formula_index_;
}

QString Glass::formulaName() const
{
    return formula_name_;
}

int Glass::dispersionCoefCount() const
{
    return dispersion_data_.size();
}

double Glass::dispersionCoef(int n) const
{
    return dispersion_data_[n];
}

bool Glass::hasThermalData() const
{
    return hasThermalData_;
}

double Glass::D0() const
{
    return thermal_data_[0];
}

double Glass::D1() const
{
    return thermal_data_[1];
}

double Glass::D2() const
{
    return thermal_data_[2];
}

double Glass::E0() const
{
    return thermal_data_[3];
}

double Glass::E1() const
{
    return thermal_data_[4];
}

double Glass::Ltk() const
{
    return thermal_data_[5];
}

double Glass::Tref() const
{
    return Tref_;
}

double Glass::relCost() const
{
    return rel_cost_;
}

double Glass::climateResist() const
{
    return climate_resist_;
}

double Glass::stainResist() const
{
    return stain_resist_;
}

double Glass::acidResist() const
{
    return acid_resist_;
}

double Glass::alkaliResist() const
{
    return alkali_resist_;
}

double Glass::phosphateResist() const
{
    return phosphate_resist_;
}

double Glass::lambdaMax() const
{
    return lambda_max_;
}

double Glass::lambdaMin() const
{
    return lambda_min_;
}

QVector<double> Glass::getThermalData() const
{
    return thermal_data_;
}


// setter ***************************************
void Glass::setName(const QString& str)
{
    product_name_ = str;
}

void Glass::setSupplier(const QString& str)
{
    supplier_ = str;
}

void Glass::setMIL(const QString& str)
{
    MIL_ = str;
}

void Glass::setComment(const QString& str)
{
    comment_ = str;
}

void Glass::setStatus(const QString& str)
{
    status_ = str;
}

void Glass::setLowTCE(double val)
{
    lowTCE_ = val;
}

void Glass::setHighTCE(double val)
{
    highTCE_ = val;
}


void Glass::setRelCost(double val)
{
    rel_cost_ = val;
}

void Glass::setClimateResist(double val)
{
    climate_resist_ = val;
}

void Glass::setStainResist(double val)
{
    stain_resist_ = val;
}

void Glass::setAcidResist(double val)
{
    acid_resist_ = val;
}

void Glass::setAlkaliResist(double val)
{
    alkali_resist_ = val;
}

void Glass::setPhosphateResist(double val)
{
    phosphate_resist_ = val;
}

void Glass::setLambdaMax(double val)
{
    lambda_max_ = val;
}

void Glass::setLambdaMin(double val)
{
    lambda_min_ = val;
}

void Glass::setHasThermalData(bool state)
{
    hasThermalData_ = state;
}


#endif // GLASS_H
