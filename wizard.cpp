#include "wizard.h"
#include "ui_wizard.h"
#include "gclgms.h"
#include "gdxcc.h"
Wizard::Wizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::Wizard)
{
    ui->setupUi(this);
}

Wizard::~Wizard()
{
    delete ui;
}
/*
void Wizard::openGDX(QString filename){
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
/*
void Wizard::selectSysdir(){
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

QString Wizard::getSysdir(){
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
*/
