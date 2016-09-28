#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QActionGroup>
#include "ganttflow.h"
#include "ganttrect.h"
#include "wizard.h"
#include "exportwizard.h"
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
/// Ui namespace, containing the MainView and Wizard classes
namespace Ui {
class MainWindow;
}
/// Class for the Main window
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
    /// wether the file dialog is open (to not redo the labels if scrolling inside filedialog)
    bool filedialogopen = false;
    /// resets zoom
    void resetZoom();
    /// width of the unit name bar, dynamically set to width of longest label.
    int UNITNAMEBARWIDTH = 0;
    /// Recursively make all flows connected to this task visible
    void showFlows(int taskIndex);
    void recursivelyActivateFlowsToTheRight(int taskIndex);
    void recursivelyActivateFlowsToTheLeft(int taskIndex);
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
    /// The export wizard
    ExportWizard * wiz;
    ///Graphic representations of each task
    QList<GanttRect*> * tasksRep;
    ///Graphic representations of each flow
    QList<GanttFlow*> * flowsRep;
    ///A list of the vertical lines for each unit
    QList<QGraphicsLineItem*> * lines;
    ///A list of the ruler labels
    QList<QGraphicsTextItem*> * rulerLabels;
    ///A list of the labels of each unit
    QList<QGraphicsTextItem*> * unitLabels;
    /// The scene to which everything is drawn
    QGraphicsScene * scene;
    /// Scene for name labels
    QGraphicsScene * nameLabelScene ;
    /// Scene for ruler labels
    QGraphicsScene * rulerLabelScene;
    /// The time when the longest task ends.
    float longestTask;
    /// The largest amount (For scaling the view)
    float largestAmount;
    /// Largest amount for each task, for the coloring option
    QList<float> * largestAmounts;
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
    /// The currently opened file
    QString openFile;
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
    QAction *  openCSVAct;
    /// Menu action for resetting the view
    QAction *  openGDXAct;
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
    /// Menu action for exporting
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
    /// called by openCSVAct, opens a File selection window and parses the selected CSV file.
    void openCSV();
    /// called by openGDXAct, opens a Wizard for reading GDX files.
    void openGDX();
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
    void startExportWiz();
    void exportWiz();
    // filter slots
    void ruleChangedAmount(int index);
    void ruleChangedDuration(int index);
    void ruleChangedStart(int index);
    void ruleChangedEnd(int index);
    void applyFilter();
    void clearFilter();
    // public functions for the csv Wizard
    /// Creates the graphical representation of the gantt chart
    void createRepresentation();
    /// Adds a Flow to the flows list
    void addFlow(QString unit1name, QString op1name, QString unit2name, QString op2name, float pr, float cr, float amount);
    /// Adds a Task to the tasks list. color is a color name in hex format, if none is given a random color will be generated.
    void addTask(QString unitName, QString taskName, float start, float end, float amount, QString color = "random");
    /// reset the window (important for loading new files)
    void reset();
    /// Setup the label views
    void doLabels();
    /// reset the label views (when window was resized)
    void resetLabelScenes();
    /// toggle the table sidebar
    void toggleBar();
protected:
    void resizeEvent(QResizeEvent * event);
    void keyPressEvent(QKeyEvent * e);
    void keyReleaseEvent(QKeyEvent * e);
    void wheelEvent(QWheelEvent * e);
};

#endif // MAINWINDOW_H
