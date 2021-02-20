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

#include "glass.h"
#include "glass_catalog.h"
#include "glass_datasheet_form.h"

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
    QObject::connect(m_comboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(setUpTable()));

    QObject::connect(ui->pushButton_showDatasheet,SIGNAL(clicked()), this, SLOT(showDatasheet()));

    m_table = ui->tableWidget;
    setUpTable();
}

CatalogViewForm::~CatalogViewForm()
{
    m_table->clear();
    m_catalogList.clear();

    delete ui;
}

void CatalogViewForm::addTableItem(int row, int col, QString str)
{
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setText(str);
    m_table->setItem(row,col,item);
}

void CatalogViewForm::setUpTable()
{
    GlassCatalog* catalog = m_catalogList[ m_comboBox->currentIndex() ];

    QStringList headerLabels = QStringList() << tr("Glass") << tr("Nd" ) << tr("Vd") << tr("Ne") << tr("Ve") << tr("PgF") << tr("Status") << tr("Formula") << tr("Comment");

    int rowCount    = catalog->glassCount();
    int columnCount = headerLabels.size();
    m_table->setSortingEnabled(false);
    m_table->clear();
    m_table->setColumnCount(columnCount);
    m_table->setRowCount(rowCount);
    m_table->setHorizontalHeaderLabels(headerLabels);

    Glass* glass;
    int j;
    int digit = 6;

    for(int i = 0; i < rowCount; i++)
    {
        glass = catalog->glass(i);
        j = 0;

        // name
        addTableItem(i,j,glass->name());

        // refractive index
        if( tr("Unknown") == glass->formulaName() )
        {
            addTableItem(i,++j,"-"); // nd
            addTableItem(i,++j,"-"); // vd
            addTableItem(i,++j,"-"); // ne
            addTableItem(i,++j,"-"); // ve
            addTableItem(i,++j,"-"); // PgF
        }
        else
        {
            addTableItem(i,++j,QString::number(glass->nd(),  'f', digit));
            addTableItem(i,++j,QString::number(glass->vd(),  'f', digit));
            addTableItem(i,++j,QString::number(glass->ne(),  'f', digit));
            addTableItem(i,++j,QString::number(glass->ve(),  'f', digit));
            addTableItem(i,++j,QString::number(glass->PgF(), 'f', digit));
        }

        // status
        addTableItem(i,++j,glass->status());

        //formula
        addTableItem(i,++j,glass->formulaName());

        // individual comment
        addTableItem(i,++j,glass->comment());
    }

    glass = nullptr;
    catalog = nullptr;

    m_table->setSortingEnabled(true);
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
