#ifndef LOAD_CATALOG_RESULT_DIALOG_H
#define LOAD_CATALOG_RESULT_DIALOG_H

#include <QDialog>

namespace Ui {
class LoadCatalogResultDialog;
}

class LoadCatalogResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadCatalogResultDialog(QWidget *parent = nullptr);
    ~LoadCatalogResultDialog();

    void setLabel(QString labeltext);
    void setText(QString text);

private:
    Ui::LoadCatalogResultDialog *ui;
};

#endif // LOAD_CATALOG_RESULT_DIALOG_H
