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

#include <algorithm>
#include <QShortcut>
#include <QApplication>
#include <QClipboard>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "qcustomtablewidget.h"

QCustomTableWidget::QCustomTableWidget(QWidget* parent) :
    QTableWidget(parent)
{
    QShortcut *scCtrlC = new QShortcut(QKeySequence("Ctrl+C"), this);
    QObject::connect(scCtrlC, SIGNAL(activated()), this, SLOT(copyCell()));
    QShortcut *scCtrlV = new QShortcut(QKeySequence("Ctrl+V"), this);
    QObject::connect(scCtrlV, SIGNAL(activated()), this, SLOT(pasteCell()));
}


void QCustomTableWidget::copyCell()
{
    //https://qiita.com/MOKYN/items/4b7c090af655efda0c42

    QAbstractItemModel* model = this->model();
    QItemSelectionModel* selection = this->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();

    if (indexes.count() == 0) {
            return;
    }

    //qSort(indexes); // デフォルトだと列ソートのため行でソートする
    std::sort(indexes.begin(), indexes.end());

    QString clip;
    QModelIndex previous = indexes.first();
    indexes.removeFirst();
    clip.append(model->data(previous).toString());

    for(QModelIndex& current : indexes.toVector())
    {
            if (current.row() != previous.row())
            {
                    clip.append("\n");
            }
            else
            {
                    clip.append("\t");
            }

            clip.append(model->data(current).toString());
            previous = current;
    }

    QApplication::clipboard()->setText(clip);
}

void QCustomTableWidget::pasteCell()
{
    if (this->selectedItems().size() != 1)
    {
            return;
    }
    QString clip = QApplication::clipboard()->text();
    QStringList rowList = clip.split("\n");

    QTableWidgetItem *currentItem = this->selectedItems().first();
    int pasteRow = currentItem->row();
    int pasteCol = currentItem->column();
    for(QString &row : rowList.toVector())
    {
        QStringList colList = row.split("\t");
        for (QString &cell : colList.toVector())
        {
                this->item(pasteRow, pasteCol)->setText(cell);
                pasteCol++;
        }
        pasteCol = currentItem->column();
        pasteRow++;
    }
}

bool QCustomTableWidget::exportCSV(const QString &filepath)
{
    QFile data(filepath);
    if (data.open(QFile::WriteOnly | QIODevice::Append)) {

    }

    QTextStream output(&data);

    const int nrows = this->rowCount();
    const int ncolumns = this->columnCount();

    // header labels
    for(int j = 0; j < ncolumns; j++){
        output << this->horizontalHeaderItem(j)->text();
        if( j < ncolumns - 1 ){
            output << ", ";
        }
    }
    output << "\n";

    for(int i = 0; i < nrows; i++){
        for(int j = 0; j < ncolumns; j++){
            QTableWidgetItem* item = this->item(i,j);
            if(item){
                output << this->item(i,j)->text();
            }else{
                output << NAN;
            }
            if( j < ncolumns - 1 ){
                output << ", ";
            }
        }
        output << "\n";
    }

    data.close();

    return true;
}
