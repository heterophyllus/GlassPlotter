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

#include "glass_search_form.h"
#include "ui_glass_search_form.h"

#include <QMdiArea>
#include <QComboBox>
#include <QTableWidget>

#include "glass.h"
#include "glass_catalog.h"

GlassSearchForm::GlassSearchForm(QList<GlassCatalog*> catalogList, QMdiArea *parent) :
    QWidget(parent),
    ui(new Ui::GlassSearchForm),
    m_parentMdiArea(parent),
    m_catalogList(catalogList)
{
    ui->setupUi(this);
    this->setWindowTitle("Glass Search");

    ui->lineEdit_OutputCount->setValidator(new QIntValidator(0,100));
    ui->lineEdit_OutputCount->setText("5");

    // initialize parameters table
    QStringList hHeaderLabels({"Item"," Target", "Weight"});
    ui->tableWidget_Parameters->setColumnCount(hHeaderLabels.size());
    ui->tableWidget_Parameters->setHorizontalHeaderLabels(hHeaderLabels);
    addParameter();
    ui->tableWidget_Parameters->resizeColumnsToContents();

    QObject::connect(ui->pushButton_Add,    SIGNAL(clicked()), this, SLOT(addParameter()));
    QObject::connect(ui->pushButton_Remove, SIGNAL(clicked()), this, SLOT(removeParameter()));

    QObject::connect(ui->pushButton_Search, SIGNAL(clicked()), this, SLOT(showSearchResult()));
}

GlassSearchForm::~GlassSearchForm()
{
    m_catalogList.clear();
    delete ui;
}

void GlassSearchForm::addParameter()
{
    QTableWidget *table = ui->tableWidget_Parameters;
    int newRowCount = table->rowCount() + 1;
    int currentRow = newRowCount -1;
    table->setRowCount(newRowCount);

    QComboBox* combo = createParameterCombo();
    table->setCellWidget(currentRow, 0, combo);

    setCellValue(table, currentRow, 1, "1.0"); // target
    setCellValue(table, currentRow, 2, "1.0"); // weight

    table->update();
}

void GlassSearchForm::removeParameter()
{
    if(ui->tableWidget_Parameters->rowCount() > 0) {
        int currentRow = ui->tableWidget_Parameters->currentRow();
        ui->tableWidget_Parameters->removeRow(currentRow);
    }
}

void GlassSearchForm::showSearchResult()
{
    // search glass
    int resultCount = ui->lineEdit_OutputCount->text().toInt();
    QList<Glass*> results;
    QList<Glass*> allGlasses;

    for(auto &cat : m_catalogList) {
        int glassCount = cat->glassCount();
        for(int gi = 0; gi < glassCount; gi++) {
            allGlasses.append(cat->glass(gi));
        }
    }

    for(auto &g : allGlasses) {

        double e = getErrorValue(g);

        if(results.empty()) {
            results.append(g);
        }else {
            for(int i = 0; i < results.size(); i++) {
                if(e < getErrorValue(results[i])) {
                    results.insert(i,g);
                    if(results.size() > resultCount) {
                        results.pop_back();
                    }
                    break;
                }
            }
        }

    }

    //setup result table
    QStringList hHeaderLabels({"Glass", "Catalog"});
    for(int i = 0; i < ui->tableWidget_Parameters->rowCount(); i++) {
        QComboBox* combo = dynamic_cast<QComboBox*>(ui->tableWidget_Parameters->cellWidget(i,0));
        hHeaderLabels.append(combo->currentText());
    }
    ui->tableWidget_Result->setColumnCount(hHeaderLabels.size());
    ui->tableWidget_Result->setHorizontalHeaderLabels(hHeaderLabels);
    ui->tableWidget_Result->setRowCount(resultCount);
    for(int i = 0; i < resultCount; i++) {
        Glass* g = results[i];
        setCellValue(ui->tableWidget_Result, i, 0, g->name());
        setCellValue(ui->tableWidget_Result, i, 1, g->supplyer());

        for(int j = 0; j < ui->tableWidget_Parameters->rowCount(); j++) {
            QString param = dynamic_cast<QComboBox*>(ui->tableWidget_Parameters->cellWidget(j, 0))->currentText();
            setCellValue(ui->tableWidget_Result, i, j+2, numToQString(g->getValue(param)));
        }

    }


}

void GlassSearchForm::setCellValue(QTableWidget* table, int row, int col, QString str)
{
    QTableWidgetItem *item = table->item(row, col);
        if(!item){
            item = new QTableWidgetItem;
            table->setItem(row, col, item);
        }
        item->setTextAlignment(Qt::AlignRight);
        item->setText(str);
}


bool GlassSearchForm::compareErrorValue(Glass* g1, Glass* g2)
{
    return ( getErrorValue(g1) < getErrorValue(g2) );
}

double GlassSearchForm::getErrorValue(Glass* g)
{
    double e = 0.0;

    int parameterCount = ui->tableWidget_Parameters->rowCount();

    for(int i = 0; i < parameterCount; i++) {

        double pt = ui->tableWidget_Parameters->item(i, 1)->text().toDouble();
        double w = ui->tableWidget_Parameters->item(i, 2)->text().toDouble();

        QString paramName = dynamic_cast<QComboBox*>(ui->tableWidget_Parameters->cellWidget(i,0))->currentText();
        double p = g->getValue(paramName);

        e += w*pow(p-pt,2);
    }

    return e;
}

QComboBox* GlassSearchForm::createParameterCombo()
{
    QStringList items({"nd", "ne", "vd", "ve", "PgF"});
    QComboBox *combo = new QComboBox();
    combo->addItems(items);
    combo->setCurrentIndex(0);

    return combo;
}
