#ifndef PRESET_DIALOG_H
#define PRESET_DIALOG_H

#include <QDialog>
#include <QTextCodec>
#include <QListWidget>
#include <QSettings>
#include <QInputDialog>

namespace Ui {
class PresetDialog;
}

class PresetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PresetDialog(QSettings* settings, QList<double> currentCoefs, QWidget *parent = nullptr);
    ~PresetDialog();

    QList<double> getCoefs();

private slots:
    void loadPreset();
    void saveAsNew();
    void deletePreset();

private:
    Ui::PresetDialog *ui;

    QSettings* m_settings;
    QListWidget* m_list;

    QList<double> m_coefs;

    void updateList();

};

#endif // PRESET_DIALOG_H
