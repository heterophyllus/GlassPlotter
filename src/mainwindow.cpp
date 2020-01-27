#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set default value
    _AGFdir = QApplication::applicationDirPath() + "/AGF";

    //menu
    QObject::connect(ui->actionLoadAGFfolder,SIGNAL(triggered()),
                     this, SLOT(on_menu_File_LoadAGF_Triggered()));

    QObject::connect(ui->actionAbout, SIGNAL(triggered()),
                     this, SLOT(on_menu_Help_Abou_Triggered()));

    //custom plot
    _customPlot = ui->widget;
    _customPlot->setInteraction(QCP::iRangeDrag, true);
    _customPlot->setInteraction(QCP::iRangeZoom, true);
    _customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    _glassmapmanager = new GlassMapManager(_customPlot);

    //create comboBox
    ui->comboBox_plotType->clear();
    ui->comboBox_plotType->addItem("Nd-Vd");
    ui->comboBox_plotType->addItem("Ne-Ve");
    ui->comboBox_plotType->addItem("Vd-PgF");
    QObject::connect(ui->comboBox_plotType,SIGNAL(currentIndexChanged(int)),
                     this, SLOT(on_comboChanged(int)));

    //reset view button
    QObject::connect(ui->button_resetView,SIGNAL(clicked()),
                     this, SLOT(on_buttonResetViewClicked()));

    //curve
    ui->lineEdit_coef0->setText("7.278e-1");
    ui->lineEdit_coef1->setText("-5.656e-3");
    ui->lineEdit_coef2->setText("5.213e-5");
    ui->lineEdit_coef3->setText("-1.665e-7");
    QObject::connect(ui->groupBox_curve,SIGNAL(toggled(bool)),
                     this, SLOT(on_checkBoxCurveChanged(bool)));
    QObject::connect(ui->lineEdit_coef0,SIGNAL(textEdited(QString)),
                     this, SLOT(on_lineEdit_textEdited(QString)));
    QObject::connect(ui->lineEdit_coef1,SIGNAL(textEdited(QString)),
                     this, SLOT(on_lineEdit_textEdited(QString)));
    QObject::connect(ui->lineEdit_coef2,SIGNAL(textEdited(QString)),
                     this, SLOT(on_lineEdit_textEdited(QString)));
    QObject::connect(ui->lineEdit_coef3,SIGNAL(textEdited(QString)),
                     this, SLOT(on_lineEdit_textEdited(QString)));


    // read Zemax AGF in the folder
    if(!_glassmapmanager->readAllAGF(_AGFdir)){
        QMessageBox::information(this,tr("Error"), tr("AGF load error\n Check File->Load AGF folder"));
        //return;
    }
    _glassmapmanager->createGlassMapList(0);

    //create table widget
    createTableWidget();
    QObject::connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),
                     this, SLOT(on_cellChanged(int,int)));

    _glassmapmanager->resetAxis(0);
    _glassmapmanager->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createTableWidget()
{
    //disconnect to avoid updating
    QObject::disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),
                     this, SLOT(on_cellChanged(int,int)));

    QTableWidget *table = ui->tableWidget;
    table->clear();

    // set table format 
    table->setColumnCount( 4 );
    table->setRowCount( _glassmapmanager->catalogCount() );

    // set header
    table->setHorizontalHeaderLabels( QStringList() << tr("CATALOG") << tr("") << tr("P") << tr("L" )  );

    // set supplyers' names and checkboxes
    QString supplyername;
    for (int i = 0; i< _glassmapmanager->catalogCount() ; i++)
    {
        supplyername = _glassmapmanager->catalog(i)->supplyer();
        table->setItem( i, ColumnSupplyer, new QTableWidgetItem(supplyername) ); //supplyer
        table->item(i,ColumnSupplyer)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        table->setItem( i, ColumnColor, new QTableWidgetItem("")   ); // color
        table->item(i,ColumnColor)->setBackgroundColor(_glassmapmanager->getColor(supplyername));
        table->item(i,ColumnColor)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        table->setItem( i, ColumnPlot, new QTableWidgetItem("")   );                   //plot
        table->item(i,ColumnPlot)->setCheckState(Qt::Unchecked );
        table->setItem( i, ColumnLabel, new QTableWidgetItem("")   );                   //label
        table->item(i,ColumnLabel)->setCheckState(Qt::Unchecked );
    }


    table->setColumnWidth(ColumnColor,5);
    table->setColumnWidth(ColumnPlot,10);
    table->setColumnWidth(ColumnLabel,10);

    //re-connect
    QObject::connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),
                     this, SLOT(on_cellChanged(int,int)));


    // update visible state
    bool pointstate;
    bool labelstate;

    for(int i = 0 ;i<ui->tableWidget->rowCount();i++)
    {
        supplyername = _glassmapmanager->catalog(i)->supplyer();
        pointstate = ui->tableWidget->item(i,ColumnPlot)->checkState();
        labelstate = ui->tableWidget->item(i,ColumnLabel)->checkState();
        _glassmapmanager->setChartVisible(supplyername,pointstate,labelstate);
    }

    _glassmapmanager->replot();

}

void MainWindow::on_menu_File_LoadAGF_Triggered()
{
    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setOption(QFileDialog::ShowDirsOnly, true);
    if(fileDialog.exec()){
        QStringList filePaths = fileDialog.selectedFiles();
        _AGFdir = filePaths.first();
    }

    if(!_glassmapmanager->readAllAGF(_AGFdir))
    {
        QMessageBox::information(this,tr("File"), tr("AGF load missed"));
        return;
    }
    _glassmapmanager->createGlassMapList(ui->comboBox_plotType->currentIndex());
    _glassmapmanager->resetAxis(ui->comboBox_plotType->currentIndex());
    createTableWidget();

    QMessageBox::information(this,tr("Load AGF folder"), tr("AGF loaded"));

}

void MainWindow::on_comboChanged(int index)
{
    _glassmapmanager->clearGlassMapList();
    _glassmapmanager->createGlassMapList(index);

    QString supplyer;
    bool pointstate;
    bool labelstate;

    for(int i = 0 ;i<ui->tableWidget->rowCount();i++)
    {
        supplyer = _glassmapmanager->catalog(i)->supplyer();
        pointstate = ui->tableWidget->item(i,ColumnPlot)->checkState();
        labelstate = ui->tableWidget->item(i,ColumnLabel)->checkState();
        _glassmapmanager->setChartVisible(supplyer,pointstate,labelstate);
    }

    _glassmapmanager->resetAxis(ui->comboBox_plotType->currentIndex());
    _glassmapmanager->replot();

}

void MainWindow::on_cellChanged(int catalogIndex, int PlotOrLabel)
{

    QString supplyer = _glassmapmanager->catalog(catalogIndex)->supplyer();
    bool pointstate = ui->tableWidget->item(catalogIndex,ColumnPlot)->checkState();
    bool labelstate = ui->tableWidget->item(catalogIndex,ColumnLabel)->checkState();
    _glassmapmanager->setChartVisible(supplyer,pointstate,labelstate);

    _glassmapmanager->replot();

}

void MainWindow::on_checkBoxCurveChanged(bool checkState)
{
    QList<double> coefs;
    coefs.clear();

    coefs.append(ui->lineEdit_coef0->text().toDouble());
    coefs.append(ui->lineEdit_coef1->text().toDouble());
    coefs.append(ui->lineEdit_coef2->text().toDouble());
    coefs.append(ui->lineEdit_coef3->text().toDouble());

    _glassmapmanager->setCurveCoefs(coefs);
    _glassmapmanager->setCurveVisible(checkState);
    _customPlot->replot();
}

void MainWindow::on_lineEdit_textEdited(QString linetext)
{
    QList<double> coefs;

    coefs.append(ui->lineEdit_coef0->text().toDouble());
    coefs.append(ui->lineEdit_coef1->text().toDouble());
    coefs.append(ui->lineEdit_coef2->text().toDouble());
    coefs.append(ui->lineEdit_coef3->text().toDouble());

    _glassmapmanager->setCurveCoefs(coefs);
    _glassmapmanager->replot();

}

void MainWindow::on_buttonResetViewClicked()
{
    _glassmapmanager->resetAxis(ui->comboBox_plotType->currentIndex());
    _glassmapmanager->replot();
}

void MainWindow::on_menu_Help_Abou_Triggered()
{
    QMessageBox::about(this,tr("About"),tr(" GlassPlotter\n\n Copyright(c) 2020 Hiiragi(heterophyllus) "));
}
