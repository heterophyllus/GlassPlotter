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

#include "glass_selection_dialog.h"
#include "ui_glass_selection_dialog.h"

#include <QSortFilterProxyModel>

GlassSelectionDialog::GlassSelectionDialog(const QList<GlassCatalog*> *catalogListPtr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GlassSelectionDialog)
{
    if(catalogListPtr->empty()) return;

    ui->setupUi(this);

    this->setWindowTitle("Select Glass");

    m_catalogListPtr = catalogListPtr;
    m_comboBoxSupplyer = ui->comboBox_Supplyer;
    m_lineEditFilter   = ui->lineEdit_Filter;
    m_listWidgetGlass  = ui->listWidget_Glass;

    for(int i = 0; i < m_catalogListPtr->size(); i++){
        m_comboBoxSupplyer->addItem(m_catalogListPtr->at(i)->supplyer());
    }

    QObject::connect(m_comboBoxSupplyer,SIGNAL(currentIndexChanged(int)), this, SLOT(onComboChanged()));

    QObject::connect(m_lineEditFilter,SIGNAL(textEdited(QString)), this, SLOT(updateGlassList()));

    createGlassNameList();
    updateGlassList();
}

GlassSelectionDialog::~GlassSelectionDialog()
{
    //m_catalogList.clear();
    delete ui;
}

void GlassSelectionDialog::onComboChanged()
{
    m_lineEditFilter->clear();
    createGlassNameList();
    updateGlassList();
}

void GlassSelectionDialog::createGlassNameList()
{
    m_glassNameList.clear();
    int catalogIndex = m_comboBoxSupplyer->currentIndex();
    GlassCatalog* catalog = m_catalogListPtr->at(catalogIndex);
    for(int i = 0; i < catalog->glassCount(); i++)
    {
        m_glassNameList.append(catalog->glass(i)->productName());
    }
    catalog = nullptr;
}

void GlassSelectionDialog::updateGlassList()
{
    m_listWidgetGlass->clear();

    QStringList glassNameList = m_glassNameList;
    QString filter = m_lineEditFilter->text();
    if(filter.length() > 0  ){
        glassNameList = glassNameList.filter( m_lineEditFilter->text(), Qt::CaseInsensitive );
    }

    m_listWidgetGlass->addItems(glassNameList);
    m_listWidgetGlass->setCurrentRow(0);  // avoid empty selection
}



int GlassSelectionDialog::getCatalogIndex()
{
    return m_comboBoxSupplyer->currentIndex();
}

QString GlassSelectionDialog::getSupplyerName()
{
    return m_comboBoxSupplyer->currentText();
}

QString GlassSelectionDialog::getGlassName()
{
    return m_listWidgetGlass->currentItem()->text();
}
