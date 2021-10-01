#include "catalog_view_setting_dialog.h"
#include "ui_catalog_view_setting_dialog.h"

#include <QListWidget>

CatalogViewSettingDialog::CatalogViewSettingDialog(QStringList allPropertyList, QStringList currentCheckedList, int currentDigit, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CatalogViewSettingDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Catalog Setting");

    ui->label_Description->setText(QApplication::translate("CatalogViewSettingDialog",
    "Check properties to add the table.\n"
    "Drag a property to arrange the order.", nullptr));

    m_listWidget = ui->listWidget_PropertyNames;

    for(int i = 0; i < allPropertyList.size(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(m_listWidget);
        item->setText(allPropertyList[i]);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

        if(currentCheckedList.contains(item->text())){
            item->setCheckState(Qt::Checked);
        }
        else{
            item->setCheckState(Qt::Unchecked);
        }
        m_listWidget->addItem(item);
    }

    // draggable
    m_listWidget->setAcceptDrops( true );
    m_listWidget->setDragEnabled( true );
    m_listWidget->setDragDropMode( QAbstractItemView::DragDropMode::InternalMove );
    m_listWidget->update();

    // spinbox
    ui->spinBox_Digit->setValue(currentDigit);
}

CatalogViewSettingDialog::~CatalogViewSettingDialog()
{
    m_listWidget->clear();
    delete ui;
}


void CatalogViewSettingDialog::getSettings(QStringList& checkedList, int& digit)
{
    // properties to be listed
    checkedList.clear();

    QListWidgetItem *item;
    for(int i = 0; i < m_listWidget->count(); i++)
    {
        item = m_listWidget->item(i);
        if(item->checkState()){
            checkedList.append(item->text());
        }
    }

    // digit
    digit = ui->spinBox_Digit->value();
}
