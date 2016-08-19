/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *Sidebar;
    QTableWidget *tableWidget;
    QTabWidget *tabWidget;
    QWidget *tasksTab;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *allTasksTable;
    QWidget *filterTab;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLabel *label_6;
    QDoubleSpinBox *filter1start;
    QLineEdit *filterTask;
    QDoubleSpinBox *filter2end;
    QComboBox *ruleEnd;
    QLabel *label_8;
    QDoubleSpinBox *filter2duration;
    QDoubleSpinBox *filter2start;
    QDoubleSpinBox *filter1amount;
    QDoubleSpinBox *filter2amount;
    QDoubleSpinBox *filter1duration;
    QComboBox *ruleStart;
    QLabel *label_4;
    QLineEdit *filterUnit;
    QDoubleSpinBox *filter1end;
    QLabel *label_9;
    QComboBox *ruleDuration;
    QLabel *label_3;
    QComboBox *ruleAmount;
    QPushButton *filterApplyButton;
    QPushButton *filterClearButton;
    QLabel *tableCount;
    QWidget *errorsTab;
    QVBoxLayout *verticalLayout;
    QTextBrowser *errorsViewer;
    QPushButton *saveButton;
    QHBoxLayout *V_Zoom;
    QLabel *label_2;
    QPushButton *v_zoom_out;
    QSlider *verticalSlider;
    QPushButton *v_zoom_in;
    QHBoxLayout *H_Zoom;
    QLabel *label;
    QPushButton *h_zoom_out;
    QSlider *horizontalSlider;
    QPushButton *h_zoom_in;
    QGraphicsView *ganttView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1072, 641);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        Sidebar = new QVBoxLayout(layoutWidget);
        Sidebar->setSpacing(6);
        Sidebar->setContentsMargins(11, 11, 11, 11);
        Sidebar->setObjectName(QStringLiteral("Sidebar"));
        Sidebar->setContentsMargins(0, 0, 0, 0);
        tableWidget = new QTableWidget(layoutWidget);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        if (tableWidget->rowCount() < 6)
            tableWidget->setRowCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setItem(0, 0, __qtablewidgetitem);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setMaximumSize(QSize(16777215, 104));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setShowGrid(false);
        tableWidget->setWordWrap(false);
        tableWidget->setRowCount(6);
        tableWidget->setColumnCount(2);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->horizontalHeader()->setHighlightSections(true);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setDefaultSectionSize(17);
        tableWidget->verticalHeader()->setMinimumSectionSize(17);

        Sidebar->addWidget(tableWidget);

        tabWidget = new QTabWidget(layoutWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tasksTab = new QWidget();
        tasksTab->setObjectName(QStringLiteral("tasksTab"));
        verticalLayout_2 = new QVBoxLayout(tasksTab);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        allTasksTable = new QTableWidget(tasksTab);
        if (allTasksTable->columnCount() < 8)
            allTasksTable->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        allTasksTable->setHorizontalHeaderItem(0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        allTasksTable->setHorizontalHeaderItem(1, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        allTasksTable->setHorizontalHeaderItem(2, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        allTasksTable->setHorizontalHeaderItem(3, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        allTasksTable->setHorizontalHeaderItem(4, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        allTasksTable->setHorizontalHeaderItem(5, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        allTasksTable->setHorizontalHeaderItem(6, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        allTasksTable->setHorizontalHeaderItem(7, __qtablewidgetitem8);
        allTasksTable->setObjectName(QStringLiteral("allTasksTable"));
        allTasksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        allTasksTable->setSortingEnabled(true);
        allTasksTable->setRowCount(0);
        allTasksTable->setColumnCount(8);
        allTasksTable->horizontalHeader()->setDefaultSectionSize(68);
        allTasksTable->horizontalHeader()->setStretchLastSection(false);
        allTasksTable->verticalHeader()->setVisible(false);

        verticalLayout_2->addWidget(allTasksTable);

        tabWidget->addTab(tasksTab, QString());
        filterTab = new QWidget();
        filterTab->setObjectName(QStringLiteral("filterTab"));
        gridLayout = new QGridLayout(filterTab);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_5 = new QLabel(filterTab);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 7, 0, 1, 1);

        label_6 = new QLabel(filterTab);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 8, 0, 1, 1);

        filter1start = new QDoubleSpinBox(filterTab);
        filter1start->setObjectName(QStringLiteral("filter1start"));
        filter1start->setEnabled(false);
        filter1start->setDecimals(5);
        filter1start->setMaximum(1.67772e+7);

        gridLayout->addWidget(filter1start, 4, 2, 1, 1);

        filterTask = new QLineEdit(filterTab);
        filterTask->setObjectName(QStringLiteral("filterTask"));

        gridLayout->addWidget(filterTask, 1, 1, 1, 1);

        filter2end = new QDoubleSpinBox(filterTab);
        filter2end->setObjectName(QStringLiteral("filter2end"));
        filter2end->setEnabled(false);
        filter2end->setDecimals(5);
        filter2end->setMaximum(1.67772e+7);

        gridLayout->addWidget(filter2end, 7, 3, 1, 1);

        ruleEnd = new QComboBox(filterTab);
        ruleEnd->setObjectName(QStringLiteral("ruleEnd"));

        gridLayout->addWidget(ruleEnd, 7, 1, 1, 1);

        label_8 = new QLabel(filterTab);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 0, 0, 1, 1);

        filter2duration = new QDoubleSpinBox(filterTab);
        filter2duration->setObjectName(QStringLiteral("filter2duration"));
        filter2duration->setEnabled(false);
        filter2duration->setDecimals(5);
        filter2duration->setMaximum(1.67772e+7);

        gridLayout->addWidget(filter2duration, 8, 3, 1, 1);

        filter2start = new QDoubleSpinBox(filterTab);
        filter2start->setObjectName(QStringLiteral("filter2start"));
        filter2start->setEnabled(false);
        filter2start->setDecimals(5);
        filter2start->setMaximum(1.67772e+7);

        gridLayout->addWidget(filter2start, 4, 3, 1, 1);

        filter1amount = new QDoubleSpinBox(filterTab);
        filter1amount->setObjectName(QStringLiteral("filter1amount"));
        filter1amount->setEnabled(false);
        filter1amount->setDecimals(5);
        filter1amount->setMaximum(1.67772e+7);

        gridLayout->addWidget(filter1amount, 9, 2, 1, 1);

        filter2amount = new QDoubleSpinBox(filterTab);
        filter2amount->setObjectName(QStringLiteral("filter2amount"));
        filter2amount->setEnabled(false);
        filter2amount->setDecimals(5);
        filter2amount->setMaximum(1.67772e+7);

        gridLayout->addWidget(filter2amount, 9, 3, 1, 1);

        filter1duration = new QDoubleSpinBox(filterTab);
        filter1duration->setObjectName(QStringLiteral("filter1duration"));
        filter1duration->setEnabled(false);
        filter1duration->setDecimals(5);
        filter1duration->setMaximum(1.67772e+7);

        gridLayout->addWidget(filter1duration, 8, 2, 1, 1);

        ruleStart = new QComboBox(filterTab);
        ruleStart->setObjectName(QStringLiteral("ruleStart"));

        gridLayout->addWidget(ruleStart, 4, 1, 1, 1);

        label_4 = new QLabel(filterTab);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        filterUnit = new QLineEdit(filterTab);
        filterUnit->setObjectName(QStringLiteral("filterUnit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(filterUnit->sizePolicy().hasHeightForWidth());
        filterUnit->setSizePolicy(sizePolicy);

        gridLayout->addWidget(filterUnit, 0, 1, 1, 1);

        filter1end = new QDoubleSpinBox(filterTab);
        filter1end->setObjectName(QStringLiteral("filter1end"));
        filter1end->setEnabled(false);
        filter1end->setDecimals(5);
        filter1end->setMaximum(1.67772e+7);

        gridLayout->addWidget(filter1end, 7, 2, 1, 1);

        label_9 = new QLabel(filterTab);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout->addWidget(label_9, 1, 0, 1, 1);

        ruleDuration = new QComboBox(filterTab);
        ruleDuration->setObjectName(QStringLiteral("ruleDuration"));

        gridLayout->addWidget(ruleDuration, 8, 1, 1, 1);

        label_3 = new QLabel(filterTab);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 9, 0, 1, 1);

        ruleAmount = new QComboBox(filterTab);
        ruleAmount->setObjectName(QStringLiteral("ruleAmount"));

        gridLayout->addWidget(ruleAmount, 9, 1, 1, 1);

        filterApplyButton = new QPushButton(filterTab);
        filterApplyButton->setObjectName(QStringLiteral("filterApplyButton"));

        gridLayout->addWidget(filterApplyButton, 10, 0, 1, 1);

        filterClearButton = new QPushButton(filterTab);
        filterClearButton->setObjectName(QStringLiteral("filterClearButton"));

        gridLayout->addWidget(filterClearButton, 10, 1, 1, 1);

        tableCount = new QLabel(filterTab);
        tableCount->setObjectName(QStringLiteral("tableCount"));

        gridLayout->addWidget(tableCount, 10, 2, 1, 1);

        tabWidget->addTab(filterTab, QString());
        errorsTab = new QWidget();
        errorsTab->setObjectName(QStringLiteral("errorsTab"));
        verticalLayout = new QVBoxLayout(errorsTab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        errorsViewer = new QTextBrowser(errorsTab);
        errorsViewer->setObjectName(QStringLiteral("errorsViewer"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(errorsViewer->sizePolicy().hasHeightForWidth());
        errorsViewer->setSizePolicy(sizePolicy1);
        errorsViewer->setAcceptRichText(false);
        errorsViewer->setOpenLinks(false);

        verticalLayout->addWidget(errorsViewer);

        saveButton = new QPushButton(errorsTab);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setMaximumSize(QSize(72, 16777215));

        verticalLayout->addWidget(saveButton);

        tabWidget->addTab(errorsTab, QString());

        Sidebar->addWidget(tabWidget);

        V_Zoom = new QHBoxLayout();
        V_Zoom->setSpacing(6);
        V_Zoom->setObjectName(QStringLiteral("V_Zoom"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(110, 22));
        label_2->setMaximumSize(QSize(110, 22));

        V_Zoom->addWidget(label_2);

        v_zoom_out = new QPushButton(layoutWidget);
        v_zoom_out->setObjectName(QStringLiteral("v_zoom_out"));

        V_Zoom->addWidget(v_zoom_out);

        verticalSlider = new QSlider(layoutWidget);
        verticalSlider->setObjectName(QStringLiteral("verticalSlider"));
        verticalSlider->setValue(50);
        verticalSlider->setOrientation(Qt::Horizontal);

        V_Zoom->addWidget(verticalSlider);

        v_zoom_in = new QPushButton(layoutWidget);
        v_zoom_in->setObjectName(QStringLiteral("v_zoom_in"));

        V_Zoom->addWidget(v_zoom_in);


        Sidebar->addLayout(V_Zoom);

        H_Zoom = new QHBoxLayout();
        H_Zoom->setSpacing(6);
        H_Zoom->setObjectName(QStringLiteral("H_Zoom"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(110, 22));
        label->setMaximumSize(QSize(110, 22));

        H_Zoom->addWidget(label);

        h_zoom_out = new QPushButton(layoutWidget);
        h_zoom_out->setObjectName(QStringLiteral("h_zoom_out"));

        H_Zoom->addWidget(h_zoom_out);

        horizontalSlider = new QSlider(layoutWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setValue(50);
        horizontalSlider->setOrientation(Qt::Horizontal);

        H_Zoom->addWidget(horizontalSlider);

        h_zoom_in = new QPushButton(layoutWidget);
        h_zoom_in->setObjectName(QStringLiteral("h_zoom_in"));

        H_Zoom->addWidget(h_zoom_in);


        Sidebar->addLayout(H_Zoom);

        splitter->addWidget(layoutWidget);
        ganttView = new QGraphicsView(splitter);
        ganttView->setObjectName(QStringLiteral("ganttView"));
        ganttView->setMinimumSize(QSize(500, 500));
        ganttView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        splitter->addWidget(ganttView);

        gridLayout_2->addWidget(splitter, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1072, 22));
        menuBar->setDefaultUp(false);
        menuBar->setNativeMenuBar(true);
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        tableWidget->setSortingEnabled(__sortingEnabled);

        QTableWidgetItem *___qtablewidgetitem = allTasksTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Unit", 0));
        QTableWidgetItem *___qtablewidgetitem1 = allTasksTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Task", 0));
        QTableWidgetItem *___qtablewidgetitem2 = allTasksTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Start", 0));
        QTableWidgetItem *___qtablewidgetitem3 = allTasksTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "End", 0));
        QTableWidgetItem *___qtablewidgetitem4 = allTasksTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "Duration", 0));
        QTableWidgetItem *___qtablewidgetitem5 = allTasksTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "Amount", 0));
        QTableWidgetItem *___qtablewidgetitem6 = allTasksTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "Type", 0));
        QTableWidgetItem *___qtablewidgetitem7 = allTasksTable->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "Index", 0));
        tabWidget->setTabText(tabWidget->indexOf(tasksTab), QApplication::translate("MainWindow", "All Tasks", 0));
        label_5->setText(QApplication::translate("MainWindow", "End", 0));
        label_6->setText(QApplication::translate("MainWindow", "Duration", 0));
        filterTask->setPlaceholderText(QApplication::translate("MainWindow", "No filter", 0));
        ruleEnd->clear();
        ruleEnd->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "No rule", 0)
         << QApplication::translate("MainWindow", "Between", 0)
         << QApplication::translate("MainWindow", "Outside of", 0)
         << QApplication::translate("MainWindow", "Greater than", 0)
         << QApplication::translate("MainWindow", "Less than", 0)
        );
        label_8->setText(QApplication::translate("MainWindow", "Unit filter", 0));
        ruleStart->clear();
        ruleStart->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "No rule", 0)
         << QApplication::translate("MainWindow", "Between", 0)
         << QApplication::translate("MainWindow", "Outside of", 0)
         << QApplication::translate("MainWindow", "Greater than", 0)
         << QApplication::translate("MainWindow", "Less than", 0)
        );
        label_4->setText(QApplication::translate("MainWindow", "Start", 0));
        filterUnit->setPlaceholderText(QApplication::translate("MainWindow", "No filter", 0));
        label_9->setText(QApplication::translate("MainWindow", "Task filter", 0));
        ruleDuration->clear();
        ruleDuration->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "No rule", 0)
         << QApplication::translate("MainWindow", "Between", 0)
         << QApplication::translate("MainWindow", "Outside of", 0)
         << QApplication::translate("MainWindow", "Greater than", 0)
         << QApplication::translate("MainWindow", "Less than", 0)
        );
        label_3->setText(QApplication::translate("MainWindow", "Amount", 0));
        ruleAmount->clear();
        ruleAmount->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "No rule", 0)
         << QApplication::translate("MainWindow", "Between", 0)
         << QApplication::translate("MainWindow", "Outside of", 0)
         << QApplication::translate("MainWindow", "Greater than", 0)
         << QApplication::translate("MainWindow", "Less than", 0)
        );
        filterApplyButton->setText(QApplication::translate("MainWindow", "Apply", 0));
        filterClearButton->setText(QApplication::translate("MainWindow", "Clear", 0));
        tableCount->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(filterTab), QApplication::translate("MainWindow", "Filter", 0));
        errorsViewer->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'.SF NS Text'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">No check has been run yet. Run check to check for overlapping tasks and Flow amount sum.</p></body></html>", 0));
        saveButton->setText(QApplication::translate("MainWindow", "Save...", 0));
        tabWidget->setTabText(tabWidget->indexOf(errorsTab), QApplication::translate("MainWindow", "Log", 0));
        label_2->setText(QApplication::translate("MainWindow", "Vertical Zoom", 0));
        v_zoom_out->setText(QApplication::translate("MainWindow", "(-)", 0));
        v_zoom_in->setText(QApplication::translate("MainWindow", "(+)", 0));
        label->setText(QApplication::translate("MainWindow", "Horizontal Zoom", 0));
        h_zoom_out->setText(QApplication::translate("MainWindow", "(-)", 0));
        h_zoom_in->setText(QApplication::translate("MainWindow", "(+)", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
