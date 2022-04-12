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


#include <QListWidget>
#include <QDialog>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

#include "glass_catalog_manager.h"
#include "glass_datasheet_form.h"
#include "catalog_view_setting_dialog.h"

CatalogViewForm::CatalogViewForm(QMdiArea *parent) :
    QWidget(parent),
    ui(new Ui::CatalogViewForm)
{
    ui->setupUi(this);
    this->setWindowTitle("Catalog");

    m_parentMdiArea = parent;

    m_comboBox = ui->comboBox_Supplyer;
    for(int i = 0; i < GlassCatalogManager::catalogList().size(); i++){
        m_comboBox->addItem(GlassCatalogManager::catalogList().at(i)->supplier());
    }
    QObject::connect(m_comboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(update()));

    QObject::connect(ui->pushButton_showDatasheet,SIGNAL(clicked()), this, SLOT(showDatasheet()));
    QObject::connect(ui->pushButton_Setting,SIGNAL(clicked()),       this, SLOT(showSettingDlg()));

    m_table = ui->tableWidget;
    m_table->setSortingEnabled(true);

    m_allPropertyList = QStringList({"nd",
                                     "ne",
                                     "vd",
                                     "ve",
                                     "PgF",
                                     "PCt_",
                                     "status",
                                     "individual comment",
                                     "MIL",
                                     "Dispersion Formula",
                                     "Dispersion Coefficients",
                                     "Thermal Coefficients",
                                     "Low TCE",
                                     "High TCE",
                                     "Relative Cost",
                                     "Climate Resist",
                                     "Acid Resist",
                                     "Alkali Resist",
                                     "Phosphate Resist"
                                    });

    // set up default table
    m_currentPropertyList << "nd" << "ne" << "vd" << "ve" << "PgF";
    m_currentDigit = 5;

    m_table->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_table, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenuOnTable()));

    this->update();
}

CatalogViewForm::~CatalogViewForm()
{
    m_table->clear();

    delete ui;
}

void CatalogViewForm::update()
{
    setUpTable(m_currentPropertyList, GlassCatalogManager::catalogList().at( m_comboBox->currentIndex() ), m_currentDigit);
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
        else if("PCt_" == properties[j]){
            headerLabels.append("PCt_");
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
        addTableItem(i,0,glass->productName());

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
                addTableItem(row,col,numToQString(glass->getValue("nd"), 'f', digit));
            }
            else if("ne" == properties[j]){
                addTableItem(row,col,numToQString(glass->getValue("ne"), 'f', digit));
            }
            else if("vd" == properties[j]){
                addTableItem(row,col,numToQString(glass->getValue("vd"), 'f', digit));
            }
            else if("ve" == properties[j]){
                addTableItem(row,col,numToQString(glass->getValue("ve"), 'f', digit));
            }
            else if("PgF" == properties[j]){
                addTableItem(row,col,numToQString(glass->getValue("PgF"), 'f', digit));
            }
            else if("PCt_" == properties[j]){
                addTableItem(row,col,numToQString(glass->getValue("PCt_"), 'f', digit));
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
                addTableItem(row, ++col, numToQString(glass->Tref(),'f',digit));
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

    m_table->setSortingEnabled(true);
    m_table->update();
}

void CatalogViewForm::showDatasheet()
{
    // get catalog name and glass name of the current row
    QString supplyername = m_comboBox->currentText();
    QString productname    = m_table->item(m_table->currentRow(),0)->text();
    QString fullname = productname + "_" + supplyername;

    // show glass datasheet form
    GlassDataSheetForm* subwindow = new GlassDataSheetForm(GlassCatalogManager::find(fullname), m_parentMdiArea);
    subwindow->setAttribute(Qt::WA_DeleteOnClose);
    m_parentMdiArea->addSubWindow(subwindow);
    subwindow->parentWidget()->setGeometry(0,10, this->width()*1/2,this->height()*3/4);
    subwindow->show();
}

void CatalogViewForm::showSettingDlg()
{
    CatalogViewSettingDialog *dlg = new CatalogViewSettingDialog(m_allPropertyList, m_currentPropertyList, m_currentDigit, this);

    if(dlg->exec() == QDialog::Accepted){
        dlg->getSettings(m_currentPropertyList, m_currentDigit);
        update();
    }

    delete dlg;
}

void CatalogViewForm::showContextMenuOnTable()
{
    QMenu contextMenu;
    QAction *action1 = contextMenu.addAction("Export Data");
    QObject::connect(action1, SIGNAL(triggered()), this, SLOT(exportCSV()));
    contextMenu.exec(QCursor::pos());
}

void CatalogViewForm::exportCSV()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save as"),"",tr("CSV file(*.csv);;All Files(*.*)"));
    if(filePath.isEmpty()){
        return;
    }

    if(m_table->exportCSV(filePath)){
        QMessageBox::information(this, "Success", "The data was successfully exported");
    }else{
        QMessageBox::warning(this, "Error", "Failed to export");
    }
}
