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

#include "spectral_line.h"
#include <math.h>
#include <QDebug>

//http://www.hoya-opticalworld.com/japanese/technical/002.html
const double SpectralLine::t = 1013.980;
const double SpectralLine::s =  852.110;
const double SpectralLine::r =  706.519;
const double SpectralLine::C =  656.273;
const double SpectralLine::C_=  643.847;
const double SpectralLine::D =  589.294;
const double SpectralLine::d =  587.562;
const double SpectralLine::e =  546.074;
const double SpectralLine::F =  486.133;
const double SpectralLine::F_=  479.991;
const double SpectralLine::g =  435.834;
const double SpectralLine::h =  404.656;
const double SpectralLine::i =  365.015;


/**
 * @brief Get wavelength of the line
 * @param spectralname
 * @return
 */
double SpectralLine::wavelength(const QString& spectralname)
{
    if(spectralname == "t"){
        return SpectralLine::t;
    }
    else if(spectralname == "s"){
        return SpectralLine::s;
    }
    else if(spectralname == "r"){
        return SpectralLine::r;
    }
    else if(spectralname == "C"){
        return SpectralLine::C;
    }
    else if(spectralname == "C_"){
        return SpectralLine::C_;
    }
    else if(spectralname == "D"){
        return SpectralLine::D;
    }
    else if(spectralname == "d"){
        return SpectralLine::d;
    }
    else if(spectralname == "e"){
        return SpectralLine::e;
    }
    else if(spectralname == "F"){
        return SpectralLine::F;
    }
    else if(spectralname == "F_"){
        return SpectralLine::F_;
    }
    else if(spectralname == "g"){
        return SpectralLine::g;
    }
    else if(spectralname == "h"){
        return SpectralLine::h;
    }
    else if(spectralname == "i"){
        return SpectralLine::i;
    }
    else{
        qDebug() << "Unknown spectral name: " << spectralname;
        return NAN;
    }
}

