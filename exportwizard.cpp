#include "exportwizard.h"
#include "ui_exportwizard.h"

ExportWizard::ExportWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::ExportWizard)
{
    ui->setupUi(this);
}

ExportWizard::~ExportWizard()
{
    delete ui;
}
bool ExportWizard::validateCurrentPage(){
    format = ui->format->currentText();
    vscale = ui->vscale->value();
    allFlows = ui->allflows->isChecked();
    filter = ui->filter->isChecked();
    labels = ui->labels->isChecked();
    interval = ui->interval->value();
    color = ui->color->currentIndex();
    finished();
    return true;
}
