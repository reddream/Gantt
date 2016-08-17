#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QActionGroup>
#include "ganttflow.h"
#include "ganttrect.h"
/// representation of a task
struct Task{
    /// Task name
    QString name;
    /// The index of the unit the Task is on, in the MainWindow::units list
    int unitIndex;
    /// The index of this task in the MainWindow::tasks list
    int taskIndex;
    /// Task start time
    float start;
    /// Task end time
    float end;
    /// Task amount
    float amount;
    /// The color of this task as defined in the input file. If no color is given, this will be set to a random color in MainWindow::addTask
    QColor color;
};
/// representation of a flow
struct Flow{
    /// The index of this flow in the MainWindow::flows list.
    int flowIndex;
    /// The index of the unit of the first Task in the MainWindow::units list
    int unit1Index = -1;
    /// The index of the unit of the second Task in the MainWindow::units list
    int unit2Index = -1;
    /// The index of the first task in the MainWindow::tasks list
    int op1Index = -1;
    /// The index of the second task in the MainWindow::tasks list
    int op2Index = -1;
    /// The amount of this flow
    float amount;
    /// The production rate of this flow
    float productionRate;
    /// The consumption rate of this flow
    float consumptionRate;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MainWindow *ui;
    float verticalZoom = 1;
    float horizontalZoom = 1;
    void resetZoom();
    /// Recursively make all flows connected to this task visible
    void showFlows(int taskIndex);
    void recursivelyActivateFlowsToTheRight(int taskIndex);
    void recursivelyActivateFlowsToTheLeft(int taskIndex);
    /// Creates the graphical representation of the gantt chart
    void createRepresentation();
    /// A list of all tasks
    QList<Task> * tasks;
    /// A list of all flows
    QList<Flow> * flows;
    /// A list of all unit names
    QStringList * units;
    /// A map of unit colors
    QMap<QString, QColor> * unitColors;
    /// A map of task colors
    QMap<QString, QColor> * taskColors;
    ///Graphic representations of each task
    QList<GanttRect*> * tasksRep;
    ///Graphic representations of each flow
    QList<GanttFlow*> * flowsRep;
    ///A list of the vertical lines for each unit
    QList<QGraphicsLineItem*> * lines;
    ///A list of the labels of each unit
    QList<QGraphicsTextItem*> * unitLabels;
    /// The scene to which everything is drawn
    QGraphicsScene * scene;
    /// read the Sysdir from the config file
    QString getSysdir();
    /// Adds a Flow to the flows list
    void addFlow(QString unit1name, QString op1name, QString unit2name, QString op2name, float pr, float cr, float amount);
    /// Adds a Task to the tasks list. color is a color name in hex format, if none is given a random color will be generated.
    void addTask(QString unitName, QString taskName, float start, float end, float amount, QString color = "random");
    /// The time when the longest task ends.
    float longestTask;
    float largestAmount;
    /// Set wether to visualize amounts by height
    void showAmount(bool yes);
    int currentClickedTaskIndex = -1;
    int currentClickedFlowIndex = -1;
    /// select a Task in the Table by index
    void selectTaskOnTable(int index);
    ///-2 = hide all flows, -1 = show all flows by default
    int showAllFlows = -2;
    /// creates the menu bar
    void createMenu();
    /// returns a readable description of a Task
    QString describeTask(Task task);
    ///Directory in which the open file prompt will start in
    QString defaultDirectory = "/Users/juliusnaeumann/Downloads/examples-CSV";
    /// The currently opened file
    QString openFile;
    /// parse a CSV file
    void openCSV(QString filename);
    /// read a GDX file
    void openGDX(QString filename);
    /// File menu
    QMenu * fileMenu;
    /// Check menu
    QMenu * checkMenu;
    /// Export menu
    QMenu * exportMenu;
    /// View menu
    QMenu * viewMenu;
    /// Task coloring submenu
    QActionGroup * taskColoringMenu;
    //Menu Actions
    /// Menu action for opening a new file
    QAction *  openAct;
    /// Menu action for resetting the view
    QAction *  resetAct;
    /// Menu action for  selecting GAMS Sysdir
    QAction * selectSysdirAct;
    /// Menu action for visualizing amounts
    QAction *  visualizeamountsAct;
    /// Menu action for showing all flows
    QAction * showAllFlowsAct;
    /// Menu action for zooming in
    QAction *  zoominAct;
    /// Menu action for zooming out
    QAction *  zoomoutAct;
    /// Menu action for scrolling up
    QAction *  scrollnAct;
    /// Menu action for scrolling right
    QAction *  scrolleAct;
    /// Menu action for scrolling down
    QAction *  scrollsAct;
    /// Menu action for scrolling left
    QAction *  scrollwAct;
    /// Menu action for centering the view to fit all content
    QAction *  centerAct;
    /// Menu action for coloring each task relative to the amount
    QAction *  colorByAmountAct;
    /// Menu action for coloring each task by unit
    QAction *  colorByUnitAct;
    /// Menu action for coloring each task by preset color;
    QAction * colorByColorAct;
    /// Menu action for coloring each task by task color;
    QAction * colorByTaskAct;
    /// Menu action for checking
    QAction *  checkAct;
    QAction *  exportAct;

private slots:
    /// called when  the vertical zoom slider changed
    void verticalZoomSliderChanged(int x);
    /// called when  the horizontal zoom slider changed
    void horizontalZoomSliderChanged(int x);
    /// zoom in vertically by 2x
    void v_zoomin();
    /// zoom out vertically by 2x
    void v_zoomout();
    /// zoom in horizontally by 2x
    void h_zoomin();
    /// zoom out horizontally by 2x
    void h_zoomout();
    /// called when a task ( its graphical representation as a GanttRect ) has been clicked
    void taskClicked(int index);
    /// called when a task ( its graphical representation as a GanttRect ) has started being hovered on
    void taskHoverEnter(int index);
    /// called when a task ( its graphical representation as a GanttRect ) has stopped being hovered on
    void taskHoverLeave(int index);
    /// called when a flow ( its graphical representation as a GanttFlow ) has been clicked
    void flowClicked(int index);
    /// called when a task has been selected in the table
    void taskTableClicked(int index, int _);
    //Menu
    /// called by openAct, opens a File selection window. Recognizes wether a CSV or a CSV file was opened and calls openCSV() and openGDX(),  respectively.
    void open();
    /// reset the window
    void reset();
    /// called by selectSysdirAct, writes a new Sysdir to the config file
    void selectSysdir();
    /// called by visualizeAmountsAct
    void visualizeamounts();
    /// called by showAllFlowsAct
    void showAllFlowsToggled();
    void scrolln();
    void scrolle();
    void scrolls();
    void scrollw();
    void center();
    void check();
    /// Save log ouput to file
    void saveLogs();
    void colorByAmount();
    void colorByUnit();
    void colorByColor();
    void colorByTask();
    void exportToImage();
    // filter slots
    void ruleChangedAmount(int index);
    void ruleChangedDuration(int index);
    void ruleChangedStart(int index);
    void ruleChangedEnd(int index);
    void applyFilter();
    void clearFilter();
protected:
    void resizeEvent(QResizeEvent * event);
    void keyPressEvent(QKeyEvent * e);
    void keyReleaseEvent(QKeyEvent * e);
    void wheelEvent(QWheelEvent * e);
};

#endif // MAINWINDOW_H
