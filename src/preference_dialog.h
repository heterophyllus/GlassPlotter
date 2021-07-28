#ifndef PREFERENCE_DIALOG_H
#define PREFERENCE_DIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class PreferenceDialog;
}

class PreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferenceDialog(QSettings *settings, QWidget *parent = nullptr);
    ~PreferenceDialog();

private slots:
    void browseDirectory();
    void onAccept();

private:
    void syncUiWithSettings();

    Ui::PreferenceDialog *ui;
    QSettings *m_settings;
};

#endif // PREFERENCE_DIALOG_H
