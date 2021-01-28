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


#include "glassdatasheetform.h"
#include "ui_glassdatasheetform.h"

GlassDataSheetForm::GlassDataSheetForm(Glass* glass, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GlassDataSheetForm)
{
    ui->setupUi(this);

    m_glass = glass;

    setWindowTitle("DataSheet - " + m_glass->name());

    // set names
    ui->label_GlassName->setText( m_glass->name() + " (" + m_glass->supplyer() + ")" );
    ui->label_Fundamental->setText( "nd= " + QString::number(m_glass->nd()) + "   Vd= " + QString::number(m_glass->vd()) );

    // The layout is created with reference to Schott's website
    setUpBasicTab();
    setUpIndicesTab();
    setUpPartialTab();
    setUpDispersionTab();
    setUpThermalTab();
}

GlassDataSheetForm::~GlassDataSheetForm()
{
    delete ui;
}

void GlassDataSheetForm::setUpBasicTab()
{
    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_Basic;
    QGridLayout* gridLayout = new QGridLayout(scrollAreaContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_Basic"));

    QLabel *label;

    // name
    label = new QLabel("Name", scrollAreaContents);
    gridLayout->addWidget(label, 0, 0, 1, 1);
    label = new QLabel(m_glass->name(), scrollAreaContents);
    gridLayout->addWidget(label, 0, 1, 1, 1);

    // Catalog
    label = new QLabel("Catalog", scrollAreaContents);
    gridLayout->addWidget(label, 1, 0, 1, 1);
    label = new QLabel(m_glass->supplyer(), scrollAreaContents);
    gridLayout->addWidget(label, 1, 1, 1, 1);

    // MIL
    label = new QLabel("MIL", scrollAreaContents);
    gridLayout->addWidget(label, 2, 0, 1, 1);
    label = new QLabel(m_glass->MIL(), scrollAreaContents);
    gridLayout->addWidget(label, 2, 1, 1, 1);

    // status
    label = new QLabel("Status", scrollAreaContents);
    gridLayout->addWidget(label, 3, 0, 1, 1);
    label = new QLabel(m_glass->status(), scrollAreaContents);
    gridLayout->addWidget(label, 3, 1, 1, 1);

    label = nullptr;

}

void GlassDataSheetForm::setUpIndicesTab()
{
    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents;
    QGridLayout* gridLayout = new QGridLayout(scrollAreaContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_Indices"));

    QLabel* label;

    // table header
    label = new QLabel("Spectral", scrollAreaContents);
    gridLayout->addWidget(label, 0, 0, 1, 1);

    label = new QLabel("Wavelength",scrollAreaContents);
    gridLayout->addWidget(label, 0, 1, 1, 1);

    label = new QLabel("Index", scrollAreaContents);
    gridLayout->addWidget(label, 0, 2, 1, 1);

    // list up indices
    QStringList spectralList = SpectralLine::spectralLineList();
    QString spectralLineName;
    double lambdamicron;
    double index;
    for(int i = 1; i < spectralList.size(); i++)
    {
        spectralLineName = spectralList.at(i-1);
        lambdamicron = SpectralLine::wavelength(spectralLineName);
        index = m_glass->index(spectralLineName);

        // Spectral name
        label = new QLabel(scrollAreaContents);
        label -> setText(spectralLineName);
        gridLayout->addWidget(label, i, 0, 1, 1);

        // lambda(nm)
        label = new QLabel(scrollAreaContents);
        label ->setText(QString::number(lambdamicron));
        gridLayout->addWidget(label, i, 1, 1, 1);

        // refractive index
        label = new QLabel(scrollAreaContents);
        label -> setText(QString::number(index));
        gridLayout->addWidget(label, i, 2, 1, 1);
    }
}

void GlassDataSheetForm::setUpPartialTab()
{
    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_Partial;
    QGridLayout* gridLayout = new QGridLayout(scrollAreaContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_Partial"));

    QLabel* label;
    QStringList firsts, seconds;

    // Px,y
    firsts  = QStringList() << "s" << "C" << "d" << "e" << "g" << "i";
    seconds = QStringList() << "t" << "s" << "C" << "d" << "F" << "h";
    for(int i = 0; i < firsts.size(); i++){
        label = new QLabel(scrollAreaContents);
        label->setText("P " + firsts[i] + "," + seconds[i]);
        gridLayout->addWidget(label, i, 0, 1, 1);

        label = new QLabel(scrollAreaContents);
        label->setText(QString::number(m_glass->Pxy(firsts[i], seconds[i])));
        gridLayout->addWidget(label, i, 1, 1, 1);
    }

    // P'x,y
    int rowOffset = gridLayout->rowCount();
    firsts  = QStringList() << "s" << "C_" << "d" << "e" << "g" << "i";
    seconds = QStringList() << "t" << "s" << "C_" << "d" << "F_" << "h";
    for(int j = 0; j < firsts.size(); j++){
        label = new QLabel(scrollAreaContents);
        label->setText("P' " + firsts[j] + "," + seconds[j]);
        gridLayout->addWidget(label, j+rowOffset, 0, 1, 1);

        label = new QLabel(scrollAreaContents);
        label->setText(QString::number(m_glass->Pxy_(firsts[j], seconds[j])));
        gridLayout->addWidget(label, j+rowOffset, 1, 1, 1);
    }

}


void GlassDataSheetForm::setUpDispersionTab()
{
    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_Dispersion;

    QGridLayout* gridLayout = new QGridLayout(scrollAreaContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_Dispersion"));

    QLabel *label;

    // formula name
    label = new QLabel(scrollAreaContents);
    label->setText("Formula");
    gridLayout->addWidget(label, 0, 0, 1, 1);

    label = new QLabel(scrollAreaContents);
    label->setText(m_glass->formulaName());
    gridLayout->addWidget(label, 0, 1, 1, 1);

    // list up coefficients
    for(int i = 1; i < m_glass->dispersionCoefCount();i++)
    {
        // C0, C1, C2...
        label = new QLabel(scrollAreaContents);
        label->setText("C" + QString::number(i-1));
        gridLayout->addWidget(label, i, 0, 1, 1);

        // coefficient value
        label = new QLabel(scrollAreaContents);
        label->setText(QString::number(m_glass->dispersionCoef(i-1)));
        gridLayout->addWidget(label, i, 1, 1, 1);
    }
}


void GlassDataSheetForm::setUpThermalTab()
{
    QWidget* scrollAreaContents = ui->scrollAreaWidgetContents_dndT;
    QGridLayout* gridLayout = new QGridLayout(scrollAreaContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout_dndT"));

    QStringList coefNames = QStringList() << "D0" << "D1" << "D2" << "E0" << "E1" << "Ltk";
    QList<double> coefValues = QList<double>() << m_glass->D0() << m_glass->D1() << m_glass->D2() << m_glass->E0() << m_glass->E1() << m_glass->Ltk();
    QLabel *label;

    if(m_glass->hasThermalData){
        for(int i = 0; i < coefNames.size(); i++){
            label = new QLabel(scrollAreaContents);
            label->setText(coefNames[i]);
            gridLayout->addWidget(label, i, 0, 1, 1);

            label = new QLabel(scrollAreaContents);
            label->setText(QString::number(coefValues[i]));
            gridLayout->addWidget(label, i, 1, 1, 1);
        }
    }else{
        label = new QLabel(scrollAreaContents);
        label->setText("No Data");
        gridLayout->addWidget(label, 0, 0, 1, 1);
    }
}
