/*! \mainpage Gantt documentation
 *
 * \section intro_sec Introduction
 *
 * Basic functionality:
 *
 * MainWindow::reset() will clear or reset everything.
 * MainWindow::openCSV() or MainWindow::openGDX() will add flows and tasks with MainWindow::addTask() and MainWindow::addFlow().
 * MainWindow::createRepresentation will "bake" all tasks and flows and generate the scene.
 *
 * \section install_sec Installation
 *
 * To cross compile on unix for any platform: http://stackoverflow.com/questions/14170590/building-qt-5-on-linux-for-windows/14170591#14170591
 *
 * (set $PATH to mxe/usr/bin)
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtMath>
#include <QDebug>
#include <QPen>
#include <QTime>
#include <QFileDialog>
#include <QFile>
#include <QKeyEvent>
#include <QWidget>
#include <QMessageBox>
#include <QToolTip>
#include <QInputDialog>
#include "constants.h"
#include "qtablewidgetnumberitem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gclgms.h"
#include "gdxcc.h"
//INIT stuff

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QWidget::setWindowIcon(QIcon("gantticon.png"));
    QWidget::setWindowTitle("Gantt");
    connect(ui->verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(verticalZoomSliderChanged(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(horizontalZoomSliderChanged(int)));
    connect(ui->v_zoom_in,SIGNAL(pressed()),this,SLOT(v_zoomin()));
    connect(ui->v_zoom_out,SIGNAL(pressed()),this,SLOT(v_zoomout()));
    connect(ui->h_zoom_in,SIGNAL(pressed()),this,SLOT(h_zoomin()));
    connect(ui->h_zoom_out,SIGNAL(pressed()),this,SLOT(h_zoomout()));
    connect(ui->saveButton, SIGNAL(pressed()), this, SLOT(saveLogs()));
    connect(ui->allTasksTable, SIGNAL(cellDoubleClicked(int,int)),this, SLOT(taskTableClicked(int,int)));

    connect(ui->ruleAmount, SIGNAL(currentIndexChanged(int)),this, SLOT(ruleChangedAmount(int)));
    connect(ui->ruleDuration, SIGNAL(currentIndexChanged(int)),this, SLOT(ruleChangedDuration(int)));
    connect(ui->ruleStart, SIGNAL(currentIndexChanged(int)),this, SLOT(ruleChangedStart(int)));
    connect(ui->ruleEnd, SIGNAL(currentIndexChanged(int)),this, SLOT(ruleChangedEnd(int)));
    connect(ui->filterApplyButton, SIGNAL(pressed()),this, SLOT(applyFilter()));
    connect(ui->filterClearButton, SIGNAL(pressed()),this, SLOT(clearFilter()));

    //loadData();

    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->hideSection(6);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    ui->ganttView->viewport()->installEventFilter(this);

    createMenu();
    reset();
    if (getSysdir() == ""){
        selectSysdir();
    }
}


//create Menu (File, Edit...)
void MainWindow::createMenu(){
    openAct = new QAction(tr("&Open"),this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Load a csv or gdx file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    resetAct = new QAction(tr("&Reset"),this);
    resetAct->setShortcuts(QKeySequence::Refresh);
    resetAct->setStatusTip(tr("Reset"));
    connect(resetAct, &QAction::triggered, this, &MainWindow::reset);

    selectSysdirAct = new QAction(tr("&Select GAMS Sysdir"),this);
    selectSysdirAct->setShortcuts(QKeySequence::Save);
    selectSysdirAct->setStatusTip(tr("Select the location of the GAMS Sysdir"));
    connect(selectSysdirAct, &QAction::triggered, this, &MainWindow::selectSysdir);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(resetAct);
    fileMenu->addAction(selectSysdirAct);

    checkAct = new QAction(tr("&Check"),this);
    checkAct->setShortcut(QKeySequence::New);
    checkAct->setStatusTip(tr("Check the file for overlapping tasks, Correct flow amount sum and time order in flow"));
    connect(checkAct, &QAction::triggered, this, &MainWindow::check);

    checkMenu = menuBar()->addMenu(tr("&Check"));
    checkMenu->addAction(checkAct);

    visualizeamountsAct = new QAction(tr("&Visualise Amounts"),this);
    visualizeamountsAct->setShortcuts(QKeySequence::Paste);
    visualizeamountsAct->setCheckable(true);
    visualizeamountsAct->setStatusTip(tr("Visualize amounts in the Gantt Chart, relative to highest amount"));
    connect(visualizeamountsAct, &QAction::changed, this, &MainWindow::visualizeamounts);

    showAllFlowsAct = new QAction(tr("&Show all Flows"), this);
    showAllFlowsAct->setShortcut(QKeySequence::Find);
    showAllFlowsAct->setCheckable(true);
    showAllFlowsAct->setStatusTip(tr("Show all flows, regardless of the current selection"));
    connect(showAllFlowsAct, &QAction::changed, this, &MainWindow::showAllFlowsToggled);

    zoominAct = new QAction(tr("&Zoom in"), this);
    zoominAct->setShortcuts(QList<QKeySequence>() << QKeySequence::ZoomIn << QKeySequence::Undo);
    zoominAct->setStatusTip(tr("Zoom in, Keeping aspect ratio"));
    connect(zoominAct, &QAction::triggered, this, &MainWindow::v_zoomin);
    connect(zoominAct, &QAction::triggered, this, &MainWindow::h_zoomin);

    zoomoutAct = new QAction(tr("&Zoom out"), this);
    zoomoutAct->setShortcut(QKeySequence::ZoomOut);
    zoomoutAct->setStatusTip(tr("Zoom out, Keeping aspect ratio"));
    connect(zoomoutAct, &QAction::triggered, this, &MainWindow::v_zoomout);
    connect(zoomoutAct, &QAction::triggered, this, &MainWindow::h_zoomout);

    centerAct = new QAction(tr("&Center view"), this);
    centerAct->setShortcut(QKeySequence::Copy);
    centerAct->setStatusTip(tr("Fit view to contents"));
    connect(centerAct, &QAction::triggered, this, &MainWindow::center);

    scrollnAct = new QAction(tr("&Scroll Up"), this);
    scrollnAct->setShortcut(QKeySequence(Qt::Key_Up));
    scrollnAct->setStatusTip(tr("Scroll up"));
    connect(scrollnAct, &QAction::triggered, this, &MainWindow::scrolln);

    scrolleAct = new QAction(tr("&Scroll Right"), this);
    scrolleAct->setShortcut(QKeySequence(Qt::Key_Right));
    scrolleAct->setStatusTip(tr("Scroll right"));
    connect(scrolleAct, &QAction::triggered, this, &MainWindow::scrolle);

    scrollsAct = new QAction(tr("&Scroll Down"), this);
    scrollsAct->setShortcut(QKeySequence(Qt::Key_Down));
    scrollsAct->setStatusTip(tr("Scroll down"));
    connect(scrollsAct, &QAction::triggered, this, &MainWindow::scrolls);

    scrollwAct = new QAction(tr("&Scroll Left"), this);
    scrollwAct->setShortcut(QKeySequence(Qt::Key_Left));
    scrollwAct->setStatusTip(tr("Scroll left"));
    connect(scrollwAct, &QAction::triggered, this, &MainWindow::scrollw);

    colorByAmountAct = new QAction(tr("&Color by Amount"), this);
    colorByAmountAct->setShortcut(QKeySequence::SelectAll);
    colorByAmountAct->setStatusTip(tr("Color tasks relative to their amount, from white over yellow to red"));
    colorByAmountAct->setCheckable(true);
    connect(colorByAmountAct, &QAction::triggered, this, &MainWindow::colorByAmount);

    colorByUnitAct = new QAction(tr("&Color by Unit"), this);
    colorByUnitAct->setShortcut(QKeySequence(Qt::Key_U));
    colorByUnitAct->setStatusTip(tr("Color tasks like their unit"));
    colorByUnitAct->setCheckable(true);
    connect(colorByUnitAct, &QAction::triggered, this, &MainWindow::colorByUnit);

    colorByTaskAct = new QAction(tr("&Color by Task"), this);
    colorByTaskAct->setShortcut(QKeySequence(Qt::Key_T));
    colorByTaskAct->setStatusTip(tr("Color tasks with same name in same color"));
    colorByTaskAct->setCheckable(true);
    connect(colorByTaskAct, &QAction::triggered, this, &MainWindow::colorByTask);

    colorByColorAct = new QAction(tr("&Color by Color"), this);
    colorByColorAct->setShortcut(QKeySequence(Qt::Key_D));
    colorByColorAct->setStatusTip(tr("Color tasks with predefined color"));
    colorByColorAct->setCheckable(true);
    connect(colorByColorAct, &QAction::triggered, this, &MainWindow::colorByColor);

    taskColoringMenu = new QActionGroup(this);
    taskColoringMenu->setExclusive(true);
    taskColoringMenu->addAction(colorByAmountAct);
    taskColoringMenu->addAction(colorByUnitAct);
    taskColoringMenu->addAction(colorByTaskAct);
    taskColoringMenu->addAction(colorByColorAct);
    colorByColorAct->setChecked(true);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(visualizeamountsAct);
    viewMenu->addAction(showAllFlowsAct);
    viewMenu->addSeparator();
    viewMenu->addAction(zoominAct);
    viewMenu->addAction(zoomoutAct);
    viewMenu->addAction(centerAct);
    viewMenu->addSeparator();
    viewMenu->addAction(scrollnAct);
    viewMenu->addAction(scrolleAct);
    viewMenu->addAction(scrollsAct);
    viewMenu->addAction(scrollwAct);
    viewMenu->addSeparator()->setText(tr("Task Coloring"));
    viewMenu->addAction(colorByAmountAct);
    viewMenu->addAction(colorByUnitAct);
    viewMenu->addAction(colorByTaskAct);
    viewMenu->addAction(colorByColorAct);

    exportAct = new QAction(tr("&Export"));
    exportAct->setShortcut(QKeySequence(Qt::Key_E));
    exportAct->setToolTip(tr("Export the current view to PNG"));
    connect(exportAct, &QAction::triggered, this, &MainWindow::exportToImage);

    exportMenu = menuBar()->addMenu(tr("&Export"));
    exportMenu->addAction(exportAct);

}
void MainWindow::selectSysdir(){
    QString newSysdir = QFileDialog::getExistingDirectory(this, "Select the GAMS Sysdir");
    if (newSysdir == 0){
        return;
    }
    QFile file(XMLCONFIGFILE);
    if(!file.open(QFile::WriteOnly)){
        QMessageBox errorBox;
        errorBox.critical(0,"Error", "Could not open file for reading");
        return;
    }

    QTextStream in(&file);
    in << newSysdir;
    file.close();
}
QString MainWindow::getSysdir(){
    QFile file(XMLCONFIGFILE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox errorBox;
        //errorBox.critical(0,"Error", "Could not open config for reading, GAMS Sysdir has not been set");

        return "";
    }
    QTextStream in(&file);
    QString out = in.readLine();
    file.close();
    qDebug()<<"SYSDIR: "<< out;
    return out;
}

//reset
void MainWindow::reset(){
    while (ui->allTasksTable->rowCount() > 0){
    ui->allTasksTable->removeRow(0);
    }
    QWidget::setWindowTitle("Gantt");
    openFile = "";
    ui->errorsViewer->setPlainText("No check has been run yet. Run check to check for overlapping tasks and Flow amount sum.");
    colorByColorAct->setChecked(true);
    visualizeamountsAct->setChecked(false);
    showAllFlowsAct->setChecked(false);
    showAllFlows = -2;
    ui->tableWidget->setItem(0,1, new QTableWidgetItem(""));
    ui->tableWidget->setItem(1,1, new QTableWidgetItem(""));
    ui->tableWidget->setItem(2,1, new QTableWidgetItem(""));
    ui->tableWidget->setItem(3,1, new QTableWidgetItem(""));
    ui->tableWidget->setItem(4,1, new QTableWidgetItem(""));
    ui->tableWidget->setItem(5,1, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0,0, new QTableWidgetItem(""));
    ui->tableWidget->setItem(1,0, new QTableWidgetItem(""));
    ui->tableWidget->setItem(2,0, new QTableWidgetItem(""));
    ui->tableWidget->setItem(3,0, new QTableWidgetItem(""));
    ui->tableWidget->setItem(4,0, new QTableWidgetItem(""));
    ui->tableWidget->setItem(5,0, new QTableWidgetItem(""));
    currentClickedTaskIndex = -1;
    currentClickedFlowIndex = -1;
    largestAmount = 0;
    longestTask = ui->ganttView->width();
    tasks = new QList<Task>();
    taskColors = new QMap<QString, QColor>();
    units = new QStringList();
    unitColors = new QMap<QString, QColor>();
    flows = new QList<Flow>();
    scene = new QGraphicsScene(0,0,ui->ganttView->frameSize().width(),ui->ganttView->frameSize().height());
    ui->ganttView->setScene(scene);
    tasksRep = new QList<GanttRect*>();
    lines = new QList<QGraphicsLineItem*>();
    flowsRep = new QList<GanttFlow*>();
    unitLabels = new QList<QGraphicsTextItem*>();

}

//DATA stuff
void MainWindow::openCSV(QString fileName){


    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox errorBox;
        errorBox.critical(0,"Error", "Could not open file for reading");
        return;
    }
    reset();
    QWidget::setWindowTitle(fileName);
    openFile = fileName;
    QTextStream in(&file);
    QStringList fileContents;
    while (true){
        QString line = in.readLine();
        if (line.isNull())
            break;
        else
            fileContents.append(line);
    }

    foreach (QString line, fileContents) {
        //qDebug()<<line;

        QStringList lineContents = line.split(";",QString::SkipEmptyParts);
        if (lineContents.count() == 6){
            //Task with color
            line.replace(" ", "");
            lineContents = line.split(";",QString::SkipEmptyParts);

            addTask(lineContents.at(0), lineContents.at(1), lineContents.at(2).toFloat(), lineContents.at(3).toFloat(), lineContents.at(4).toFloat(), lineContents.at(5));
        } else if (lineContents.count() == 5){
            //Task without color
            line.replace(" ", "");
            lineContents = line.split(";",QString::SkipEmptyParts);
            addTask(lineContents.at(0), lineContents.at(1), lineContents.at(2).toFloat(), lineContents.at(3).toFloat(), lineContents.at(4).toFloat());
        } else if (lineContents.count() == 2){
            // Unit color
            line.replace(" ", "");
            unitColors->insert((line.at(0)),QColor(line.at(1)));



        } else if (line.toStdString()[0] != '*' && (lineContents = line.split("->")).size() == 2){

            line.replace("(", " ");
            line.replace(")", " ");
            line.replace("=", " ");
            line.replace("->", " ");
            lineContents = line.split(" ",QString::SkipEmptyParts);

            addFlow(lineContents[0],
                    lineContents[1],
                    lineContents[3],
                    lineContents[4],
                    lineContents[2].toFloat(),
                    lineContents[5].toFloat(),
                    lineContents[6].toFloat()
                    );



        } else {
            qDebug() << "Invalid Line" << line;
        }

    }
    qDebug("Loaded %d Tasks on %d Units and %d flows",tasks->count(),units->count(),flows->count());

}
void MainWindow::openGDX(QString filename){
    //qDebug("Filename: %s", filename);
    static gdxStrIndexPtrs_t Indx;
    static gdxStrIndex_t IndxXXX;
    static gdxValues_t Values;
    //qDebug("Filename: %s", filename);
    gdxHandle_t PGX = NULL;
    char Msg[GMS_SSSIZE];
    char Producer[GMS_SSSIZE];
    int ErrNr;
    int VarNr;
    int NrRecs;
    int N;
    int Dim;
    char VarName[GMS_SSSIZE];
    int VarTyp;
    int D;
    //qDebug("FilenameX: %s", filename);


    if (!gdxCreateD(&PGX, getSysdir().toLatin1().data(), Msg, sizeof(Msg))) {
        qDebug("**** Could not load GDX library\n");
        qDebug("**** %s\n", Msg);
        return;
    }

    gdxGetDLLVersion(PGX, Msg);

    qDebug("Using GDX DLL version: %s\n", Msg);
    //qDebug("using GAMS system directory: %s\n", Sysdir);

    GDXSTRINDEXPTRS_INIT(IndxXXX, Indx);
    //qDebug("Filename2: %s", filename);
    gdxOpenRead(PGX, filename.toLatin1().data(), &ErrNr);
    if (ErrNr) {
        qDebug("**** Fatal I/O Error = %d when calling %s\n", ErrNr, "gdxOpenRead");

        gdxErrorStr(PGX, ErrNr, Msg);
        qDebug("%s", Msg);
        return;
    }
    reset();
    QWidget::setWindowTitle(filename);
    openFile = filename;
    gdxFileVersion(PGX, Msg, Producer);
    qDebug("GDX file written using version: %s\n", Msg);
    qDebug("GDX file written by: %s\n", Producer);

    if (!gdxFindSymbol(PGX, "GanttData", &VarNr)) {
        qDebug("**** Could not find variable GanttData\n");
        return;
    }

    gdxSymbolInfo(PGX, VarNr, VarName, &Dim, &VarTyp);
    qDebug("Dim : %d\n", Dim);
    /*
   * if (Dim != 2 || GMS_DT_VAR != VarTyp) { qDebug("**** X is not a
   * two dimensional variable: %d:%d\n",Dim,VarTyp); return; }
   */

    if (!gdxDataReadStrStart(PGX, VarNr, &NrRecs)) {
        char S[GMS_SSSIZE];

        qDebug("**** Fatal GDX Error\n");
        gdxErrorStr(PGX, gdxGetLastError(PGX), S);
        qDebug("**** %s\n", S);
        return;
    }
    qDebug("GanttData has %d records\n", NrRecs);
    char unitName[GMS_SSSIZE ];
    char taskName[GMS_SSSIZE ];
    float start = 0;
    float end = 0;
    float amount = 0;
    char color[8];
    strcpy(color, "random");

    int numberOfTasks = 0;
    while (gdxDataReadStr(PGX, Indx, Values, &N)) {
        if (N < 3) {
            if (numberOfTasks == 0) {
                numberOfTasks++;
            }
            else {
                numberOfTasks++;
                addTask(unitName, taskName, start, end, amount, color);
                start = 0;
                end = 0;
                amount = 0;
                strcpy(color, "random");
            }
            strcpy(unitName, Indx[0]);
            strcpy(taskName, Indx[1]);
        }
        if (strcmp(Indx[3], "Start") == 0) {
            if (Values[GMS_VAL_LEVEL] != GMS_SV_EPS) {
                start = Values[GMS_VAL_LEVEL];
            }
        }
        if (strcmp(Indx[3], "End") == 0) {
            if (Values[GMS_VAL_LEVEL] != GMS_SV_EPS) {
                end = Values[GMS_VAL_LEVEL];
            }
        }
        if (strcmp(Indx[3], "Amount") == 0) {
            if (Values[GMS_VAL_LEVEL] != GMS_SV_EPS) {
                amount = Values[GMS_VAL_LEVEL];
            }
        }
        if (strcmp(Indx[3], "Color") == 0) {
            if (Values[GMS_VAL_LEVEL] == GMS_SV_EPS) {
                strcpy(color, "#000000");
            }
            if (Values[GMS_VAL_LEVEL] <= 0xFFFFFF) {
                sprintf(color, "#%06x", (int)Values[GMS_VAL_LEVEL]);
            }
        }
    }
    addTask(unitName, taskName, start, end, amount, color);
    qDebug("Found %d tasks.\n", numberOfTasks);

    if (!gdxFindSymbol(PGX, "FlowData", &VarNr)) {
        qDebug("**** Could not find variable FlowData\n");
        return;
    }

    gdxSymbolInfo(PGX, VarNr, VarName, &Dim, &VarTyp);
    qDebug("Dim : %d\n", Dim);

    if (!gdxDataReadStrStart(PGX, VarNr, &NrRecs)) {
        char S[GMS_SSSIZE];

        qDebug("**** Fatal GDX Error\n");
        gdxErrorStr(PGX, gdxGetLastError(PGX), S);
        qDebug("**** %s\n", S);
    }

    qDebug("FlowData has %d records\n", NrRecs);

    char unit1name[GMS_SSSIZE ];
    char task1name[GMS_SSSIZE ];
    char unit2name[GMS_SSSIZE ];
    char task2name[GMS_SSSIZE ];
    float pr = 0;
    float cr = 0;
    amount = 0;
    int numberOfFlows = 0;
    while (gdxDataReadStr(PGX, Indx, Values, &N)) {
        if (N < 5) {
            if (numberOfFlows == 0) {
                numberOfFlows++;
            }
            else {
                numberOfFlows++;
                addFlow(unit1name, task1name, unit2name, task2name, pr, cr, amount);
                pr = 0;
                cr = 0;
                amount = 0;
            }
            strcpy(unit1name, Indx[0]);
            strcpy(task1name, Indx[1]);
            strcpy(unit2name, Indx[2]);
            strcpy(task2name, Indx[3]);
        }
        if (strcmp(Indx[4], "Amount") == 0) {
            if (Values[GMS_VAL_LEVEL] != GMS_SV_EPS) {
                amount = Values[GMS_VAL_LEVEL];
            }
        }
        if (strcmp(Indx[4], "ProductionRate") == 0) {
            if (Values[GMS_VAL_LEVEL] != GMS_SV_EPS) {
                pr = Values[GMS_VAL_LEVEL];
            }
        }
        if (strcmp(Indx[4], "ConsumptionRate") == 0) {
            if (Values[GMS_VAL_LEVEL] != GMS_SV_EPS) {
                cr = Values[GMS_VAL_LEVEL];
            }
        }
    }
    addFlow(unit1name, task1name, unit2name, task2name, pr, cr, amount);

    qDebug("Found %d flows.\n", numberOfFlows);
    gdxDataReadDone(PGX);

    if ((ErrNr = gdxClose(PGX))) {
        qDebug("**** Fatal I/O Error = %d when calling %s\n", ErrNr, "gdxClose");
    }

    if (!gdxFree(&PGX)) {
        qDebug("Problems unloading the GDX DLL\n");
        return;
    }
}



//parse csv file
void MainWindow::open(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open csv or gdx"), defaultDirectory, "CSV or GDX files (*.csv *.gdx)");
    if ( fileName == ""){
        return;
    }
    qDebug()<<fileName;
    if (QString(fileName.split(".").last()) == QString("csv")){
        openCSV(fileName);
    } else {
        char * c_filename = fileName.toLatin1().data();
        openGDX(c_filename);
    }

    createRepresentation();
    //loadData();
}



//add new task
void MainWindow::addTask(QString unitName, QString taskName, float start, float end, float amount, QString color){

    Task newTask;

    int machineIndex = units->indexOf(unitName);
    if (machineIndex == -1){
        units->append(unitName);
        machineIndex = units->size()-1;
    }
    if (end > longestTask){
        longestTask = end;
    }
    if (amount > largestAmount){
        largestAmount = amount;
    }
    newTask.unitIndex = machineIndex;
    newTask.name = taskName;
    newTask.start = start;
    newTask.end = end;
    newTask.amount = amount;
    newTask.color = QColor();
    if (color == "random"){
        newTask.color.setHsv(qrand()%255, qrand()%128+127, qrand()%128+127);
    } else{
        newTask.color.setNamedColor(color);
        if (!unitColors->contains(units->at(machineIndex))){
            qDebug()<<color << taskName;
            qDebug("%s has been assigned the color %s", unitName.toStdString().c_str(), color.toStdString().c_str());
            unitColors->insert(units->at(machineIndex), newTask.color);
        }
        if (!taskColors->contains(taskName)){
            qDebug()<<color << taskName;
            qDebug("%s has been assigned the color %s", taskName.toStdString().c_str(), color.toStdString().c_str());
            unitColors->insert(taskName, newTask.color);
        }
    }
    newTask.taskIndex = tasks->size();
    tasks->append(newTask);





}

//add new flow
void MainWindow::addFlow(QString unit1name, QString op1name, QString unit2name, QString op2name, float pr, float cr, float amount){
    Flow newFlow;
    for (int i = 0; i < units->size(); i++){
        if (units->at(i) == unit1name){
            newFlow.unit1Index = i;
        }
        if (units->at(i) == unit2name){
            newFlow.unit2Index = i;
        }
    }

    for (int i = 0; i < tasks->size(); i++){
        if (units->at(tasks->at(i).unitIndex) == unit1name && tasks->at(i).name == op1name){
            newFlow.op1Index = i;
        }
        if (units->at(tasks->at(i).unitIndex) == unit2name && tasks->at(i).name == op2name){
            newFlow.op2Index = i;
        }
    }
    newFlow.productionRate = pr;
    newFlow.consumptionRate = cr;
    newFlow.amount = amount;
    newFlow.flowIndex = flows->size();
    bool success = true;
    if (newFlow.unit1Index == -1){
        qDebug() << "Invalid Flow: Cant find unit " <<  unit1name;
        success = false;
    }
    if (newFlow.unit2Index == -1){
        qDebug() << "Invalid Flow: Cant find unit " <<  unit2name;
        success = false;
    }
    if (newFlow.op1Index == -1){
        qDebug() << "Invalid Flow: Cant find task " <<  op1name;
        success = false;
    }
    if (newFlow.op2Index == -1){
        qDebug() << "Invalid Flow: Cant find task " <<  op2name;
        success = false;
    }
    if (success){
        flows->append(newFlow);
    } else {
        qDebug() << "Could not add Task.";
    }
}

//create Gantt chart
void MainWindow::createRepresentation(){
    //Seed qrand for random colors
    qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));
    //populate UnitColors with random colors
    for (int i = 0; i < units->size(); i++){
        if (!unitColors->contains(units->at(i))){
            QColor rancol = QColor();
            rancol.setHsv(qrand()%255, qrand()%128+127, qrand()%128+127);
            unitColors->insert(units->at(i), rancol);
        }
    }
    //Draw Ruler
    QPen rulerPen(Qt::lightGray, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    for (int i = 0; i < longestTask/50 + 2; i++){
        QGraphicsTextItem * rulerlabel = new QGraphicsTextItem();
        rulerlabel->setHtml(QString("<div style='background:rgba(255, 255, 255, 50%);'>" + QString::number(i*50) + "</div>") );
        rulerlabel->setDefaultTextColor(Qt::black);
        rulerlabel->setPos(i*50 + UNITNAMEBARWIDTH,UNITREPHEIGHT * (units->size()+1));
        rulerlabel->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        scene->addItem(rulerlabel);
        QGraphicsLineItem* line = new QGraphicsLineItem(i*50 + UNITNAMEBARWIDTH,0,i*50 + UNITNAMEBARWIDTH,(units->length()+1)*UNITREPHEIGHT);

        line->setPen(rulerPen);
        scene->addItem(line);

    }
    //Draw Lines
    QPen unitPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    unitPen.setWidth(0);
    for (int i = 0; i < units->size(); i++){
        QGraphicsLineItem* line = new QGraphicsLineItem(0,(i+1)*UNITREPHEIGHT,longestTask + UNITNAMEBARWIDTH,(i+1)*UNITREPHEIGHT);
        line->setPen(unitPen);
        scene->addItem(line);
        lines->append(line);
    }
    //Draw Tasks
    for (int i = 0; i < tasks->size(); i++){
        Task task = tasks->at(i);
        GanttRect * newRect = new GanttRect(task.taskIndex,task.unitIndex,largestAmount,task.start,task.end,task.amount, task.color);
        scene->addItem(newRect);
        tasksRep->append(newRect);
        //Add Task to table

        ui->allTasksTable->insertRow(i);
        ui->allTasksTable->setItem(i,0,new QTableWidgetItem(units->at(task.unitIndex)));
        ui->allTasksTable->setItem(i,1,new QTableWidgetItem(task.name));
        ui->allTasksTable->setItem(i,2,new QTableWidgetNumberItem(task.start));
        ui->allTasksTable->setItem(i,3,new QTableWidgetNumberItem(task.end));
        ui->allTasksTable->setItem(i,4,new QTableWidgetNumberItem(task.end-task.start));
        ui->allTasksTable->setItem(i,5,new QTableWidgetNumberItem(task.amount));
        ui->allTasksTable->setItem(i,6,new QTableWidgetNumberItem(i));

        // UI Actions
        connect(newRect, SIGNAL(iWasClicked(int)),this,SLOT(taskClicked(int)));
        connect(newRect, SIGNAL(hoverStart(int)), this, SLOT(taskHoverEnter(int)));
        connect(newRect, SIGNAL(hoverLeave(int)), this, SLOT(taskHoverLeave(int)));
    }
    //Draw Flows
    for (int i = 0; i < flows->size(); i++){
        Flow flow = flows->at(i);
        GanttFlow * newLine = new GanttFlow(i,
                                            QPolygonF(QVector<QPointF>()
                                                      << tasksRep->at(flow.op1Index)->boundingRect().topRight()
                                                      << tasksRep->at(flow.op1Index)->boundingRect().bottomRight()
                                                      << tasksRep->at(flow.op2Index)->boundingRect().bottomLeft()
                                                      << tasksRep->at(flow.op2Index)->boundingRect().topLeft()));
        flowsRep->append(newLine);
        scene->addItem(newLine);
        connect(newLine, SIGNAL(iWasClicked(int)), this, SLOT(flowClicked(int)));
    }
    //Draw Labels
    for (int i = 0; i < units->size(); i++){
        QGraphicsTextItem * unitLabel = new QGraphicsTextItem();
        unitLabel->setHtml(QString("<div style='background:rgba(255, 255, 255, 50%);'>" +units->at(i) + "</div>") );
        unitLabel->setDefaultTextColor(Qt::black);
        unitLabel->setPos(0,(i+1)*UNITREPHEIGHT-unitLabel->boundingRect().height()/2);
        unitLabel->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        scene->addItem(unitLabel);
        unitLabels->append(unitLabel);
    }

    showFlows(showAllFlows);


    scene->setSceneRect(0,0,50*(longestTask/50 + 2), (UNITREPHEIGHT + 2) * units->length());
    center();

    //Generate Task Colors

    for (int i = 0; i < tasks->size(); i++){
        if (!taskColors->contains(tasks->at(i).name)){
            QColor rancol = QColor();
            rancol.setHsv(qrand()%255, qrand()%128+127, qrand()%128+127);
            taskColors->insert(tasks->at(i).name,rancol);
        }
    }
    clearFilter();

}

QString MainWindow::describeTask(Task task){
    return "Task " + task.name + " on unit " + units->at(task.unitIndex) + " Start: " + QString::number(task.start) + " End: " + QString::number(task.end) + " Amount: " + QString::number(task.amount);
}

//CHECK stuff
void MainWindow::saveLogs(){
    if (openFile == ""){
        //QMessageBox errorBox;
        //errorBox.critical(0,"Error", "No file opened");
        //ui->saveButton->setChecked(false);
        return;
    } else {
        QMessageBox errorBox;

        //select log file
        QString fileName = QFileDialog::getSaveFileName(this, tr("Location of log file"), openFile+".log", tr("log File (*.log)"));
        QFile file(fileName);
        if (file.open(QIODevice::ReadWrite)){
            QTextStream stream(&file);
            stream << ui->errorsViewer->toPlainText();
            file.close();

        } else {
            errorBox.critical(0,"Error", "Could not open file for writing");
            return;
        }
    }
}

void MainWindow::check(){
    if (openFile == ""){
        QMessageBox errorBox;
        errorBox.critical(0,"Error", "No file opened");
        return;
    }
    int overlaps = 0;
    int flowerrors = 0;
    double tolerance = QInputDialog::getDouble(this,"Input Tolerance","Tolerance:",0);
    QString log = "";
    log += "Tolerance: " + QString::number(tolerance);
    log += "Checking for overlapping tasks\n";
    for (int i = 0; i < tasks->size(); i++){
        for (int ii = i+1; ii< tasks->size(); ii++){
            if (tasks->at(i).unitIndex == tasks->at(ii).unitIndex){
                if (tasks->at(ii).end>tasks->at(i).end && tasks->at(i).end - tolerance > tasks->at(ii).start){

                    log += "\nFound overlap of " + QString::number(tasks->at(i).end - tasks->at(ii).start) + "\n";
                    log += describeTask(tasks->at(i)) + "\n";
                    log += describeTask(tasks->at(ii)) + "\n";
                    overlaps++;

                }
                if (tasks->at(i).end>tasks->at(ii).end && tasks->at(ii).end - tolerance > tasks->at(i).start){
                    log += "\nFound overlap of " + QString::number(tasks->at(ii).end - tasks->at(i).start) + "\n";
                    log += describeTask(tasks->at(i)) + "\n";
                    log += describeTask(tasks->at(ii)) + "\n";
                    overlaps++;

                }
            }
        }
    }
    log += "\n Checking sum of Amounts\n";
    for (int i = 0; i < tasks->size(); i++){
        float receivedInput = 0;
        float expectedInput = 0;
        QList<int> inFlows = QList<int>();
        QList<int> outFlows = QList<int>();
        for (int f = 0; f < flows->size(); f++){
            if (flows->at(f).op2Index == i){
                receivedInput += flows->at(f).amount;
                inFlows.append(f);
            }
            if (flows->at(f).op1Index == i){
                expectedInput += flows->at(f).amount;
                outFlows.append(f);
            }
        }
        if (receivedInput != expectedInput){
            flowerrors++;
            log += "\n Unmatched amounts on \n " + describeTask(tasks->at(i)) + "\n Sum of Flows:\n";
            for (int f = 0; f < inFlows.size(); f++){
                log += "From " + tasks->at(flows->at(inFlows.at(f)).op1Index).name + "\t\t" + QString::number(flows->at(inFlows.at(f)).amount) + "\n";
            }
            if (inFlows.length() > 1){
                log += "=\t\t" + QString::number(receivedInput) + "\n";
            }
            for (int f = 0; f < outFlows.size(); f++){
                log += "To " + tasks->at(flows->at(outFlows.at(f)).op2Index).name + "\t\t" + QString::number(flows->at(outFlows.at(f)).amount);
            }
            if (outFlows.length() > 1){
                log += "=\t\t" + QString::number(expectedInput) + "\n";
            }

        }
    }
    log+= "\n Found " + QString::number(overlaps) + " overlap errors and " + QString::number(flowerrors) + " flow errors.\n";
    ui->errorsViewer->setPlainText(log);


}

//GUI stuff

//toggle relative amounts
void MainWindow::visualizeamounts(){
    for (int i = 0; i < tasks->size(); i++){
        tasksRep->at(i)->showAmount(visualizeamountsAct->isChecked());
    }
    for (int i = 0; i < flows->size(); i++){

        flowsRep->at(i)->setPolygon(QPolygonF(QVector<QPointF>()
                                              << tasksRep->at(flows->at(i).op1Index)->boundingRect().topRight()
                                              << tasksRep->at(flows->at(i).op1Index)->boundingRect().bottomRight()
                                              << tasksRep->at(flows->at(i).op2Index)->boundingRect().bottomLeft()
                                              << tasksRep->at(flows->at(i).op2Index)->boundingRect().topLeft()));
    }
}
//toggle wether flows should be shown
void MainWindow::showAllFlowsToggled(){

    if (showAllFlowsAct->isChecked()){
        showAllFlows = -1;
    } else {
        showAllFlows = -2;
    }

    showFlows(showAllFlows);

}

void MainWindow::colorByColor(){
    for (int i = 0; i < tasks->size(); i++){
        tasksRep->at(i)->setBrush(tasks->at(i).color);
    }
}
void MainWindow::colorByUnit(){
    for (int i = 0; i < tasks->size(); i++){
        //qDebug()<<unitColors->value(units->at(tasks->at(i).unitIndex)).name();
        tasksRep->at(i)->setBrush(unitColors->value(units->at(tasks->at(i).unitIndex)));
    }
}
void MainWindow::colorByTask(){
    for (int i = 0; i < tasks->size(); i++){
        //qDebug()<<unitColors->value(units->at(tasks->at(i).unitIndex)).name();
        tasksRep->at(i)->setBrush(taskColors->value(tasks->at(i).name));
    }
}
void MainWindow::colorByAmount(){
    for (int i = 0; i < tasks->size(); i++){
        float x = tasks->at(i).amount / largestAmount;
        QColor c = QColor();
        if (x < 0.5){ // white to yellow
            c.setHsv(42.5,x*512,255);
        } else { //yellow to red
            c.setHsv(85-85*x,255,255);
        }
        tasksRep->at(i)->setBrush(c);
    }
}

void MainWindow::taskHoverEnter(int index){
    Task t = tasks->at(index);
    QToolTip::showText(mapToGlobal(tasksRep->at(index)->pos().toPoint()),
                       "Name: " + t.name
                       + " Unit: " + units->at(t.unitIndex)
                       + " Start: " + QString::number(t.start)
                       + " End: " + QString::number(t.end)
                       + " Duration: " + QString::number(t.end-t.start)
                       + " Amount: " + QString::number(t.amount),
                       this, tasksRep->at(index)->boundingRect().toRect(),10000000
                       );

}
void MainWindow::taskHoverLeave(int index){
    QToolTip::hideText();
}

//Task Clicked
void MainWindow::taskClicked(int index){
    for (int i = 0; i < tasks->count(); i++){
        //qDebug()<<ui->allTasksTable->item(i,6)->text();
        if (ui->allTasksTable->item(i,6)->text().toInt() == index){
            qDebug("fksdj    %d",i);
            ui->allTasksTable->selectRow(i);
        }
    }
    //Unselect all Flows and Tasks
    if (index > -1){
        flowClicked(currentClickedFlowIndex);
    }
    for (int i = 0; i < tasks->size(); i++){
        tasksRep->at(i)->isNotSelected();
        tasksRep->at(i)->isNotMarkedInFlow();
    }

    if (index != currentClickedTaskIndex){
        tasksRep->at(index)->isSelected();
        Task selectedTask = tasks->at(index);
        showFlows(selectedTask.taskIndex);

        ui->tableWidget->setItem(0,1, new QTableWidgetItem(units->at(selectedTask.unitIndex)));
        ui->tableWidget->setItem(1,1, new QTableWidgetItem(selectedTask.name));
        ui->tableWidget->setItem(2,1, new QTableWidgetItem(QString::number(selectedTask.start)));
        ui->tableWidget->setItem(3,1, new QTableWidgetItem(QString::number(selectedTask.end)));
        ui->tableWidget->setItem(4,1, new QTableWidgetItem(QString::number(selectedTask.end - selectedTask.start)));
        ui->tableWidget->setItem(5,1, new QTableWidgetItem(QString::number(selectedTask.amount)));
        ui->tableWidget->setItem(0,0, new QTableWidgetItem("Unit"));
        ui->tableWidget->setItem(1,0, new QTableWidgetItem("Name"));
        ui->tableWidget->setItem(2,0, new QTableWidgetItem("Start"));
        ui->tableWidget->setItem(3,0, new QTableWidgetItem("End"));
        ui->tableWidget->setItem(4,0, new QTableWidgetItem("Duration"));
        ui->tableWidget->setItem(5,0, new QTableWidgetItem("Amount"));
        currentClickedTaskIndex = index;
    } else {
        showFlows(showAllFlows);
        ui->tableWidget->setItem(0,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(1,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(2,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(3,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(4,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(5,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(0,0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(1,0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(2,0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(3,0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(4,0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(5,0, new QTableWidgetItem(""));
        currentClickedTaskIndex = -1;
    }
}

void MainWindow::taskTableClicked(int row, int col){
    int realIndex = ui->allTasksTable->item(row, 6)->text().toInt();
    qDebug("%d",realIndex);
    taskClicked(realIndex);


}

//Flow clicked
void MainWindow::flowClicked(int index){
    //Unselect all Flows and Tasks
    if (index > -1){
        taskClicked(currentClickedTaskIndex);
    }
    for (int i = 0; i < flows->size(); i++){
        flowsRep->at(i)->isNotSelected();
    }
    if (index != currentClickedFlowIndex){
        showFlows(-2);

        flowsRep->at(index)->isSelected();
        Flow selectedFlow = flows->at(index);
        recursivelyActivateFlowsToTheRight(selectedFlow.op2Index);
        recursivelyActivateFlowsToTheLeft(selectedFlow.op1Index);
        flowsRep->at(index)->show();
        ui->tableWidget->setItem(0,0, new QTableWidgetItem("From"));
        ui->tableWidget->setItem(1,0, new QTableWidgetItem("Production Rate"));
        ui->tableWidget->setItem(2,0, new QTableWidgetItem("To"));
        ui->tableWidget->setItem(3,0, new QTableWidgetItem("Consumption Rate"));
        ui->tableWidget->setItem(4,0, new QTableWidgetItem("Amount"));
        ui->tableWidget->setItem(5,0, new QTableWidgetItem(""));

        ui->tableWidget->setItem(0,1, new QTableWidgetItem(tasks->at(selectedFlow.op1Index).name + " on " + units->at(selectedFlow.unit1Index)));
        ui->tableWidget->setItem(1,1, new QTableWidgetItem(QString::number(selectedFlow.productionRate)));
        ui->tableWidget->setItem(2,1, new QTableWidgetItem(tasks->at(selectedFlow.op2Index).name + " on " + units->at(selectedFlow.unit2Index)));
        ui->tableWidget->setItem(3,1, new QTableWidgetItem(QString::number(selectedFlow.consumptionRate)));
        ui->tableWidget->setItem(4,1, new QTableWidgetItem(QString::number(selectedFlow.amount)));
        ui->tableWidget->setItem(5,1, new QTableWidgetItem(""));
        currentClickedFlowIndex = index;

    } else {
        showFlows(showAllFlows);
        ui->tableWidget->setItem(0,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(1,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(2,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(3,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(4,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(5,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(0,0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(1,0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(2,0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(3,0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(4,0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(5,0, new QTableWidgetItem(""));
        currentClickedFlowIndex = -1;
    }
}
//Filter stuff
void MainWindow::ruleChangedAmount(int index){
    switch (index) {
    case 0: // No rule
        ui->filter1amount->setEnabled(false);
        ui->filter2amount->setEnabled(false);
        break;
    case 1: // Between
        ui->filter1amount->setEnabled(true);
        ui->filter2amount->setEnabled(true);
        break;
    case 2: // Outside of
        ui->filter1amount->setEnabled(true);
        ui->filter2amount->setEnabled(true);
        break;
    case 3: // Greater than
        ui->filter1amount->setEnabled(true);
        ui->filter2amount->setEnabled(false);
        break;
    case 4: // Less than
        ui->filter1amount->setEnabled(true);
        ui->filter2amount->setEnabled(false);
        break;
    default:
        break;
    }
}
void MainWindow::ruleChangedStart(int index){
    switch (index) {
    case 0: // No rule
        ui->filter1start->setEnabled(false);
        ui->filter2start->setEnabled(false);
        break;
    case 1: // Between
        ui->filter1start->setEnabled(true);
        ui->filter2start->setEnabled(true);
        break;
    case 2: // Outside of
        ui->filter1start->setEnabled(true);
        ui->filter2start->setEnabled(true);
        break;
    case 3: // Greater than
        ui->filter1start->setEnabled(true);
        ui->filter2start->setEnabled(false);
        break;
    case 4: // Less than
        ui->filter1start->setEnabled(true);
        ui->filter2start->setEnabled(false);
        break;
    default:
        break;
    }
}
void MainWindow::ruleChangedEnd(int index){
    switch (index) {
    case 0: // No rule
        ui->filter1end->setEnabled(false);
        ui->filter2end->setEnabled(false);
        break;
    case 1: // Between
        ui->filter1end->setEnabled(true);
        ui->filter2end->setEnabled(true);
        break;
    case 2: // Outside of
        ui->filter1end->setEnabled(true);
        ui->filter2end->setEnabled(true);
        break;
    case 3: // Greater than
        ui->filter1end->setEnabled(true);
        ui->filter2end->setEnabled(false);
        break;
    case 4: // Less than
        ui->filter1end->setEnabled(true);
        ui->filter2end->setEnabled(false);
        break;
    default:
        break;
    }
}
void MainWindow::ruleChangedDuration(int index){
    switch (index) {
    case 0: // No rule
        ui->filter1duration->setEnabled(false);
        ui->filter2duration->setEnabled(false);
        break;
    case 1: // Between
        ui->filter1duration->setEnabled(true);
        ui->filter2duration->setEnabled(true);
        break;
    case 2: // Outside of
        ui->filter1duration->setEnabled(true);
        ui->filter2duration->setEnabled(true);
        break;
    case 3: // Greater than
        ui->filter1duration->setEnabled(true);
        ui->filter2duration->setEnabled(false);
        break;
    case 4: // Less than
        ui->filter1duration->setEnabled(true);
        ui->filter2duration->setEnabled(false);
        break;
    default:
        break;
    }
}
void MainWindow::clearFilter(){
    for (int i = 0; i < tasks->count(); i++){
        ui->allTasksTable->showRow(i);
        tasksRep->at(i)->show();
    }
    ui->filterUnit->setText("");
    ui->filterTask->setText("");
    ui->ruleAmount->setCurrentIndex(0);
    ui->ruleStart->setCurrentIndex(0);
    ui->ruleEnd->setCurrentIndex(0);
    ui->ruleDuration->setCurrentIndex(0);
    ui->tableCount->setText(QString::number(tasks->count()) + " Tasks shown");
}
void MainWindow::applyFilter(){
    int x = tasks->count();
    for (int i = 0; i < tasks->count(); i++){
        ui->allTasksTable->hideRow(i);
        tasksRep->at(i)->hide();
        Task task = tasks->at(ui->allTasksTable->item(i,6)->text().toInt());
        bool alive = true;
        if (ui->filterUnit->text() != "")
            alive &= units->at(task.unitIndex).contains(ui->filterUnit->text());
        if (ui->filterTask->text() != "")
            alive &= task.name.contains(ui->filterTask->text());
        switch (ui->ruleAmount->currentIndex()){
        case 0: // No rule
            break;
        case 1: // between
            alive &= task.amount >= ui->filter1amount->value() && task.amount <= ui->filter2amount->value();
            break;
        case 2: // outside of
            alive &= task.amount < ui->filter1amount->value() && task.amount > ui->filter2amount->value();
            break;
        case 3: // greater than
            alive &= task.amount > ui->filter1amount->value();
            break;
        case 4: // less than
            alive &= task.amount < ui->filter1amount->value();
            break;
        }
        switch (ui->ruleStart->currentIndex()){
        case 0: // No rule
            break;
        case 1: // between
            alive &= task.start >= ui->filter1start->value() && task.start <= ui->filter2start->value();
            break;
        case 2: // outside of
            alive &= task.start < ui->filter1start->value() && task.start > ui->filter2start->value();
            break;
        case 3: // greater than
            alive &= task.start > ui->filter1start->value();
            break;
        case 4: // less than
            alive &= task.start < ui->filter1start->value();
            break;
        }
        switch (ui->ruleEnd->currentIndex()){
        case 0: // No rule
            break;
        case 1: // between
            alive &= task.end >= ui->filter1end->value() && task.end <= ui->filter2end->value();
            break;
        case 2: // outside of
            alive &= task.end < ui->filter1end->value() && task.end > ui->filter2end->value();
            break;
        case 3: // greater than
            alive &= task.end > ui->filter1end->value();
            break;
        case 4: // less than
            alive &= task.end < ui->filter1end->value();
            break;
        }
        switch (ui->ruleDuration->currentIndex()){
        case 0: // No rule
            break;
        case 1: // between
            alive &= task.end - task.start >= ui->filter1duration->value() && task.end - task.start <= ui->filter2duration->value();
            break;
        case 2: // outside of
            alive &= task.end - task.start < ui->filter1duration->value() && task.end - task.start > ui->filter2duration->value();
            break;
        case 3: // greater than
            alive &= task.end - task.start > ui->filter1duration->value();
            break;
        case 4: // less than
            alive &= task.end - task.start < ui->filter1duration->value();
            break;
        }
        if (alive){
            ui->allTasksTable->showRow(i);
            tasksRep->at(task.taskIndex)->show();
        } else {
            x--;
        }
    }
    ui->tableCount->setText(QString::number(x) + " Tasks shown");
}


//Flow Stuff
void MainWindow::showFlows(int taskIndex){
    switch (taskIndex){
    case -1: // show all flows
        for (int i = 0; i < flows->size(); i++){
            flowsRep->at(i)->show();
        }
        break;
    case -2: // hide all flows
        for (int i = 0; i < flows->size(); i++){
            flowsRep->at(i)->hide();
        }
        break;
    default:
    {
        recursivelyActivateFlowsToTheRight(taskIndex);
        recursivelyActivateFlowsToTheLeft(taskIndex);
    }
    }
}
void MainWindow::recursivelyActivateFlowsToTheRight(int task){

    for (int i = 0; i < flows->size(); i++) {
        if (task == flows->at(i).op1Index){
            flowsRep->at(flows->at(i).flowIndex)->show();
            recursivelyActivateFlowsToTheRight(flows->at(i).op2Index);
        }
    }

}
void MainWindow::recursivelyActivateFlowsToTheLeft(int task){


    for (int i = 0; i < flows->size(); i++) {
        if (task == flows->at(i).op2Index){
            flowsRep->at(flows->at(i).flowIndex)->show();
            recursivelyActivateFlowsToTheLeft(flows->at(i).op1Index);
        }
    }

}

//Scroll
void MainWindow::scrolln(){
    ui->ganttView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->ganttView->translate(0,SCROLLAMOUNT);
    ui->ganttView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}
void MainWindow::scrolle(){
    ui->ganttView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->ganttView->translate(-SCROLLAMOUNT, 0);
    ui->ganttView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}
void MainWindow::scrolls(){
    ui->ganttView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->ganttView->translate(0,-SCROLLAMOUNT);
    ui->ganttView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}
void MainWindow::scrollw(){
    ui->ganttView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->ganttView->translate(SCROLLAMOUNT, 0);
    ui->ganttView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

//Zoom
void MainWindow::v_zoomin(){
    ui->ganttView->scale(1,2);
}
void MainWindow::v_zoomout(){
    ui->ganttView->scale(1,0.5);
}
void MainWindow::h_zoomin(){
    ui->ganttView->scale(2,1);
}
void MainWindow::h_zoomout(){
    ui->ganttView->scale(0.5,1);
}
void MainWindow::verticalZoomSliderChanged(int x){
    resetZoom();
    verticalZoom = qPow(SCALERANGE, (x+1)/50.0-1.0);
    ui->ganttView->scale(horizontalZoom, verticalZoom);
    qDebug("%f,%f",horizontalZoom,verticalZoom);
}
void MainWindow::horizontalZoomSliderChanged(int x){
    resetZoom();
    horizontalZoom = qPow(SCALERANGE, (x+1)/50.0-1.0);
    ui->ganttView->scale(horizontalZoom, verticalZoom);
    qDebug("%f,%f",horizontalZoom,verticalZoom);
}
void MainWindow::resetZoom(){

    ui->ganttView->scale(1/horizontalZoom, 1/verticalZoom);

}
void MainWindow::center(){
    ui->verticalSlider->setValue(50);
    ui->horizontalSlider->setValue(50);
    verticalZoom = 1;
    horizontalZoom = 1;
    //ui->ganttView->scale();
    ui->ganttView->fitInView(scene->sceneRect());
}
void MainWindow::wheelEvent(QWheelEvent *e){

    if (QApplication::keyboardModifiers().testFlag(Qt::KeyboardModifier::ControlModifier)){
        ui->verticalSlider->setValue(ui->verticalSlider->value()+e->delta()/100);
        ui->horizontalSlider->setValue(ui->horizontalSlider->value()+e->delta()/100);
    }
}
bool MainWindow::eventFilter(QObject *watched, QEvent *event){
    if (watched == ui->ganttView->viewport() && event->type() == QEvent::Wheel && QApplication::keyboardModifiers().testFlag(Qt::KeyboardModifier::ControlModifier)){
        return true;
    }
    return false;
}

void MainWindow::resizeEvent(QResizeEvent * event)
{

    //fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    //

};
// Export

void MainWindow::exportToImage(){
    if (openFile == ""){
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this,"Save as",openFile+".png", "Image files (*.png *.jpg *bmp)");
    if (filename == ""){
        return;
    }
    QPixmap pixMap = QPixmap::grabWidget(ui->ganttView);
    pixMap.save(filename);
}

//Pan
void MainWindow::keyPressEvent(QKeyEvent *e){
    if (e->key() == Qt::Key_Shift){
        ui->ganttView->setDragMode(QGraphicsView::ScrollHandDrag);
    }else {
        //MainWindow::keyPressEvent(e);
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent *e){
    if (e->key() == Qt::Key_Shift){
        ui->ganttView->setDragMode(QGraphicsView::NoDrag);
    } else {
        //MainWindow::keyPressEvent(e);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}