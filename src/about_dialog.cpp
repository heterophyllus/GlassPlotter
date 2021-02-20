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

#include "about_dialog.h"
#include "ui_about_dialog.h"

#include <QGridLayout>
#include <QLabel>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("About");

    QLabel *label = new QLabel(this);
    label->setObjectName(QString::fromUtf8("label"));
    label->setText(QApplication::translate("AboutDlg",
    "GlassPlotter - Simple Glassmap Viewer\n"
    "\n"
    "    Copyright (C) <2020>  Hiiragi<heterophyllus.work@gmail.com>\n"
    "\n"
    "    This program is free software: you can redistribute it and/or modify \n"
    "    it under the terms of the GNU General Public License as published by\n"
    "    the Free Software Foundation, either version 3 of the License, or\n"
    "    any later version.\n"
    "\n"
    "    This program is distributed in the hope that it will be useful,\n"
    "    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "    GNU General Public License for more details.\n"
    "\n"
    "    You should have received a copy of the GNU General Public License\n"
    "    along with this program.  If not, see <https://www.gnu.org/licenses/>.\n"
    "\n"
    "    Additional information is provided at:\n"
    "    https://github.com/heterophyllus/glassplotter", nullptr));


    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->addWidget(label, 0, 0, 1, 1);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
