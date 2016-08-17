#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>
#include "mainwindow.h"
struct RawTask {
    float start = 0;
    float end = 0;
    float amount = 0;
    int color = -1;
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

class Wizard : public QWizard
{
    Q_OBJECT

public:
    explicit Wizard(QWidget *parent = 0);
    ~Wizard();
    //Ui::MainWindow * dad;

private:
    Ui::Wizard *ui;
    QString getSysdir();
    void openGDX();
    QString loadedFile = "";
    QString loadedConfigFile = "";
    QString Sysdir = "";
    void fetchAvailableSymbols();
    void loadSymbolsFromXML();
    QStringList * availableTaskSymbols;
    QStringList * availableFlowSymbols;
    string FindGAMS();
private slots:
    ///load File button clicked
    void loadFileClicked();
    ///load Config Button Clicked
    void loadConfigClicked();
    /// get the GAMS Sysdir. If it can't be found, a dialog will request the directory from the user and save it in a config file
    void selectSysdir();
    /// save the current configuration
    void saveConfiguration();

protected:
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
