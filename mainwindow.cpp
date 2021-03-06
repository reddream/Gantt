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
 * -# add mxe to PATH: \code export PATH=$PATH:<mxe install location>/usr/bin \endcode
 * -# to compile for Windows, run
 * \code
 * cd release
 * i686-w64-mingw32.static-qmake-qt5 ..
 * make
 * \endcode
 *
 * @author Julius Naeumann, julius.naeumann@gmail.com
 * @date August 2016
 *
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
#include <QPrinter>
#include "exportwizard.h"
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

    connect(ui->filter1amount, SIGNAL(valueChanged(double)), this, SLOT(applyFilter()));
    connect(ui->filter2amount, SIGNAL(valueChanged(double)), this, SLOT(applyFilter()));
    connect(ui->filter1start, SIGNAL(valueChanged(double)), this, SLOT(applyFilter()));
    connect(ui->filter2start, SIGNAL(valueChanged(double)), this, SLOT(applyFilter()));
    connect(ui->filter1end, SIGNAL(valueChanged(double)), this, SLOT(applyFilter()));
    connect(ui->filter2end, SIGNAL(valueChanged(double)), this, SLOT(applyFilter()));
    connect(ui->filter1duration, SIGNAL(valueChanged(double)), this, SLOT(applyFilter()));
    connect(ui->filter2duration, SIGNAL(valueChanged(double)), this, SLOT(applyFilter()));
    connect(ui->filterTask, SIGNAL(textChanged(QString)), this, SLOT(applyFilter()));
    connect(ui->filterUnit, SIGNAL(textChanged(QString)), this, SLOT(applyFilter()));

    connect(ui->splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(resetLabelScenes()));

    connect(ui->centerButton, SIGNAL(pressed()), this, SLOT(center()));

    connect(ui->toggleButton, SIGNAL(pressed()), this, SLOT(toggleBar()));

    //loadData();

    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
    ui->allTasksTable->horizontalHeader()->hideSection(7);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    ui->ganttView->viewport()->installEventFilter(this);

    createMenu();
    reset();

}

//create Menu (File, Edit...)
void MainWindow::createMenu(){
    openCSVAct = new QAction(tr("&Open CSV"),this);
    openCSVAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    openCSVAct->setStatusTip(tr("Load a csvfile"));
    connect(openCSVAct, &QAction::triggered, this, &MainWindow::openCSV);

    openGDXAct = new QAction(tr("&Open GDX..."),this);
    openGDXAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_O));
    openGDXAct->setStatusTip(tr("Load a GDX file with the help of a wizard"));
    connect(openGDXAct, &QAction::triggered, this, &MainWindow::openGDX);



    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openCSVAct);
    fileMenu->addAction(openGDXAct);

    checkAct = new QAction(tr("&Check"),this);
    checkAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    checkAct->setStatusTip(tr("Check the file for overlapping tasks and orrect flow amount sum"));
    connect(checkAct, &QAction::triggered, this, &MainWindow::check);

    checkMenu = menuBar()->addMenu(tr("&Check"));
    checkMenu->addAction(checkAct);

    visualizeamountsAct = new QAction(tr("&Visualise Amounts"),this);
    visualizeamountsAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_V));
    visualizeamountsAct->setCheckable(true);
    visualizeamountsAct->setStatusTip(tr("Visualize amounts in the Gantt Chart, relative to highest amount"));
    connect(visualizeamountsAct, &QAction::changed, this, &MainWindow::visualizeamounts);

    showAllFlowsAct = new QAction(tr("&Show all Flows"), this);
    showAllFlowsAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F));
    showAllFlowsAct->setCheckable(true);
    showAllFlowsAct->setStatusTip(tr("Show all flows, regardless of the current selection"));
    connect(showAllFlowsAct, &QAction::changed, this, &MainWindow::showAllFlowsToggled);

    zoominAct = new QAction(tr("&Zoom in"), this);
    zoominAct->setShortcuts(QKeySequence::ZoomIn);
    zoominAct->setStatusTip(tr("Zoom in, Keeping aspect ratio"));
    connect(zoominAct, &QAction::triggered, this, &MainWindow::v_zoomin);
    connect(zoominAct, &QAction::triggered, this, &MainWindow::h_zoomin);

    zoomoutAct = new QAction(tr("&Zoom out"), this);
    zoomoutAct->setShortcut(QKeySequence::ZoomOut);
    zoomoutAct->setStatusTip(tr("Zoom out, Keeping aspect ratio"));
    connect(zoomoutAct, &QAction::triggered, this, &MainWindow::v_zoomout);
    connect(zoomoutAct, &QAction::triggered, this, &MainWindow::h_zoomout);

    centerAct = new QAction(tr("&Center view"), this);
    centerAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
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
    colorByAmountAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_A));
    colorByAmountAct->setStatusTip(tr("Color tasks relative to their amount, from white over yellow to red"));
    colorByAmountAct->setCheckable(true);
    connect(colorByAmountAct, &QAction::triggered, this, &MainWindow::colorByAmount);

    colorByUnitAct = new QAction(tr("&Color by Unit"), this);
    colorByUnitAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_U));
    colorByUnitAct->setStatusTip(tr("Color tasks like their unit"));
    colorByUnitAct->setCheckable(true);
    connect(colorByUnitAct, &QAction::triggered, this, &MainWindow::colorByUnit);

    colorByTaskAct = new QAction(tr("&Color by Task"), this);
    colorByTaskAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T));
    colorByTaskAct->setStatusTip(tr("Color tasks with same name in same color"));
    colorByTaskAct->setCheckable(true);
    connect(colorByTaskAct, &QAction::triggered, this, &MainWindow::colorByTask);

    colorByColorAct = new QAction(tr("&Color by Preset"), this);
    colorByColorAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));
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

    exportAct = new QAction(tr("&Export..."));
    exportAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    exportAct->setToolTip(tr("Launch export wizard"));
    connect(exportAct, &QAction::triggered, this, &MainWindow::startExportWiz);


    exportMenu = menuBar()->addMenu(tr("&Export"));
    exportMenu->addAction(exportAct);

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
    largestAmounts = new QList<float>();
    longestTask = 0;
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

    nameLabelScene = new QGraphicsScene(ui->labelView->frameRect());
    rulerLabelScene = new QGraphicsScene(ui->rulerView->frameRect());

    ui->labelView->setScene(nameLabelScene);
    ui->rulerView->setScene(rulerLabelScene);
    ui->labelView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->labelView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->rulerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->rulerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->labelView->setSceneRect(nameLabelScene->sceneRect());
    ui->rulerView->setSceneRect(rulerLabelScene->sceneRect());
    nameLabelScene->setBackgroundBrush(QBrush(QWidget::palette().color(QWidget::backgroundRole())));
    rulerLabelScene->setBackgroundBrush(QBrush(QWidget::palette().color(QWidget::backgroundRole())));
    ui->labelView->setStyleSheet( "QGraphicsView { border-style: none; }" );
    ui->rulerView->setStyleSheet( "QGraphicsView { border-style: none; }" );
    //ui->labelView->setAlignment(Qt::AlignLeft | Qt::AlignRight);
    //ui->rulerView->setAlignment(Qt::AlignLeft | Qt::AlignRight);
    resetLabelScenes();
}

//DATA stuff
void MainWindow::openCSV(){
    filedialogopen = true;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open csv"), DEFAULTDIRECTORY, "CSV files (*.csv)");
    filedialogopen = false;
    if ( fileName == ""){
        return;
    }

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

        } else if (lineContents.count() == 3){
            //Task or unit color
            line.replace(" ", "");
            lineContents = line.split(";",QString::SkipEmptyParts);
            QColor color = QColor();
            color.setNamedColor(lineContents.at(2));

            if (lineContents[0] == "TASKCOLOR"){
                qDebug("Task %s has manually been assigned the color %s", lineContents.at(1).toStdString().c_str(), lineContents.at(1).toStdString().c_str());
                taskColors->insert(lineContents.at(1), color);
            } else {
                qDebug("Unit %s has manually been assigned the color %s", lineContents.at(1).toStdString().c_str(), lineContents.at(1).toStdString().c_str());
                unitColors->insert(lineContents.at(1), color);
            }
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
    createRepresentation();

}
void MainWindow::openGDX(){

    Wizard * wiz = new Wizard(this);
    connect(wiz, SIGNAL(addFlow(QString,QString,QString,QString,float,float,float)), this, SLOT(addFlow(QString,QString,QString,QString,float,float,float)));
    connect(wiz, SIGNAL(addTask(QString,QString,float,float,float,QString)), this, SLOT(addTask(QString,QString,float,float,float,QString)));
    connect(wiz, SIGNAL(reset()), this, SLOT(reset()));
    connect(wiz, SIGNAL(createRepresentation()), this, SLOT(createRepresentation()));
    filedialogopen = true;
    wiz->show();
    filedialogopen = false;
    if (wiz->loadedFile != ""){
        openFile = wiz->loadedFile;
    }
}




//add new task
void MainWindow::addTask(QString unitName, QString taskName, float start, float end, float amount, QString color){

    Task newTask;

    int machineIndex = units->indexOf(unitName);
    if (machineIndex == -1){
        units->append(unitName);
        largestAmounts->append(amount);
        machineIndex = units->size()-1;
    } else {
        if (amount > largestAmounts->at(machineIndex)){
            largestAmounts->replace(machineIndex, amount);
        }
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
        if (taskColors->contains(taskName)){
            newTask.color = taskColors->value(taskName);
        } else if (unitColors->contains(unitName)){
            newTask.color = unitColors->value(unitName);
        } else {
            newTask.color.setHsv(qrand()%255, qrand()%128+127, qrand()%128+127);
        }
    } else{
        newTask.color.setNamedColor(color);
        // Set the unit and task colors
        if (!unitColors->contains(units->at(machineIndex))){
            //qDebug("Unit %s has automatically been assigned the color %s", unitName.toStdString().c_str(), color.toStdString().c_str());
            unitColors->insert(units->at(machineIndex), newTask.color);
        }
        if (!taskColors->contains(taskName)){
            //qDebug("%s has automatically been assigned the color %s", taskName.toStdString().c_str(), color.toStdString().c_str());
            taskColors->insert(taskName, newTask.color);
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
    //Seed qsrand for random colors
    qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));
    //populate UnitColors with random colors
    for (int i = 0; i < units->size(); i++){
        if (!unitColors->contains(units->at(i))){
            QColor rancol = QColor();
            rancol.setHsv(qrand()%255, qrand()%128+127, qrand()%128+127);
            unitColors->insert(units->at(i), rancol);
        }
    }
    //Draw Labels

    for (int i = 0; i < units->size(); i++){
        QGraphicsTextItem * unitLabel = new QGraphicsTextItem();
        unitLabel->setHtml(QString("<div style='background:rgba(255, 255, 255, 50%);'>" +units->at(i) + "</div>") );
        unitLabel->setDefaultTextColor(Qt::black);
        if (unitLabel->boundingRect().width() > 0){
            UNITNAMEBARWIDTH = unitLabel->boundingRect().width();
        }

        unitLabel->setZValue(10);
        unitLabel->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        unitLabel->hide();
        scene->addItem(unitLabel);
        unitLabels->append(unitLabel);
    }
    for (int i = 0; i < units->size(); i++){
        QTransform n;
        n.translate(-unitLabels->at(i)->boundingRect().width(),0);
        unitLabels->at(i)->setTransform(n);
        unitLabels->at(i)->setPos(-UNITNAMEBARWIDTH,(i+1)*UNITREPHEIGHT-unitLabels->at(i)->boundingRect().height()/2);
    }

    //Draw Lines
    QPen unitPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    unitPen.setWidth(0);
    for (int i = 0; i < units->size(); i++){
        QGraphicsLineItem* line = new QGraphicsLineItem(0,(i+1)*UNITREPHEIGHT,longestTask + 0,(i+1)*UNITREPHEIGHT);
        line->setPen(unitPen);
        scene->addItem(line);

    }
    //Draw Tasks
    for (int i = 0; i < tasks->size(); i++){
        Task task = tasks->at(i);
        GanttRect * newRect = new GanttRect(task.taskIndex,task.unitIndex,largestAmounts->at(task.unitIndex),task.start,task.end,task.amount, 0, task.color);
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
        int b = 0;
        int e = 0;
        for (int f = 0; f < flows->size(); f++){
            if (flows->at(f).op1Index == i){
                e++;
            }
            if (flows->at(f).op2Index == i){
                b++;
            }

        }
        QString type;
        if (b == 0 && e == 0){
            type = "Single";
        } else if (b == 0 && e > 0){
            type = "First";
        } else if (e == 0 && b > 0){
            type = "Final";
        } else {
            type = "Middle";
        }

        ui->allTasksTable->setItem(i,6,new QTableWidgetItem(type));
        ui->allTasksTable->setItem(i,7,new QTableWidgetNumberItem(i));

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


    showFlows(showAllFlows);


    scene->setSceneRect(0,0,longestTask + 0, (UNITREPHEIGHT + 2) * units->length());
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
    resetLabelScenes();
}
void MainWindow::resetLabelScenes(){

    nameLabelScene->clear();
    rulerLabelScene->clear();
    //nameLabelScene->setSceneRect(0, 0, ui->labelView->width(), ui->labelView->height());
    //rulerLabelScene->setSceneRect(0, 0, ui->rulerView->width(), ui->rulerView->height());
    //ui->labelView->fitInView(nameLabelScene->sceneRect(), Qt::IgnoreAspectRatio);
    //ui->rulerView->fitInView(rulerLabelScene->sceneRect(), Qt::IgnoreAspectRatio);

    unitLabels = new QList<QGraphicsTextItem*>();
    rulerLabels = new QList<QGraphicsTextItem*>();
    nameLabelScene->clear();
    rulerLabelScene->clear();
    for (int i = 0; i < units->size(); i++){
        QGraphicsTextItem * unitLabel = new QGraphicsTextItem();
        unitLabel->setPos(unitLabel->boundingRect().width(),0);
        unitLabel->setPlainText(units->at(i));
        nameLabelScene->addItem(unitLabel);
        unitLabels->append(unitLabel);
    }
    // remove old ruler lines
    for (int i = 0; i< lines->size(); i++){
        scene->removeItem(lines->at(i));
    }
    lines->clear();


    QPen rulerPen(Qt::lightGray, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    //number of ruler labels
    int n = ui->rulerView->width()/RULERLABELWIDTH;
    //top left corner
    QPointF tl = ui->rulerView->mapToScene(0,0);
    for (int i = 0; i < n; i++){
        QGraphicsTextItem * rulerLabel = new QGraphicsTextItem();
        float r = tl.x() + i * RULERLABELWIDTH;
        float p = ui->ganttView->mapToScene(ui->rulerView->mapFromScene(QPoint(r,0))).x();

        QGraphicsLineItem* line = new QGraphicsLineItem();
        lines->append(line);
        line->setPen(rulerPen);
        line->setZValue(-10);
        scene->addItem(line);
        rulerLabel->setPos(r,0);
        rulerLabelScene->addItem(rulerLabel);
        rulerLabels->append(rulerLabel);

        //unitLabel->setHtml(QString("<div style='background:rgba(255, 255, 255, 50%);'>" +QString::number(p) + "</div>") );
        rulerLabel->setDefaultTextColor(Qt::black);

    }

    doLabels();
}

void MainWindow::doLabels(){
    if (filedialogopen){ //this was called when a file dialog was scrolled and not the ganttview, wrong!
        return;
    }
    // move name labels
    for (int i = 0; i < units->size(); i++){
        unitLabels->at(i)->setPos(unitLabels->at(i)->pos().x(), ui->labelView->mapToScene(ui->ganttView->mapFromScene(QPoint(0,(i)*UNITREPHEIGHT))).y());
    }
    //upper line point
    float up = ui->ganttView->mapToScene(0,0).y();
    //lower line point
    float dp = ui->ganttView->mapToScene(0,ui->ganttView->height()).y();


    // move ruler lines, rename ruler labels
    for (int i = 0; i < lines->size(); i++){


        float p = ui->ganttView->mapToScene(ui->rulerView->mapFromScene(rulerLabels->at(i)->pos())).x();
        lines->at(i)->setLine(p, up, p, dp);
        rulerLabels->at(i)->setPlainText(QString::number(p,'f',1));
    }


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
    log += "\nChecking for overlapping tasks\n";
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
        if (receivedInput != expectedInput && inFlows.length() > 0 && outFlows.length() > 0){
            flowerrors++;
            log += "\n Unmatched amounts on \n " + describeTask(tasks->at(i)) + "\n Sum of Flows:\n";
            for (int f = 0; f < inFlows.size(); f++){
                log += "\nFrom " + tasks->at(flows->at(inFlows.at(f)).op1Index).name + "\n\t\t\t" + QString::number(flows->at(inFlows.at(f)).amount) ;
            }
            //if (inFlows.length() > 1){
            log += "\n\t\t=\t" + QString::number(receivedInput) + "\n";
            //}
            for (int f = 0; f < outFlows.size(); f++){
                log += "\nTo " + tasks->at(flows->at(outFlows.at(f)).op2Index).name + "\n\t\t\t" + QString::number(flows->at(outFlows.at(f)).amount);
            }
            //if (outFlows.length() > 1){
            log += "\n\t\t=\t" + QString::number(expectedInput) + "\n";
            //}

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
        float x = 2;
        if (largestAmounts->at(tasks->at(i).unitIndex) != 0){
            x = tasks->at(i).amount / largestAmounts->at(tasks->at(i).unitIndex);
        }
        QColor c = QColor(Qt::gray);
        // largestamount = 0, color black
        if (x == 0){

        } else if (x < 0.5){ // white to yellow
            c.setHsv(42.5,x*512,255);
        } else if (x >=0.5) { //yellow to red
            c.setHsv(85-85*x,255,255);
        }
        tasksRep->at(i)->setBrush(c);
    }
}

void MainWindow::taskHoverEnter(int index){
    Task t = tasks->at(index);
    ui->statusBar->showMessage("Name: " + t.name
                               + " Unit: " + units->at(t.unitIndex)
                               + " Start: " + QString::number(t.start)
                               + " End: " + QString::number(t.end)
                               + " Duration: " + QString::number(t.end-t.start)
                               + " Amount: " + QString::number(t.amount));

    /*QToolTip::showText(mapToGlobal(tasksRep->at(index)->pos().toPoint()),
                       "Name: " + t.name
                       + " Unit: " + units->at(t.unitIndex)
                       + " Start: " + QString::number(t.start)
                       + " End: " + QString::number(t.end)
                       + " Duration: " + QString::number(t.end-t.start)
                       + " Amount: " + QString::number(t.amount),
                       this, tasksRep->at(index)->boundingRect().toRect(),10000000
                       );*/

}
void MainWindow::taskHoverLeave(int index){
    //ui->hoverTaskLabel->setText("");
    //QToolTip::hideText();
}

//Task Clicked
void MainWindow::taskClicked(int index){
    for (int i = 0; i < tasks->count(); i++){
        if (ui->allTasksTable->item(i,7)->text().toInt() == index){
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
    int realIndex = ui->allTasksTable->item(row, 7)->text().toInt();
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
        Task task = tasks->at(ui->allTasksTable->item(i,7)->text().toInt());
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
            alive &= task.amount < ui->filter1amount->value() || task.amount > ui->filter2amount->value();
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
            alive &= task.start < ui->filter1start->value() || task.start > ui->filter2start->value();
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
            alive &= task.end < ui->filter1end->value() || task.end > ui->filter2end->value();
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
            alive &= task.end - task.start < ui->filter1duration->value() || task.end - task.start > ui->filter2duration->value();
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
    doLabels();
}
void MainWindow::scrolle(){
    ui->ganttView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->ganttView->translate(-SCROLLAMOUNT, 0);
    ui->ganttView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    doLabels();
}
void MainWindow::scrolls(){
    ui->ganttView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->ganttView->translate(0,-SCROLLAMOUNT);
    ui->ganttView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    doLabels();
}
void MainWindow::scrollw(){
    ui->ganttView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->ganttView->translate(SCROLLAMOUNT, 0);
    ui->ganttView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    doLabels();
}

//Zoom
void MainWindow::v_zoomin(){
    ui->ganttView->scale(1,2);
    doLabels();
}
void MainWindow::v_zoomout(){
    ui->ganttView->scale(1,0.5);
    doLabels();
}
void MainWindow::h_zoomin(){
    ui->ganttView->scale(2,1);
    doLabels();
}
void MainWindow::h_zoomout(){
    ui->ganttView->scale(0.5,1);
    doLabels();
}
void MainWindow::verticalZoomSliderChanged(int x){
    resetZoom();
    verticalZoom = qPow(SCALERANGE, (x+1)/50.0-1.0);
    ui->ganttView->scale(horizontalZoom, verticalZoom);
    qDebug("%f,%f",horizontalZoom,verticalZoom);
    doLabels();
}
void MainWindow::horizontalZoomSliderChanged(int x){
    resetZoom();
    horizontalZoom = qPow(SCALERANGE, (x+1)/50.0-1.0);
    ui->ganttView->scale(horizontalZoom, verticalZoom);
    qDebug("%f,%f",horizontalZoom,verticalZoom);
    doLabels();
}
void MainWindow::resetZoom(){

    ui->ganttView->scale(1/horizontalZoom, 1/verticalZoom);
    doLabels();

}
void MainWindow::center(){
    ui->verticalSlider->setValue(50);
    ui->horizontalSlider->setValue(50);
    verticalZoom = 1;
    horizontalZoom = 1;
    //ui->ganttView->scale();
    ui->ganttView->fitInView(scene->sceneRect());
    doLabels();
}
void MainWindow::toggleBar(){
    QList<int> currentSizes = ui->splitter->sizes();
    // adjust sizes individually here, e.g.


    if (currentSizes[0] < 2) {
        currentSizes[0] = ui->tabWidget->minimumWidth();

    } else {
        currentSizes[0] = 0;
    }
    ui->splitter->setSizes(currentSizes);
    resetLabelScenes();

}

void MainWindow::wheelEvent(QWheelEvent *e){

    if (QApplication::keyboardModifiers().testFlag(Qt::KeyboardModifier::ControlModifier)){
        ui->verticalSlider->setValue(ui->verticalSlider->value()+e->delta()/100);
        ui->horizontalSlider->setValue(ui->horizontalSlider->value()+e->delta()/100);
    }


}
bool MainWindow::eventFilter(QObject *watched, QEvent *event){

    if (watched == ui->ganttView->viewport() && event->type() == QEvent::Wheel && QApplication::keyboardModifiers().testFlag(Qt::KeyboardModifier::ControlModifier)){
        doLabels();
        return true;
    }

    if ((event->type()!=QEvent::Paint) && (event->type()!=QEvent::MouseMove) && (event->type()!=QEvent::Resize)){// || event->type()!=QEvent::MouseMove || event->type()!=QEvent::Leave || event->type()!=QEvent::WindowDeactivate){
        qDebug()<<event->type();
        doLabels();
    }

    return false;
}

void MainWindow::resizeEvent(QResizeEvent * event)
{

    //fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    //
    resetLabelScenes();

};
// Export

void MainWindow::startExportWiz(){
    if (openFile == ""){
        return;
    }
    wiz = new ExportWizard(this);
    //connect(wiz,SIGNAL(finished())), this, SLOT(exportWiz());
    connect(wiz, SIGNAL(finished()), this, SLOT(exportWiz()));
    wiz->show();
}

void MainWindow::exportWiz(){
    double vscale = wiz->vscale;
    int rulerInterval = wiz->interval;
    qDebug()<<vscale;

    QGraphicsScene * printScene = new QGraphicsScene(QRectF(0,0,rulerInterval*(longestTask/rulerInterval + 1)*vscale + UNITNAMEBARWIDTH, (UNITREPHEIGHT + 2) * units->length()));
    QPen rulerPen(Qt::lightGray, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    for (int i = 0; i < longestTask/rulerInterval + 1; i++){

        QGraphicsTextItem * rulerlabel = new QGraphicsTextItem();
        rulerlabel->setHtml(QString("<div style='background:rgba(255, 255, 255, 100%);'>" + QString::number(i*rulerInterval) + "</div>") );
        rulerlabel->setDefaultTextColor(Qt::black);
        rulerlabel->setPos(i*rulerInterval*vscale + UNITNAMEBARWIDTH,UNITREPHEIGHT * (units->size()+1));
        //rulerlabel->setTextWidth(UNITNAMEBARWIDTH);
        printScene->addItem(rulerlabel);
        QGraphicsLineItem* line = new QGraphicsLineItem(i*rulerInterval*vscale + UNITNAMEBARWIDTH,0,i*rulerInterval*vscale + UNITNAMEBARWIDTH,(units->length()+1)*UNITREPHEIGHT);

        line->setPen(rulerPen);
        printScene->addItem(line);


    }
    //Draw Lines
    QPen unitPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    unitPen.setWidth(0);
    for (int i = 0; i < units->size(); i++){
        QGraphicsLineItem* line = new QGraphicsLineItem(0,(i+1)*UNITREPHEIGHT,rulerInterval*(longestTask/rulerInterval + 2)*vscale + UNITNAMEBARWIDTH,(i+1)*UNITREPHEIGHT);
        line->setPen(unitPen);
        printScene->addItem(line);
    }
    QList<GanttRect*> taskList = QList<GanttRect*>();
    //Draw Tasks
    for (int i = 0; i < tasks->size(); i++){
        Task task = tasks->at(i);
        GanttRect * newRect = new GanttRect(task.taskIndex,task.unitIndex,largestAmounts->at(task.unitIndex),task.start*vscale,task.end*vscale,task.amount, UNITNAMEBARWIDTH, task.color);
        if (tasksRep->at(i)->isVisible() || wiz->filter){

            QColor c = QColor(Qt::gray);

            switch (wiz->color) {
            case 0: //default
                c = tasks->at(i).color;
                break;
            case 1: //task
                c = taskColors->value(tasks->at(i).name);
                break;
            case 2: //unit
                c = unitColors->value(units->at(tasks->at(i).unitIndex));
                break;
            default: //amount
                float x = 2;
                if (largestAmounts->at(tasks->at(i).unitIndex) != 0){
                    x = tasks->at(i).amount / largestAmounts->at(tasks->at(i).unitIndex);
                }
                if (x == 0){

                } else if (x < 0.5){ // white to yellow
                    c.setHsv(42.5,x*512,255);
                } else if (x >=0.5) { //yellow to red
                    c.setHsv(85-85*x,255,255);
                }
                break;
            }
            newRect->setBrush(c);
            printScene->addItem(newRect);
            if (wiz->labels){
                QGraphicsTextItem * label = new QGraphicsTextItem();

                label->setHtml(QString("<div style='background:rgba(255, 255, 255, 50%);'>" + task.name + "</div>") );
                label->setDefaultTextColor(Qt::black);

                label->setZValue(5);

                printScene->addItem(label);
                //label->setPos(newRect->pos());

                label->setPos(newRect->boundingRect().topLeft());

            }
            taskList.append(newRect);
            //rulerlabel->setTextWidth(UNITNAMEBARWIDTH);
        }
    }
    //Draw Flows

    for (int i = 0; i < flows->size(); i++){
        if (wiz->allFlows || flowsRep->at(i)->isVisible()){
            Flow flow = flows->at(i);
            GanttFlow * newLine = new GanttFlow(i,
                                                QPolygonF(QVector<QPointF>()
                                                          << taskList.at(flow.op1Index)->boundingRect().topRight()
                                                          << taskList.at(flow.op1Index)->boundingRect().bottomRight()
                                                          << taskList.at(flow.op2Index)->boundingRect().bottomLeft()
                                                          << taskList.at(flow.op2Index)->boundingRect().topLeft()));
            printScene->addItem(newLine);
        }
    }
    //Draw Labels
    for (int i = 0; i < units->size(); i++){
        QGraphicsTextItem * unitLabel = new QGraphicsTextItem();
        unitLabel->setHtml(QString("<div style='background:rgba(255, 255, 255, 100%);'>" +units->at(i) + "</div>") );
        unitLabel->setDefaultTextColor(Qt::black);
        //unitLabel->setTextWidth(UNITNAMEBARWIDTH);
        unitLabel->setPos(UNITNAMEBARWIDTH-unitLabel->boundingRect().width(),(i+1)*UNITREPHEIGHT-unitLabel->boundingRect().height()/2);
        printScene->addItem(unitLabel);
    }

    //showFlows(showAllFlows);

    QString cutname = "";
    for (int i = 0; i < openFile.split(".",QString::SkipEmptyParts).size() - 1; i++){
        if (i > 0){
            cutname += ".";
        }
        cutname += openFile.split(".",QString::SkipEmptyParts).at(i);
    }
    if (wiz->format == "PDF"){

        QString filename = QFileDialog::getSaveFileName(this,"Save as",cutname+".pdf", "PDF file (*.pdf)");
        if (filename == ""){
            return;
        }

        QPrinter printer( QPrinter::HighResolution );
        printer.setPageSizeMM(QSizeF(50*(longestTask/50 + 1)*vscale + UNITNAMEBARWIDTH, (UNITREPHEIGHT + 2) * units->length()));
        printer.setOrientation( QPrinter::Portrait );
        printer.setOutputFormat( QPrinter::PdfFormat );

        printer.setOutputFileName( filename ); // file will be created in your build directory (where debug/release directories are)

        QPainter p;

        if( !p.begin( &printer ) )
        {
            qDebug() << "Error!";
            return;
        }
        printScene->render( &p );
        p.end();
    } else {
        QString filename = QFileDialog::getSaveFileName(this,"Save as",cutname+".png", "Image files (*.png *.jpg *bmp)");
        if (filename == ""){
            return;
        }
        //QGraphicsScene * printScene = new QGraphicsScene(QRectF(0,0,rulerInterval*(longestTask/rulerInterval + 1)*vscale + UNITNAMEBARWIDTH, (UNITREPHEIGHT + 2) * units->length()));
        QGraphicsView * printView = new QGraphicsView(printScene, this);
        printView->setMinimumSize(rulerInterval*(longestTask/rulerInterval + 1)*vscale + UNITNAMEBARWIDTH, (UNITREPHEIGHT + 2) * units->length());

        QPixmap pixMap = QPixmap::grabWidget(printView);
        pixMap.save(filename);

    }

    printScene->clear();

}

//Pan
void MainWindow::keyPressEvent(QKeyEvent *e){
    if (e->key() == Qt::SHIFT){
        ui->ganttView->setDragMode(QGraphicsView::ScrollHandDrag);
    }else {
        //MainWindow::keyPressEvent(e);
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent *e){
    if (e->key() == Qt::SHIFT){
        ui->ganttView->setDragMode(QGraphicsView::NoDrag);
    } else {
        //MainWindow::keyPressEvent(e);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
