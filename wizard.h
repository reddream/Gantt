#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>
#include "mainwindow.h"
struct RawTask {
    float start = 0;
    float end = 0;
    float amount = 0;
    float color = -1;
};

struct RawFlow {
    float pr = 0;
    float cr = 0;
    float amount = 0;
};

namespace Ui {
class Wizard;
}
using namespace std;


/// Class for the GDX import wizard
class Wizard : public QWizard
{
    Q_OBJECT

public:
    explicit Wizard(QWidget *parent = 0);
    ~Wizard();
    /// currently selected gdx file
    QString loadedFile = "";
private:
    Ui::Wizard *ui;
    /// calls FindGAMS(), returns value as a QString
    QString getSysdir();
    /// import the GDX file
    void openGDX();
    /// path to the config file
    QString loadedConfigFile = "";
    ///path to the GAMS Sysdir
    QString Sysdir = "";
    /// Finds all Symbols in the loadedFile gdx
    void fetchAvailableSymbols();
    /// Loads symbols to use in openGDX() from a previously saved config file
    void loadSymbolsFromConfig();
    /// List of available Task symbols
    QStringList * availableTaskSymbols;
    /// List of available Flow symbols
    QStringList * availableFlowSymbols;
    /// Searches for the GAMS Sysdir
    string FindGAMS();
private slots:
    ///load File button clicked
    void loadFileClicked();
    ///load Config Button Clicked
    void loadConfigClicked();
    ///  set the GAMS Sysdir if none can be found
    void selectSysdir();
    /// save the current configuration
    void saveConfiguration();

protected:
    /// called at page change
    bool validateCurrentPage();

signals:
    /// call MainWindow::createRepresentation()
    void createRepresentation();
    /// call MainWindow::addFlow()
    void addFlow(QString unit1name, QString op1name, QString unit2name, QString op2name, float pr, float cr, float amount);
    /// call MainWindow::addTask()
    void addTask(QString unitName, QString taskName, float start, float end, float amount, QString color = "random");
    /// call MainWindow::reset()
    void reset();


};

#endif // WIZARD_H
