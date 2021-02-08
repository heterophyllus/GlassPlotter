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

#include "glass.h"
#include "glass_catalog.h"
#include "glass_selection_dialog.h"

using namespace Eigen;

CurveFittingDialog::CurveFittingDialog(QList<GlassCatalog*> catalogList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurveFittingDialog)
{
    ui->setupUi(this);

    m_catalogList = catalogList;

    m_listWidget = ui->listWidget_Glasses;

    m_comboBoxOrder = ui->comboBox_Order;
    m_comboBoxOrder->clear();
    for (int i = 1; i <= MAX_FITTING_ORDER;i++){
        m_comboBoxOrder->addItem(QString::number(i));
    }

    QObject::connect(ui->pushButton_AddGlass,    SIGNAL(clicked()), this, SLOT(addGlass()));
    QObject::connect(ui->pushButton_DeleteGlass, SIGNAL(clicked()), this, SLOT(deleteSelectedGlass()));

    m_fittingResult = {0,0,0,0}; // order: 0,1,2,3
}

CurveFittingDialog::~CurveFittingDialog()
{
    m_fittingResult.clear();
    m_catalogList.clear();
    m_targetGlassList.clear();

    delete ui;
}

void CurveFittingDialog::updateGlassList()
{
    m_targetGlassList.clear();
    QString glassname, supplyername, tempname;
    Glass* glass;
    for(int i = 0;i < m_listWidget->count() ;i++)
    {
        tempname = m_listWidget->item(i)->text();
        glassname = tempname.split("_")[0];
        supplyername = tempname.split("_")[1];

        for(auto cat: m_catalogList)
        {
            if(cat->supplyer() == supplyername){
                glass = cat->glass(glassname);
                m_targetGlassList.append(glass);
            }
        }

    }
}

void CurveFittingDialog::addGlass()
{
    GlassSelectionDialog* dlg = new GlassSelectionDialog(m_catalogList,this);
    if(dlg->exec() == QDialog::Accepted)
    {
        QString catalogName = m_catalogList[dlg->getCatalogIndex()]->supplyer();
        QString glassName = dlg->getGlassName();

        m_listWidget->addItem(glassName + "_" + catalogName);
    }
}

void CurveFittingDialog::deleteSelectedGlass()
{
    if(m_listWidget->selectedItems().size() > 0){
        delete m_listWidget->currentItem();
        m_listWidget->update();
    }
}

bool CurveFittingDialog::calculateFitting(QString xdataname, QString ydataname)
{
    //https://en.wikipedia.org/wiki/Polynomial_regression

    updateGlassList();

    int N = m_comboBoxOrder->currentIndex() + 1; //order
    int M = m_targetGlassList.size(); //samples

    if(m_targetGlassList.isEmpty()){
        QMessageBox::warning(this,tr("File"), tr("No glass has been selected"));
        return false;
    }else if( M <= N ){
        QMessageBox::warning(this,tr("File"), tr("Too few samples. Curve will be weird."));
    }

    // sampling points
    MatrixXd X = MatrixXd::Zero(M, N+1);
    VectorXd y(M);

    for(int i = 0; i < M; i++)
    {
        for(int j = 0; j < N+1; j++){
            X(i,j) = pow(m_targetGlassList[i]->getValue(xdataname), j);
        }
        y(i) = m_targetGlassList[i]->getValue(ydataname);
    }

    MatrixXd A = X.transpose()*X;
    VectorXd b = (X.transpose())*y;
    VectorXd beta = A.bdcSvd(ComputeThinU | ComputeThinV).solve(b);
    //VectorXd beta = (X.transpose()*X).inverse()*(X.transpose())*y;

    m_fittingResult = {0.0, 0.0, 0.0, 0.0};
    for(int k = 0; k <= N; k++)
    {
        m_fittingResult[k] = beta(k);
    }

    return true;
}

QList<double> CurveFittingDialog::fittingResult()
{
    return m_fittingResult;
}
