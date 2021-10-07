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

#include "glass_datasheet_form.h"
#include "ui_glass_datasheet_form.h"

#include "spectral_line.h"


GlassDataSheetForm::GlassDataSheetForm(Glass* glass, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GlassDataSheetForm)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setWindowTitle("DataSheet - " + glass->fullName());

    m_glass = glass;

    // set names
    ui->label_GlassName->setText( m_glass->productName() + " (" + m_glass->supplyer() + ")" );
    ui->label_Fundamental->setText( "nd= " + QString::number(m_glass->getValue("nd")) + "   vd= " + QString::number(m_glass->getValue("vd")) );

    // set up all tabs
    setUpBasicTab();
    setUpIndicesTab();
    setUpPartialTab();
    setUpDispersionTab();
    setUpThermalTab();
    setUpTransmittanceTab();
    setUpOtherDataTab();
}

GlassDataSheetForm::~GlassDataSheetForm()
{
    delete ui;
}

void GlassDataSheetForm::addGridItem(QGridLayout* gridLayout, int row, int col, QString str)
{
    QWidget* scrollAreaContents = gridLayout->parentWidget();

    QLabel* label = new QLabel(str, scrollAreaContents);
    gridLayout->addWidget(label, row, col, 1, 1);

    label = nullptr;
}


void GlassDataSheetForm::setUpBasicTab()
{
    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_Basic;
    QGridLayout* grid = new QGridLayout(scrollAreaContents);
    grid->setObjectName(QString::fromUtf8("gridLayout_Basic"));

    // name
    addGridItem(grid, 0, 0, "Name");
    addGridItem(grid, 0, 1, m_glass->productName());

    // catalog
    addGridItem(grid, 1, 0, "Catalog");
    addGridItem(grid, 1, 1, m_glass->supplyer());

    // MIL
    addGridItem(grid, 2, 0, "MIL");
    addGridItem(grid, 2, 1, m_glass->MIL());

    // status
    addGridItem(grid, 3, 0, "Status");
    addGridItem(grid, 3, 1, m_glass->status());

    // comment
    addGridItem(grid, 4, 0, "Comment");
    addGridItem(grid, 4, 1, m_glass->comment());
}

void GlassDataSheetForm::setUpIndicesTab()
{
    const int wvl_digit = 3;
    const int val_digit = 6;

    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents;
    QGridLayout* grid = new QGridLayout(scrollAreaContents);
    grid->setObjectName(QString::fromUtf8("gridLayout_Indices"));

    // table header
    addGridItem(grid, 0, 0, "Spectral");
    addGridItem(grid, 0, 1, "Wavelength");
    addGridItem(grid, 0, 2, "Index");

    // list up indices
    QStringList spectralList = {"t", "s", "r", "C", "d", "e", "F", "g", "h", "i"};
    QString     spectralLineName;

    int row;
    for(int i = 0; i < spectralList.size(); i++)
    {
        row = i + 1;

        // Spectral name
        spectralLineName = spectralList[i];
        addGridItem(grid, row, 0, spectralLineName);

        // lambda(nm)
        addGridItem(grid, row, 1, QString::number(SpectralLine::wavelength(spectralLineName), 'f', wvl_digit));

        // refractive index
        addGridItem(grid, row, 2, numToQString(m_glass->refractiveIndex_rel(spectralLineName), 'f', val_digit));
    }
}

void GlassDataSheetForm::setUpPartialTab()
{   
    const int digit = 6;

    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_Partial;
    QGridLayout* grid = new QGridLayout(scrollAreaContents);
    grid->setObjectName(QString::fromUtf8("gridLayout_Partial"));

    QStringList firsts, seconds;

    int row;

    // Px,y
    firsts  = QStringList( {"s", "C", "d", "e", "g", "i"} );
    seconds = QStringList( {"t", "s", "C", "d", "F", "h"} );

    for(int i = 0; i < firsts.size(); i++){
        row = i;
        addGridItem(grid, row, 0, "P " + firsts[i] + "," + seconds[i]);
        addGridItem(grid, row, 1, numToQString(m_glass->Pxy(firsts[i], seconds[i]), 'f', digit));
    }

    // P'x,y
    int rowOffset = grid->rowCount();
    firsts  = QStringList( {"s", "C_", "d",  "e", "g",  "i"} );
    seconds = QStringList( {"t", "s",  "C_", "d", "F_", "h"} );

    for(int j = 0; j < firsts.size(); j++)
    {
        row = j + rowOffset;

        addGridItem(grid, row, 0, "P' " + firsts[j] + "," + seconds[j]);
        addGridItem(grid, row, 1, numToQString(m_glass->Pxy_(firsts[j], seconds[j]), 'f', digit));
    }

}


void GlassDataSheetForm::setUpDispersionTab()
{   
    const int digit = 6;

    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_Dispersion;
    QGridLayout* grid = new QGridLayout(scrollAreaContents);
    grid->setObjectName(QString::fromUtf8("gridLayout_Dispersion"));

    int row;

    // formula name
    addGridItem(grid, 0, 0, "Formula");
    addGridItem(grid, 0, 1, m_glass->formulaName());

    // list up coefficients
    for(int i = 0; i < m_glass->dispersionCoefCount();i++)
    {
        row = i + 1;

        addGridItem(grid, row, 0, "C" + QString::number(i));                             // C0, C1, C2...
        addGridItem(grid, row, 1, numToQString(m_glass->dispersionCoef(i),'g',digit)); // coefficient value
    }
}


void GlassDataSheetForm::setUpThermalTab()
{   
    const int digit = 6;

    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_dndT;
    QGridLayout* grid = new QGridLayout(scrollAreaContents);
    grid->setObjectName(QString::fromUtf8("gridLayout_dndT"));

    QStringList   coefNames  = QStringList(   {"D0", "D1", "D2", "E0", "E1", "Ltk", "T0"} );
    QList<double> coefValues = QList<double>( {m_glass->D0(), m_glass->D1(), m_glass->D2(), m_glass->E0(), m_glass->E1(), m_glass->Ltk(), m_glass->Tref()} );
    for(int i = 0; i < coefNames.size(); i++){
        addGridItem(grid, i, 0, coefNames[i]);
        addGridItem(grid, i, 1, numToQString(coefValues[i], 'g', digit));
    }

}


void GlassDataSheetForm::setUpTransmittanceTab()
{
    const int wvl_digit = 3;
    const int thi_digit = 3;
    const int tra_digit = 3;

    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_Transmittance;
    QGridLayout* grid = new QGridLayout(scrollAreaContents);
    grid->setObjectName(QString::fromUtf8("gridLayout_Transmittance"));

    int row;

    // set lambda max and min
    row = 0;
    addGridItem(grid, row, 0, "Lambda Min/Max");
    addGridItem(grid, row, 1, numToQString(m_glass->lambdaMin(),'f',wvl_digit));
    addGridItem(grid, row, 2, numToQString(m_glass->lambdaMax(),'f',wvl_digit));

    // insert space
    ++row;
    addGridItem(grid, row, 0, "");

    // set header labels
    ++row;
    addGridItem(grid, row, 0, "Wavelength(micron)");
    addGridItem(grid, row, 1, "Transmittance");
    addGridItem(grid, row, 2, "Thickness");

    // set transmittance data
    QList<double> lambdamicrons, transmittances, thicknesses;
    m_glass->getTransmittanceData(lambdamicrons, transmittances, thicknesses);
    int ndata = lambdamicrons.size();

    int rowOffset = row+1;
    for(int i = 0; i < ndata; i++)
    {
        row = i + rowOffset;

        addGridItem(grid, row, 0, numToQString(lambdamicrons[i],  'f', wvl_digit));
        addGridItem(grid, row, 1, numToQString(transmittances[i], 'f', tra_digit));
        addGridItem(grid, row, 2, numToQString(thicknesses[i],    'f', thi_digit));
    }

}


void GlassDataSheetForm::setUpOtherDataTab()
{
    const int digit = 2;

    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_OtherData;
    QGridLayout* grid = new QGridLayout(scrollAreaContents);
    grid->setObjectName(QString::fromUtf8("gridLayout_otherdata"));

    addGridItem(grid, 0, 0, "Relative Cost");
    addGridItem(grid, 0, 1, numToQString(m_glass->relCost(), 'f', digit));

    addGridItem(grid, 1, 0, "Climate Resist");
    addGridItem(grid, 1, 1, numToQString(m_glass->climateResist(), 'f', digit));

    addGridItem(grid, 2, 0, "Stain Resist");
    addGridItem(grid, 2, 1, numToQString(m_glass->stainResist(), 'f', digit));

    addGridItem(grid, 3, 0, "Acid Resist");
    addGridItem(grid, 3, 1, numToQString(m_glass->acidResist(), 'f', digit));

    addGridItem(grid, 4, 0, "Alkali Resist");
    addGridItem(grid, 4, 1, numToQString(m_glass->alkaliResist(), 'f', digit));

    addGridItem(grid, 5, 0, "Phosphate Resist");
    addGridItem(grid, 5, 1, numToQString(m_glass->phosphateResist(), 'f', digit));
}

