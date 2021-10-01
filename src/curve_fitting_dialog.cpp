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
 **  Date    : 2020-5-25                                                    **
 *****************************************************************************/

#include "curve_fitting_dialog.h"
#include "ui_curve_fitting_dialog.h"

#include <QMessageBox>
#include <QLineEdit>

#include "glass_selection_dialog.h"

#include "Eigen/Dense"

using namespace Eigen;

CurveFittingDialog::CurveFittingDialog(const QList<GlassCatalog*> *catalogListPtr, QString xdataname, QString ydataname, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurveFittingDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Curve Fitting");

    m_catalogListPtr = catalogListPtr;
    m_xDataName   = xdataname;
    m_yDataName   = ydataname;

    m_table = ui->tableWidget;
    m_table->clear();
    m_table->setColumnCount(3); //x,y,comment
    m_table->setHorizontalHeaderLabels(QStringList() << "X" << "Y" << "Comment");

    m_comboBoxOrder = ui->comboBox_Order;
    m_comboBoxOrder->clear();
    for (int i = 1; i <= m_maxFittingOrder;i++){
        m_comboBoxOrder->addItem(QString::number(i));
    }

    QObject::connect(ui->pushButton_AddRow,    SIGNAL(clicked()), this, SLOT(addNewRow()));
    QObject::connect(ui->pushButton_AddGlass,    SIGNAL(clicked()), this, SLOT(addGlassForNewRow()));
    QObject::connect(ui->pushButton_DeleteRow, SIGNAL(clicked()), this, SLOT(deleteSelectedRow()));

}

CurveFittingDialog::~CurveFittingDialog()
{
    //m_catalogList.clear();

    m_table->clear();

    delete ui;
}

/**
 * @brief Add new row to the table
 * @param s1 string value for X cell
 * @param s2 string value for Y cell
 * @param s3 string value for Comment cell
 */
void CurveFittingDialog::addNewRow(QString s1, QString s2, QString s3)
{
    int nRow = m_table->rowCount();
    m_table->insertRow(nRow);

    // X
    QTableWidgetItem* item;
    item = new QTableWidgetItem;
    item->setText(s1);
    m_table->setItem(nRow,0,item);

    // Y
    item = new QTableWidgetItem;
    item->setText(s2);
    m_table->setItem(nRow,1,item);

    // comment
    item = new QTableWidgetItem;
    item->setText(s3);
    m_table->setItem(nRow,2,item);

    m_table->update();
}


/**
 * @brief Add coordinates for new row by selecting glass
 */
void CurveFittingDialog::addGlassForNewRow()
{
    GlassSelectionDialog *dlg = new GlassSelectionDialog(m_catalogListPtr, this);
    if(dlg->exec() == QDialog::Accepted)
    {
        // get glass
        int catalogIndex  = dlg->getCatalogIndex();
        QString glassName = dlg->getGlassName();
        Glass* glass   = m_catalogListPtr->at(catalogIndex)->glass(glassName);

        // check dispersion formula of the glass
        if("Unknown" == glass->formulaName()){
            QMessageBox::warning(this,tr("Error"), "Could not get coordinates due to unknown dispersion formula");
        }
        // add glass property for new row
        else{
            QString s1 = QString::number(glass->getValue(m_xDataName));
            QString s2 = QString::number(glass->getValue(m_yDataName));
            QString s3 = glass->name() + "_" + glass->supplyer();
            addNewRow(s1, s2, s3);
        }
        glass = nullptr;
    }

    delete dlg;
    dlg = nullptr;

}

/**
 * @brief Delete currently selected row
 */
void CurveFittingDialog::deleteSelectedRow()
{
    int row = m_table->currentRow();

    m_table->removeRow(row);
    m_table->update();
}

/**
 * @brief Get fitting result using simple polynomial regression.  The result will be contained the parameter "result"
 * @param result
 * @return if true, successfully computed
 */
bool CurveFittingDialog::getFittingResult(QList<double>& result)
{
    //https://en.wikipedia.org/wiki/Polynomial_regression

    int N = m_comboBoxOrder->currentIndex() + 1; // order
    int M = m_table->rowCount();                 // number of samples

    // check
    if(M == 0){ // no input
        return false;
    }else{
        double d;
        bool ok;
        for(int i = 0; i < m_table->rowCount(); i++){
            for(int j = 0; j < 2; j++){
                d = m_table->item(i,j)->text().toDouble(&ok);
                if(!ok){ // invalid input
                    return false;
                }
            }
        }
    }

    // get coords of sampling points
    MatrixXd X = MatrixXd::Zero(M, N+1);
    VectorXd y(M);

    for(int i = 0; i < M; i++)
    {
        for(int j = 0; j < N+1; j++){
            X(i,j) = pow(m_table->item(i,0)->text().toDouble(), j);
        }
        y(i) = m_table->item(i,1)->text().toDouble();
    }

    MatrixXd A = X.transpose()*X;
    VectorXd b = (X.transpose())*y;
    VectorXd beta = A.bdcSvd(ComputeThinU | ComputeThinV).solve(b);
    //VectorXd beta = (X.transpose()*X).inverse()*(X.transpose())*y;

    // return fitting result
    result = {0.0, 0.0, 0.0, 0.0};
    for(int k = 0; k <= N; k++)
    {
        result[k] = beta(k);
    }

    return true;
}
