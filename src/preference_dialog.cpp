#include "preference_dialog.h"
#include "ui_preference_dialog.h"

#include <QFileDialog>

PreferenceDialog::PreferenceDialog(QSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);

    m_settings = settings;
    syncUiWithSettings();

    QObject::connect(ui->pushButton_Browse, SIGNAL(clicked()), this, SLOT(browseDirectory()));
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

PreferenceDialog::~PreferenceDialog()
{
    m_settings = nullptr;
    delete ui;
}

void PreferenceDialog::browseDirectory()
{
    QFileDialog::Options options =
          QFileDialog::ShowDirsOnly | QFileDialog::HideNameFilterDetails |
          QFileDialog::DontUseCustomDirectoryIcons;

    QString dirName = QFileDialog::getExistingDirectory(
        this,
        tr("Select directory"),
        QApplication::applicationDirPath(),
        options
    );

    if ( !dirName.isEmpty() ) {
        ui->lineEdit_CatalogDir->setText(dirName);
    }
}

void PreferenceDialog::syncUiWithSettings()
{
    m_settings->beginGroup("Preference");
    QString catalogDir  = m_settings->value("Directory", "").toString();
    QString catalogExt  = m_settings->value("Extension", "").toString();
    bool loadWithResult = m_settings->value("ShowResult", false).toBool();
    m_settings->endGroup();

    ui->lineEdit_CatalogDir->setText(catalogDir);
    ui->comboBox_Extension->setCurrentText(catalogExt);
    ui->checkBox_ParseResult->setChecked(loadWithResult);
}

void PreferenceDialog::onAccept()
{
    QString catalogDir = ui->lineEdit_CatalogDir->text();
    QString catalogExt = ui->comboBox_Extension->currentText();
    bool withResult = ui->checkBox_ParseResult->checkState();

    m_settings->beginGroup("Preference");
    m_settings->setValue("Directory", catalogDir);
    m_settings->setValue("Extension", catalogExt);
    m_settings->setValue("ShowResult", withResult);
    m_settings->endGroup();

    accept();
}
