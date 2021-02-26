#include "load_catalog_result_dialog.h"
#include "ui_load_catalog_result_dialog.h"

#include <QTextEdit>

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

