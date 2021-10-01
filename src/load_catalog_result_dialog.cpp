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

#include "load_catalog_result_dialog.h"
#include "ui_load_catalog_result_dialog.h"


LoadCatalogResultDialog::LoadCatalogResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadCatalogResultDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("File Loading Result");

    connect(ui->pushButton_OK, SIGNAL(clicked()), this, SLOT(accept()));
}

LoadCatalogResultDialog::~LoadCatalogResultDialog()
{
    delete ui;
}

void LoadCatalogResultDialog::setLabel(QString labeltext)
{
    ui->label->setText(labeltext);
}

void LoadCatalogResultDialog::setText(QString text)
{
    ui->textEdit_Result->setText(text);
}

