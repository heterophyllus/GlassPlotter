#include "preference_dialog.h"
#include "ui_preference_dialog.h"

#include <QFileDialog>

PreferenceDialog::PreferenceDialog(QSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Preference");

    m_settings = settings;
    syncUiWithSettings();

    QObject::connect(ui->pushButton_Browse, SIGNAL(clicked()), this, SLOT(browseCatalogFiles()));
    QObject::connect(ui->pushButton_Clear,  SIGNAL(clicked()), this, SLOT(clearCatalogFiles()));
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

PreferenceDialog::~PreferenceDialog()
{
    m_settings = nullptr;
    delete ui;
}

void PreferenceDialog::browseCatalogFiles()
{
    ui->listWidget_DefaultFiels->clear();

    // open file selection dialog
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                                                          tr("select AGF"),
                                                          QApplication::applicationDirPath(),
                                                          tr("AGF files(*.agf);;XML Files(*.xml)"));
    if(!filePaths.empty()){
        ui->listWidget_DefaultFiels->addItems(filePaths);
    }

}

void PreferenceDialog::clearCatalogFiles()
{
    ui->listWidget_DefaultFiels->clear();
}

void PreferenceDialog::syncUiWithSettings()
{
    m_settings->beginGroup("Preference");

    // catalog file paths
    int catalogFilecount = m_settings->value("NumFiles", 0).toInt();
    if(catalogFilecount > 0) {
        for(int i = 0; i < catalogFilecount; i++) {
            QString catalogFilePath  = m_settings->value("File" + QString::number(i), "").toString();
            ui->listWidget_DefaultFiels->addItem(catalogFilePath);
        }
    }

    // on/off result dialog
    bool loadWithResult = m_settings->value("ShowResult", false).toBool();
    ui->checkBox_ParseResult->setChecked(loadWithResult);

    m_settings->endGroup();
}

void PreferenceDialog::onAccept()
{
    m_settings->beginGroup("Preference");

    // catalog file paths
    int catalogFileCount = ui->listWidget_DefaultFiels->count();
    m_settings->setValue("NumFiles", catalogFileCount);
    if( catalogFileCount > 0) {
        for(int i = 0; i < catalogFileCount; i++) {
            QString filePath = ui->listWidget_DefaultFiels->item(i)->text();
            m_settings->setValue("File" + QString::number(i), filePath);
        }
    }

    // on/off result dialog
    bool withResult = ui->checkBox_ParseResult->checkState();
    m_settings->setValue("ShowResult", withResult);

    m_settings->endGroup();
    m_settings->sync();

    accept();
}
