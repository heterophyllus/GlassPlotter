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

#include "catalogviewform.h"
#include "ui_catalogviewform.h"

CatalogViewForm::CatalogViewForm(QList<GlassCatalog*> catalogList, QMdiArea *parent) :
    QWidget(parent),
    ui(new Ui::CatalogViewForm)
{
    ui->setupUi(this);

    m_catalogList = catalogList;
    m_comboBox = ui->comboBox_Supplyer;
    m_table = ui->tableWidget;

    for(int i = 0; i < m_catalogList.size(); i++){
        m_comboBox->addItem(m_catalogList.at(i)->supplyer());
    }

    QObject::connect(m_comboBox,SIGNAL(currentIndexChanged(int)),
                         this, SLOT(setUpTable()));

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

    QStringList headerLabels = QStringList() << tr("Glass") << tr("Nd" ) << tr("Vd") << tr("MIL") << tr("Status") << tr("Formula") << tr("C0") << tr("C1") << tr("C2") << tr("C3") <<tr("C4") << tr("C5") << tr("C6") << tr("C7") << tr("C8") << tr("C9") <<tr("C10") << tr("C11") << tr("Comment") ;

    m_table->setSortingEnabled(false);

    m_table->clear();
    m_table->setColumnCount( headerLabels.size() );
    m_table->setRowCount( catalog->glassCount() );
    m_table->setHorizontalHeaderLabels( headerLabels );

    Glass* glass;
    QTableWidgetItem* item;
    int j;
    for(int i = 0;i < catalog->glassCount();i++)
    {
        glass = catalog->glass(i);
        j = 0;

        // name
        item = new QTableWidgetItem(glass->name());
        m_table->setItem(i, j, item);

        // index
        item = new QTableWidgetItem(QString::number(glass->nd()));
        m_table->setItem(i, ++j, item);

        // Abbe
        item = new QTableWidgetItem(QString::number(glass->vd()));
        m_table->setItem(i, ++j, item);

        // MIL
        item = new QTableWidgetItem(glass->MIL());
        m_table->setItem(i, ++j, item);

        // status
        item = new QTableWidgetItem(glass->status());
        m_table->setItem(i, ++j, item);

        //formula
        item = new QTableWidgetItem(glass->dispersion()->formulaName());
        m_table->setItem(i, ++j, item);

        //coefficients
        j++;
        for(int k = 0;k<glass->dispersion()->coefs.size();k++)
        {
            item = new QTableWidgetItem();
            item->setText(QString::number(glass->dispersion()->coefs[k]));
            m_table->setItem(i, j++, item);
        }

        // individual comment
        item = new QTableWidgetItem();
        item->setText(glass->comment());
        m_table->setItem(i, j++, item);
    }
    glass = nullptr;
    item  = nullptr;
    catalog = nullptr;

    m_table->setSortingEnabled(true);
}
