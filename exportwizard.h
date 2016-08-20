#ifndef EXPORTWIZARD_H
#define EXPORTWIZARD_H

#include <QWizard>

namespace Ui {
class ExportWizard;
}

class ExportWizard : public QWizard
{
    Q_OBJECT

public:
    explicit ExportWizard(QWidget *parent = 0);
    ~ExportWizard();
    QString format;
    double vscale;
    int interval;
    bool labels;
    bool allFlows;
    bool filter;
    int color;
signals:
    void finished();
private:
    Ui::ExportWizard *ui;
protected:
    /// called at page change
    bool validateCurrentPage();

};

#endif // EXPORTWIZARD_H
