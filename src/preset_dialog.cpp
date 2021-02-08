#include "preset_dialog.h"
#include "ui_presetdialog.h"

PresetDialog::PresetDialog(QSettings* settings, QList<double> currentCoefs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PresetDialog)
{
    ui->setupUi(this);

    m_settings = settings;
    m_coefs = currentCoefs;
    m_list = ui->listWidget;

    updateList();

    QObject::connect(ui->pushButton_LoadPreset,  SIGNAL(clicked()), this, SLOT(loadPreset()));
    QObject::connect(ui->pushButton_SaveAsNew,   SIGNAL(clicked()), this, SLOT(saveAsNew()));
    QObject::connect(ui->pushButton_DeletePreset,SIGNAL(clicked()), this, SLOT(deletePreset()));
}


PresetDialog::~PresetDialog()
{
    m_coefs.clear();
    delete ui;
}


QList<double> PresetDialog::getCoefs()
{
    return m_coefs;
}

void PresetDialog::updateList()
{
    m_list->clear();
    m_list->addItems(m_settings->childGroups());
}

void PresetDialog::loadPreset()
{
    if(m_list->selectedItems().size() > 0)
    {
        QString groupName = m_list->currentItem()->text();

        m_settings->beginGroup(groupName);
        m_coefs[0] = m_settings->value("C0",0.0).toDouble();
        m_coefs[1] = m_settings->value("C1",0.0).toDouble();
        m_coefs[2] = m_settings->value("C2",0.0).toDouble();
        m_coefs[3] = m_settings->value("C3",0.0).toDouble();
        m_settings->endGroup();

        this->accept();
    }
}

void PresetDialog::saveAsNew()
{
    bool ok;
    QString text = QInputDialog::getText(
        this,
        tr("Save As New"),
        tr("Input setting name"),
        QLineEdit::Normal,
        "preset",
        &ok
    );

    if(ok){
        m_settings->beginGroup(text);
        m_settings->setValue("C0", m_coefs[0]);
        m_settings->setValue("C1", m_coefs[1]);
        m_settings->setValue("C2", m_coefs[2]);
        m_settings->setValue("C3", m_coefs[3]);
        m_settings->endGroup();
        m_settings->sync();

        updateList();
        //this->accept();
    }
}

void PresetDialog::deletePreset()
{
    if(m_list->selectedItems().size() > 0)
    {
        QString groupname = m_list->currentItem()->text();
        m_settings->beginGroup(groupname);
        m_settings->remove("");
        m_settings->endGroup();
        m_settings->sync();

        updateList();
    }
}
