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
#include "glass.h"

GlassDataSheetForm::GlassDataSheetForm(Glass* glass, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GlassDataSheetForm)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setWindowTitle("DataSheet - " + glass->name());

    m_glass = glass;

    // set names
    ui->label_GlassName->setText( m_glass->name() + " (" + m_glass->supplyer() + ")" );
    ui->label_Fundamental->setText( "nd= " + QString::number(m_glass->nd()) + "   Vd= " + QString::number(m_glass->vd()) );

    // The layout is created with reference to Schott's website
    setUpBasicTab();
    setUpIndicesTab();
    setUpPartialTab();
    setUpDispersionTab();
    setUpThermalTab();
    setUpTransmittanceTab();
}

GlassDataSheetForm::~GlassDataSheetForm()
{
    delete ui;
}

void GlassDataSheetForm::addItem(int row, int col, QString str, QGridLayout* gridLayout)
{
    QWidget* scrollAreaContents = gridLayout->parentWidget();

    QLabel* label = new QLabel(str, scrollAreaContents);
    gridLayout->addWidget(label, row, col, 1, 1);

    label = nullptr;
}


void GlassDataSheetForm::setUpBasicTab()
{
    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_Basic;
    QGridLayout* gridLayout = new QGridLayout(scrollAreaContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_Basic"));

    // name
    addItem(0, 0, "Name",              gridLayout);
    addItem(0, 1, m_glass->name(),     gridLayout);

    // catalog
    addItem(1, 0, "Catalog",           gridLayout);
    addItem(1, 1, m_glass->supplyer(), gridLayout);

    // MIL
    addItem(2, 0, "MIL",               gridLayout);
    addItem(2, 1, m_glass->MIL(),      gridLayout);

    // status
    addItem(3, 0, "Status",            gridLayout);
    addItem(3, 1, m_glass->status(),   gridLayout);

    // comment
    addItem(4, 0, "Comment",           gridLayout);
    addItem(4, 1, m_glass->comment(),  gridLayout);
}

void GlassDataSheetForm::setUpIndicesTab()
{
    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents;
    QGridLayout* gridLayout = new QGridLayout(scrollAreaContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_Indices"));

    // table header
    addItem(0, 0, "Spectral",   gridLayout);
    addItem(0, 1, "Wavelength", gridLayout);
    addItem(0, 2, "Index",      gridLayout);

    // list up indices
    QStringList spectralList = SpectralLine::spectralLineList();
    QString spectralLineName;

    int row;
    for(int i = 0; i < spectralList.size(); i++)
    {
        row = i + 1;

        // Spectral name
        spectralLineName = spectralList[i];
        addItem(row, 0, spectralLineName, gridLayout);

        // lambda(nm)
        addItem(row, 1, QString::number(SpectralLine::wavelength(spectralLineName), 'f', 3), gridLayout);

        // refractive index
        addItem(row, 2, QString::number(m_glass->index(spectralLineName), 'f', 6), gridLayout);
    }
}

void GlassDataSheetForm::setUpPartialTab()
{
    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_Partial;
    QGridLayout* gridLayout = new QGridLayout(scrollAreaContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_Partial"));

    QStringList firsts, seconds;

    int row;

    // Px,y
    firsts  = QStringList() << "s" << "C" << "d" << "e" << "g" << "i";
    seconds = QStringList() << "t" << "s" << "C" << "d" << "F" << "h";

    for(int i = 0; i < firsts.size(); i++){
        row = i;
        addItem(row, 0, "P " + firsts[i] + "," + seconds[i], gridLayout);
        addItem(row, 1, QString::number(m_glass->Pxy(firsts[i], seconds[i]), 'f', 6), gridLayout);
    }

    // P'x,y
    int rowOffset = gridLayout->rowCount();
    firsts  = QStringList() << "s" << "C_" << "d" << "e" << "g" << "i";
    seconds = QStringList() << "t" << "s" << "C_" << "d" << "F_" << "h";

    for(int j = 0; j < firsts.size(); j++)
    {
        row = j + rowOffset;

        addItem(row, 0, "P' " + firsts[j] + "," + seconds[j],                          gridLayout);
        addItem(row, 1, QString::number(m_glass->Pxy_(firsts[j], seconds[j]), 'f', 6), gridLayout);
    }

}


void GlassDataSheetForm::setUpDispersionTab()
{
    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_Dispersion;

    QGridLayout* gridLayout = new QGridLayout(scrollAreaContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_Dispersion"));

    int row;

    // formula name
    addItem(0, 0, tr("Formula"),          gridLayout);
    addItem(0, 1, m_glass->formulaName(), gridLayout);

    // list up coefficients
    for(int i = 0; i < m_glass->dispersionCoefCount();i++)
    {
        row = i + 1;

        addItem(row, 0, "C" + QString::number(i),                          gridLayout); // C0, C1, C2...
        addItem(row, 1, QString::number(m_glass->dispersionCoef(i),'g',5), gridLayout); // coefficient value
    }
}


void GlassDataSheetForm::setUpThermalTab()
{
    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_dndT;
    QGridLayout* gridLayout = new QGridLayout(scrollAreaContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_dndT"));

    QStringList coefNames = QStringList() << "D0" << "D1" << "D2" << "E0" << "E1" << "Ltk" << "T0";
    QList<double> coefValues = QList<double>() << m_glass->D0() << m_glass->D1() << m_glass->D2() << m_glass->E0() << m_glass->E1() << m_glass->Ltk() << m_glass->T0();

    if(m_glass->hasThermalData){
        for(int i = 0; i < coefNames.size(); i++){
            addItem(i, 0, coefNames[i],                   gridLayout);
            addItem(i, 1, QString::number(coefValues[i]), gridLayout);
        }
    }else{
        addItem(0, 0, tr("No Data"), gridLayout);
    }
}


void GlassDataSheetForm::setUpTransmittanceTab()
{
    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_Transmittance;
    QGridLayout* gridLayout = new QGridLayout(scrollAreaContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_Transmittance"));

    int digit = 4;
    int row;

    // set lambda max and min
    row = 0;
    addItem(row, 0, tr("Lambda Min/Max"),                           gridLayout);
    addItem(row, 1, QString::number(m_glass->lambdaMin(),'f',digit),gridLayout);
    addItem(row, 2, QString::number(m_glass->lambdaMax(),'f',digit),gridLayout);

    // insert space
    ++row;
    addItem(row, 0, "", gridLayout);

    // set header labels
    ++row;
    addItem(row, 0, tr("Wavelength(micron)"),gridLayout);
    addItem(row, 1, tr("Transmittance"),     gridLayout);
    addItem(row, 2, tr("Thickness"),         gridLayout);

    // set transmittance data
    QList<double> lambdamicrons, transmittances, thicknesses;
    m_glass->getTransmittanceData(lambdamicrons, transmittances, thicknesses);
    int ndata = lambdamicrons.size();

    int rowOffset = row+1;
    for(int i = 0; i < ndata; i++)
    {
        row = i + rowOffset;

        addItem(row,0,QString::number(lambdamicrons[i],  'f', digit),gridLayout);
        addItem(row,1,QString::number(transmittances[i], 'f', digit),gridLayout);
        addItem(row,2,QString::number(thicknesses[i],    'f', digit),gridLayout);
    }

}
