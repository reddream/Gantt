#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>
#include "mainwindow.h"
namespace Ui {
class Wizard;
}

class Wizard : public QWizard
{
    Q_OBJECT

public:
    explicit Wizard(QWidget *parent = 0);
    ~Wizard();
    //MainWindow * myParent;

private:
    Ui::Wizard *ui;
    /// read the Sysdir from the config file
    void selectSysdir();
    QString getSysdir();
};

#endif // WIZARD_H
