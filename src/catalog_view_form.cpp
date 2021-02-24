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
 **  Date    : 2020-5-20                                                    **
 *****************************************************************************/

#include "catalog_view_form.h"
#include "ui_catalog_view_form.h"

#include <QMdiArea>
#include <QComboBox>
#include <QTableWidget>
#include <QListWidget>
#include <QDialog>
#include <QDebug>

#include "glass.h"
#include "glass_catalog.h"
#include "glass_datasheet_form.h"
#include "catalog_view_setting_dialog.h"

CatalogViewForm::CatalogViewForm(QList<GlassCatalog*> catalogList, QMdiArea *parent) :
    QWidget(parent),
    ui(new Ui::CatalogViewForm)
{
    ui->setupUi(this);
    this->setWindowTitle("Catalog");

    m_parentMdiArea = parent;

    m_catalogList = catalogList;

    m_comboBox = ui->comboBox_Supplyer;
    for(int i = 0; i < m_catalogList.size(); i++){
        m_comboBox->addItem(m_catalogList.at(i)->supplyer());
    }
    QObject::connect(m_comboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(update()));

    QObject::connect(ui->pushButton_showDatasheet,SIGNAL(clicked()), this, SLOT(showDatasheet()));
    QObject::connect(ui->pushButton_Setting,SIGNAL(clicked()), this, SLOT(showSettingDlg()));

    m_table = ui->tableWidget;

    m_allPropertyList.clear();
    m_allPropertyList.reserve(29);
    m_allPropertyList.append("status");
    m_allPropertyList.append("individual comment");
    m_allPropertyList.append("MIL");
    m_allPropertyList.append("nd");
    m_allPropertyList.append("ne");
    m_allPropertyList.append("vd");
    m_allPropertyList.append("ve");
    m_allPropertyList.append("PgF");
    m_allPropertyList.append("Dispersion Formula");
    m_allPropertyList.append("Dispersion Coefficients");
    m_allPropertyList.append("Thermal Coefficients");
    m_allPropertyList.append("Low TCE");
    m_allPropertyList.append("High TCE");
    m_allPropertyList.append("Relative Cost");
    m_allPropertyList.append("Climate Resist");
    m_allPropertyList.append("Acid Resist");
    m_allPropertyList.append("Alkali Resist");
    m_allPropertyList.append("Phosphate Resist");


    // default table
    m_currentPropertyList << "nd" << "ne" << "vd" << "ve" << "PgF";
    m_currentDigit = 5;
    this->update();
}

CatalogViewForm::~CatalogViewForm()
{
    m_table->clear();
    m_catalogList.clear();

    delete ui;
}

void CatalogViewForm::update()
{
    setUpTable(m_currentPropertyList, m_catalogList[ m_comboBox->currentIndex() ], m_currentDigit);
}

void CatalogViewForm::addTableItem(int row, int col, QString str)
{
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setText(str);
    m_table->setItem(row,col,item);
}


void CatalogViewForm::setUpTable(QStringList properties, GlassCatalog* catalog, int digit)
{

    /***************************************
     *
     * set table row, col, header labels
     *
     ***************************************/
    QStringList headerLabels;
    headerLabels.append("name");
    for(int j = 0; j < properties.size(); j++)
    {
        if("status" == properties[j]){
            headerLabels.append("status");
        }
        else if("individual comment" == properties[j]){
            headerLabels.append("Individual Comment");
        }
        else if("MIL" == properties[j]){
            headerLabels.append("MIL");
        }
        else if("nd" == properties[j]){
            headerLabels.append("nd");
        }
        else if("ne" == properties[j]){
            headerLabels.append("ne");
        }
        else if("vd" == properties[j]){
            headerLabels.append("vd");
        }
        else if("ve" == properties[j]){
            headerLabels.append("ve");
        }
        else if("PgF" == properties[j]){
            headerLabels.append("PgF");
        }
        else if("Dispersion Formula" == properties[j]){
            headerLabels.append("Dispersion Formula");
        }
        else if("Dispersion Coefficients" == properties[j]){
            for (int ci = 0; ci<12 ;ci++ ) {
                headerLabels.append("C" + QString::number(ci));
            }
        }
        else if("Thermal Coefficients" == properties[j]){
            headerLabels.append("D0");
            headerLabels.append("D1");
            headerLabels.append("E0");
            headerLabels.append("E1");
            headerLabels.append("Ltk");
            headerLabels.append("T0");
        }
        else if("Low TCE" == properties[j]){
            headerLabels.append("Low TCE");
        }
        else if("High TCE" == properties[j]){
            headerLabels.append("High TCE");
        }
        else if("Relative Cost" == properties[j]){
            headerLabels.append("Relative Cost");
        }
        else if("Climate Resist" == properties[j]){
            headerLabels.append("Climate Resist");
        }
        else if("Acid Resist" == properties[j]){
            headerLabels.append("Acid Resist");
        }
        else if("Alkali Resist" == properties[j]){
            headerLabels.append("Alkali Resist");
        }
        else if("Phosphate Resist" == properties[j]){
            headerLabels.append("Phosphate Resist");
        }
    }

    int rowCount    = catalog->glassCount();
    int columnCount = headerLabels.size();
    m_table->clear();
    m_table->setSortingEnabled(false);
    m_table->setColumnCount(columnCount);
    m_table->setRowCount(rowCount);
    m_table->setHorizontalHeaderLabels(headerLabels);


    /*********************************
     *
     * fill in glass properties
     *
     *********************************/
    Glass* glass;
    int row, col;
    for(int i = 0; i < rowCount; i++)
    {
        row = i;
        glass = catalog->glass(i);

        // glass name should be at the first column.
        addTableItem(i,0,glass->name());

        // properties
        col = 1;
        for(int j = 0; j < properties.size(); j++)
        {
            if("status" == properties[j]){
                addTableItem(row,col,glass->status());
            }
            else if("individual comment" == properties[j]){
                addTableItem(row,col,glass->comment());
            }
            else if("MIL" == properties[j]){
                addTableItem(row,col,glass->MIL());
            }
            else if("nd" == properties[j]){
                addTableItem(row,col,numToQString(glass->nd(), 'f', digit));
            }
            else if("ne" == properties[j]){
                addTableItem(row,col,numToQString(glass->ne(), 'f', digit));
            }
            else if("vd" == properties[j]){
                addTableItem(row,col,numToQString(glass->vd(), 'f', digit));
            }
            else if("ve" == properties[j]){
                addTableItem(row,col,numToQString(glass->ve(), 'f', digit));
            }
            else if("PgF" == properties[j]){
                addTableItem(row,col,numToQString(glass->PgF(), 'f', digit));
            }
            else if("Dispersion Formula" == properties[j]){
                addTableItem(row,col,glass->formulaName());
            }
            else if("Dispersion Coefficients" == properties[j]){
                for (int ci = 0; ci<12 ;ci++ ) {
                    addTableItem(row,col,numToQString(glass->dispersionCoef(ci), 'e', digit));
                    col++;
                }
                col--;
            }
            else if("Thermal Coefficients" == properties[j]){
                addTableItem(row, col,   numToQString(glass->D0(),'g',digit));
                addTableItem(row, ++col, numToQString(glass->D1(),'g',digit));
                addTableItem(row, ++col, numToQString(glass->E0(),'g',digit));
                addTableItem(row, ++col, numToQString(glass->E1(),'g',digit));
                addTableItem(row, ++col, numToQString(glass->Ltk(),'g',digit));
                addTableItem(row, ++col, numToQString(glass->T0(),'f',digit));
            }
            else if("Low TCE" == properties[j]){
                addTableItem(row, col, numToQString(glass->lowTCE(), 'f', digit));
            }
            else if("High TCE" == properties[j]){
                addTableItem(row, col, numToQString(glass->highTCE(), 'f', digit));
            }
            else if("Relative Cost" == properties[j]){
                addTableItem(row, col, numToQString(glass->relCost(), 'f', digit));
            }
            else if("Climate Resist" == properties[j]){
                addTableItem(row, col, numToQString(glass->climateResist(), 'f', digit));
            }
            else if("Acid Resist" == properties[j]){
                addTableItem(row, col, numToQString(glass->acidResist(), 'f', digit));
            }
            else if("Alkali Resist" == properties[j]){
                addTableItem(row, col, numToQString(glass->alkaliResist(), 'f', digit));
            }
            else if("Phosphate Resist" == properties[j]){
                addTableItem(row, col, numToQString(glass->phosphateResist(), 'f', digit));
            }
            col++;
        }
    }

    m_table->update();
}

void CatalogViewForm::showDatasheet()
{
    // get catalog name and glass name of the current row
    QString supplyername = m_comboBox->currentText();
    QString glassname    = m_table->item(m_table->currentRow(),0)->text();

    // show glass datasheet form
    for(auto &cat: m_catalogList)
    {
        if(cat->supplyer() == supplyername)
        {
            GlassDataSheetForm* subwindow = new GlassDataSheetForm(cat->glass(glassname), m_parentMdiArea);
            m_parentMdiArea->addSubWindow(subwindow);
            subwindow->parentWidget()->setGeometry(0,10, this->width()*1/2,this->height()*3/4);
            subwindow->show();

            break;
        }
    }
}

void CatalogViewForm::showSettingDlg()
{
    CatalogViewSettingDialog *dlg = new CatalogViewSettingDialog(m_allPropertyList, this);

    if(dlg->exec() == QDialog::Accepted){
        dlg->getSettings(m_currentPropertyList, m_currentDigit);
        update();
    }

    try {
        delete dlg;
    }  catch (...) {
        dlg = nullptr;
    }

}
