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

void CatalogViewForm::setUpTable()
{
    GlassCatalog* catalog = m_catalogList[ m_comboBox->currentIndex() ];

    QStringList headerLabels = QStringList() << tr("Glass") << tr("Nd" ) << tr("Vd") << tr("Ne") << tr("Ve") << tr("PgF") << tr("Status") << tr("Formula") << tr("Comment");

    m_table->setSortingEnabled(false);
    m_table->clear();
    m_table->setColumnCount( headerLabels.size() );
    m_table->setRowCount( catalog->glassCount() );
    m_table->setHorizontalHeaderLabels( headerLabels );

    Glass* glass;
    QTableWidgetItem* item;
    int j;
    int digit = 6;

    for(int i = 0; i < catalog->glassCount(); i++)
    {
        glass = catalog->glass(i);
        j = 0;

        // name
        item = new QTableWidgetItem(glass->name());
        m_table->setItem(i, j, item);

        // refractive index
        if( tr("Unknown") == glass->formulaName() ){
            item = new QTableWidgetItem("-");
            m_table->setItem(i, ++j, item); // nd
            m_table->setItem(i, ++j, item); // vd
            m_table->setItem(i, ++j, item); // ne
            m_table->setItem(i, ++j, item); // ve
            m_table->setItem(i, ++j, item); // PgF
        }
        else{
            // nd
            item = new QTableWidgetItem(QString::number(glass->nd(), 'f', digit));
            m_table->setItem(i, ++j, item);

            // vd
            item = new QTableWidgetItem(QString::number(glass->vd(), 'f', digit));
            m_table->setItem(i, ++j, item);

            // ne
            item = new QTableWidgetItem(QString::number(glass->ne(), 'f', digit));
            m_table->setItem(i, ++j, item);

            // ve
            item = new QTableWidgetItem(QString::number(glass->ve(), 'f', digit));
            m_table->setItem(i, ++j, item);

            // PgF
            item = new QTableWidgetItem(QString::number(glass->PgF(), 'f', digit));
            m_table->setItem(i, ++j, item);
        }

        // status
        item = new QTableWidgetItem(glass->status());
        m_table->setItem(i, ++j, item);

        //formula
        item = new QTableWidgetItem(glass->formulaName());
        m_table->setItem(i, ++j, item);

        // individual comment
        item = new QTableWidgetItem();
        item->setText(glass->comment());
        m_table->setItem(i, ++j, item);
    }
    glass = nullptr;
    item  = nullptr;
    catalog = nullptr;

    m_table->setSortingEnabled(true);
}

void CatalogViewForm::showDatasheet()
{
    // get catalog name and glass name of the current row
    QString supplyername = m_comboBox->currentText();
    QString glassname = m_table->item(m_table->currentRow(),0)->text();

    Glass* glass = nullptr;
    for(int i = 0; i < m_catalogList.size(); i++)
    {
        if(m_catalogList[i]->supplyer() == supplyername){
            glass = m_catalogList[i]->glass(glassname);

            GlassDataSheetForm* subwindow = new GlassDataSheetForm(glass, m_parentMdiArea);
            m_parentMdiArea->addSubWindow(subwindow);
            subwindow->parentWidget()->setGeometry(0,10, this->width()*1/2,this->height()*3/4);
            subwindow->show();

            break;
        }
    }

    glass = nullptr;
}
