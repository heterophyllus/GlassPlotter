#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
//#include <QtWidgets>

#include "qcustomplot.h"
#include "glasscatalog.h"
#include "glassmapmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createTableWidget();

private slots:
    void on_cellChanged(int catalogIndex, int PlotOrLabel);
    void on_comboChanged(int index);
    void on_checkBoxCurveChanged(bool checkState);
    void on_lineEdit_textEdited(QString linetext);
    void on_buttonResetViewClicked();
    void on_menu_File_LoadAGF_Triggered();
    void on_menu_Help_Abou_Triggered();
private:
    Ui::MainWindow *ui;

    QCustomPlot* _customPlot;
    GlassMapManager* _glassmapmanager;

    QString _AGFdir;

    static const int ColumnSupplyer = 0;
    static const int ColumnColor    = 1;
    static const int ColumnPlot     = 2;
    static const int ColumnLabel    = 3;

};
#endif // MAINWINDOW_H
