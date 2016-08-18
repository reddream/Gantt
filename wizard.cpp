#include "wizard.h"
#include "ui_wizard.h"
#include "gclgms.h"
#include "gdxcc.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include "constants.h"
#if defined(_WIN32)
#define UNICODE
#include <Windows.h>
#endif
Wizard::Wizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::Wizard)
{
    ui->setupUi(this);
    ui->loadFileButton->click();
    connect(ui->loadFileButton, SIGNAL(clicked(bool)), this, SLOT(loadFileClicked()));
    connect(ui->loadConfigButton, SIGNAL(clicked(bool)), this, SLOT(loadConfigClicked()));
    connect(ui->loadSysdirButton, SIGNAL(clicked(bool)), this, SLOT(selectSysdir()));
    connect(ui->saveConfigButton, SIGNAL(clicked(bool)), this, SLOT(saveConfiguration()));

    if (getSysdir() != ""){
        ui->sysdirLabel->setText(getSysdir());
        Sysdir = getSysdir();
    }
}

Wizard::~Wizard()
{
    delete ui;
}
void Wizard::loadFileClicked(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open gdx"), DEFAULTDIRECTORY, "GDX files (*.gdx)");
    if ( fileName == ""){
        return;
    }

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox errorBox;
        errorBox.critical(0,"Error", "Could not open file for reading");
        return;
    }
    file.close();
    loadedFile = fileName;
    ui->loadedFileLabel->setText(fileName);
}
void Wizard::loadConfigClicked(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open configuration file"), loadedFile == "" ? DEFAULTDIRECTORY : loadedFile+".config");
    if ( fileName == ""){
        return;
    }

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox errorBox;
        errorBox.critical(0,"Error", "Could not open file for reading");
        return;
    }
    file.close();
    loadedConfigFile = fileName;
    ui->loadedConfigFileLabel->setText(fileName);
}
bool Wizard::validateCurrentPage(){
    qDebug("%d", currentId());
    if (currentId() == 0){
        if (loadedFile == "" || Sysdir == ""){
            return false;
        } else {
            fetchAvailableSymbols();
            if (loadedConfigFile != "") {
                loadSymbolsFromConfig();
            }
        }
    }
    if (currentId() == 3){
        openGDX();
    }

    return true;
}
void Wizard::fetchAvailableSymbols(){
    availableFlowSymbols = new QStringList();
    availableTaskSymbols = new QStringList();
    ui->selectAmount->clear();
    ui->selectStart->clear();
    ui->selectEnd->clear();
    ui->selectColor->clear();
    ui->selectPr->clear();
    ui->selectCr->clear();
    ui->selectFlowAmount->clear();

    static gdxStrIndexPtrs_t Indx;
    static gdxStrIndex_t IndxXXX;
    static gdxValues_t Values;
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
    //int D;


    if (!gdxCreateD(&PGX, Sysdir.toLatin1().data(), Msg, sizeof(Msg))) {
        qDebug("**** Could not load GDX library\n");
        qDebug("**** %s\n", Msg);
        return;
    }

    gdxGetDLLVersion(PGX, Msg);

    qDebug("Using GDX DLL version: %s\n", Msg);

    GDXSTRINDEXPTRS_INIT(IndxXXX, Indx);
    gdxOpenRead(PGX, loadedFile.toLatin1().data(), &ErrNr);
    if (ErrNr) {
        qDebug("**** Fatal I/O Error = %d when calling %s\n", ErrNr, "gdxOpenRead");

        gdxErrorStr(PGX, ErrNr, Msg);
        qDebug("%s", Msg);
        return;
    }
    gdxFileVersion(PGX, Msg, Producer);
    qDebug("GDX file written using version: %s\n", Msg);
    qDebug("GDX file written by: %s\n", Producer);

    int SyCnt;
    int UelCnt;
    gdxSystemInfo(PGX, &SyCnt, &UelCnt);

    qDebug("Found %d Symbols.",SyCnt);
    for (int i = 0; i<= SyCnt; i++){
        gdxSymbolInfo(PGX, i, VarName, &Dim, &VarTyp);

        qDebug("Dim : %d, Name: %s, Type: %d\n", Dim, VarName, VarTyp);

        if (Dim == 4){ //Candidate for Task symbols
            QStringList foundNames = QStringList();
            if (!gdxDataReadStrStart(PGX, i, &NrRecs)) {
                char S[GMS_SSSIZE];

                qDebug("**** Fatal GDX Error\n");
                gdxErrorStr(PGX, gdxGetLastError(PGX), S);
                qDebug("**** %s\n", S);
                return;
            }
            qDebug("%s has %d records\n",VarName,  NrRecs);
            // Find all Symbols in the 4th dimension
            while (gdxDataReadStr(PGX, Indx, Values, &N)) {
                if (!foundNames.contains(QString(Indx[3]))){
                    foundNames.append(QString(Indx[3]));
                    qDebug("Found possible task symbol %s on %s", Indx[3], VarName);
                }
            }
            foreach (QString n, foundNames) {
                availableTaskSymbols->append(QString(VarName)+" - "+n);
            }
        }
        if (Dim == 5){ //Candidate for Flow symbols
            QStringList foundNames;
            if (!gdxDataReadStrStart(PGX, i, &NrRecs)) {
                char S[GMS_SSSIZE];

                qDebug("**** Fatal GDX Error\n");
                gdxErrorStr(PGX, gdxGetLastError(PGX), S);
                qDebug("**** %s\n", S);
                return;
            }
            qDebug("%s has %d records\n", VarName, NrRecs);
            // Find all Symbols in the 5th dimension
            while (gdxDataReadStr(PGX, Indx, Values, &N)) {
                if (!foundNames.contains(QString(Indx[4]))){
                    foundNames.append(QString(Indx[4]));
                    qDebug("Found possible flow symbol %s on %s", Indx[4], VarName);
                }
            }
            foreach (QString n, foundNames) {
                availableFlowSymbols->append(QString(VarName)+" - "+n);
            }
        }
    }

    qDebug("Found %d task symbols and %d flow symbols.\n", availableTaskSymbols->count(), availableFlowSymbols->count());
    gdxDataReadDone(PGX);

    if ((ErrNr = gdxClose(PGX))) {
        qDebug("**** Fatal I/O Error = %d when calling %s\n", ErrNr, "gdxClose");
    }

    if (!gdxFree(&PGX)) {
        qDebug("Problems unloading the GDX DLL\n");
    }
    ui->selectColor->addItem(RANDOMCOLORID);
    for(int i = 0; i < availableTaskSymbols->count(); i++){
        ui->selectAmount->addItem(availableTaskSymbols->at(i));
        ui->selectStart->addItem(availableTaskSymbols->at(i));
        ui->selectEnd->addItem(availableTaskSymbols->at(i));
        ui->selectColor->addItem(availableTaskSymbols->at(i));
    }

    for(int i = 0; i < availableFlowSymbols->count(); i++){
        ui->selectPr->addItem(availableFlowSymbols->at(i));
        ui->selectCr->addItem(availableFlowSymbols->at(i));
        ui->selectFlowAmount->addItem(availableFlowSymbols->at(i));
    }


}
void Wizard::saveConfiguration(){
    QString saveLocation = QFileDialog::getSaveFileName(this, "Select location to save file location to",loadedFile+".config");
    if (saveLocation == ""){
        QMessageBox errorBox;
        errorBox.critical(0,"Error", "Error selecting location");
        return;
    }
    QFile file(saveLocation);
    if(!file.open(QFile::WriteOnly)){
        QMessageBox errorBox;
        errorBox.critical(0,"Error", "Error opening config file for writing");
        return;
    }

    QTextStream in(&file);
    in << "#This file stores the configuration for reading a gdx file as a Gantt chart. Line numbers are important, dont change this order.\n";
    in << "#Format: VariableName - SymbolName Order: Amount, Start, End, Color, IncludeFlows?, Pr, Cr, FlowAmount. Whitespaces are important.\n";
    in << ui->selectAmount->currentText()+"\n";
    in << ui->selectStart->currentText()+"\n";
    in << ui->selectEnd->currentText()+"\n";
    in << ui->selectColor->currentText()+"\n";
    in << (ui->checkBox->isChecked() ? "YES\n" : "NO\n");
    in << ui->selectPr->currentText()+"\n";
    in << ui->selectCr->currentText()+"\n";
    in << ui->selectFlowAmount->currentText()+"\n";
    file.close();
}

void Wizard::loadSymbolsFromConfig(){
    QFile file(loadedConfigFile);
    if(!file.open(QFile::ReadOnly)){
        QMessageBox errorBox;
        errorBox.critical(0,"Error", "Error opening config file for reading");
        return;
    }
    QStringList fileContents = QStringList();
    QTextStream in(&file);
    while (true){
        QString line = in.readLine();
        if (line.isNull())
            break;
        else
            fileContents.append(line);
    }
    if (fileContents.count() < 10){
        QMessageBox errorBox;
        errorBox.critical(0,"Error", "Invalid config file");
        return;
    }
    ui->checkBox->setChecked(fileContents.at(6) == "YES");
    int i = -1;
    qDebug("Amount : %d", ui->selectAmount->findText(fileContents.at(2)));
    if (i = ui->selectAmount->findText(fileContents.at(2))){
        ui->selectAmount->setCurrentIndex(i);
    }
    if (i = ui->selectStart->findText(fileContents.at(3))){
        ui->selectStart->setCurrentIndex(i);
    }
    if (i = ui->selectEnd->findText(fileContents.at(4))){
        ui->selectEnd->setCurrentIndex(i);
    }
    if (i = ui->selectColor->findText(fileContents.at(5))){
        ui->selectColor->setCurrentIndex(i);
    }
    if (i = ui->selectPr->findText(fileContents.at(7))){
        ui->selectPr->setCurrentIndex(i);
    }
    if (i = ui->selectCr->findText(fileContents.at(8))){
        ui->selectCr->setCurrentIndex(i);
    }
    if (i = ui->selectFlowAmount->findText(fileContents.at(9))){
        ui->selectFlowAmount->setCurrentIndex(i);
    }

}


void Wizard::openGDX(){

    QString amountVar = ui->selectAmount->currentText().replace(" ","").split("-").at(0);
    QString amountSym = ui->selectAmount->currentText().replace(" ","").split("-").at(1);
    QString startVar  = ui->selectStart->currentText().replace(" ","").split("-").at(0);
    QString startSym  = ui->selectStart->currentText().replace(" ","").split("-").at(1);
    QString endVar  = ui->selectEnd->currentText().replace(" ","").split("-").at(0);
    QString endSym  = ui->selectEnd->currentText().replace(" ","").split("-").at(1);
    QString colorVar;
    QString colorSym;
    bool randomColor = false;
    if (ui->selectColor->currentText() == RANDOMCOLORID){
        randomColor = true;
    } else {
        colorVar  = ui->selectColor->currentText().replace(" ","").split("-").at(0);
        colorSym = ui->selectColor->currentText().replace(" ","").split("-").at(1);
    }
    QString prVar = ui->selectPr->currentText().replace(" ","").split("-").at(0);
    QString prSym = ui->selectPr->currentText().replace(" ","").split("-").at(1);
    QString crVar = ui->selectCr->currentText().replace(" ","").split("-").at(0);
    QString crSym = ui->selectCr->currentText().replace(" ","").split("-").at(1);
    QString flowAmountVar = ui->selectFlowAmount->currentText().replace(" ","").split("-").at(0);
    QString flowAmountSym = ui->selectFlowAmount->currentText().replace(" ","").split("-").at(1);

    QMap<QString, RawTask> tasks{};
    QMap<QString, RawFlow> flows{};

    QStringList taskVars = {amountVar, startVar, endVar, colorVar};
    QStringList flowVars = {prVar, crVar, flowAmountVar};



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
    gdxOpenRead(PGX, loadedFile.toLatin1().data(), &ErrNr);
    if (ErrNr) {
        qDebug("**** Fatal I/O Error = %d when calling %s\n", ErrNr, "gdxOpenRead");

        gdxErrorStr(PGX, ErrNr, Msg);
        qDebug("%s", Msg);
        return;
    }

    gdxFileVersion(PGX, Msg, Producer);
    qDebug("GDX file written using version: %s\n", Msg);
    qDebug("GDX file written by: %s\n", Producer);
    foreach (QString taskVar, taskVars) {

        if (!gdxFindSymbol(PGX, taskVar.toLatin1().data(), &VarNr)) {
            qDebug("**** Could not find variable %s \n",taskVar.toLatin1().data());
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
        qDebug("%s has %d records\n",taskVar.toLatin1().data(), NrRecs);


        while (gdxDataReadStr(PGX, Indx, Values, &N)) {

            if (strcmp(Indx[3], amountSym.toLatin1().data()) == 0 && taskVar == amountVar) {
                if (Values[GMS_VAL_LEVEL] != GMS_SV_EPS) {
                    QString name = QString(Indx[0])+"."+QString(Indx[1])+"."+QString(Indx[2]);
                    RawTask task;
                    if (tasks.contains(name)){
                        task =  tasks.value(name);
                    }
                    task.amount = Values[GMS_VAL_LEVEL];
                    tasks.insert(name, task);
                }
            }

            if (strcmp(Indx[3], startSym.toLatin1().data()) == 0 && taskVar == startVar) {
                if (Values[GMS_VAL_LEVEL] != GMS_SV_EPS) {
                    QString name = QString(Indx[0])+"."+QString(Indx[1])+"."+QString(Indx[2]);
                    RawTask task;
                    if (tasks.contains(name)){
                        task =  tasks.value(name);
                    }
                    task.start = Values[GMS_VAL_LEVEL];
                    tasks.insert(name, task);
                }
            }
            if (strcmp(Indx[3], endSym.toLatin1().data()) == 0 && taskVar == endVar) {
                if (Values[GMS_VAL_LEVEL] != GMS_SV_EPS) {
                    QString name = QString(Indx[0])+"."+QString(Indx[1])+"."+QString(Indx[2]);
                    RawTask task;
                    if (tasks.contains(name)){
                        task =  tasks.value(name);
                    }
                    task.end = Values[GMS_VAL_LEVEL];
                    tasks.insert(name, task);
                }
            }
            if (!randomColor){
                if (strcmp(Indx[3], colorSym.toLatin1().data()) == 0 && taskVar == colorVar) {
                    if (Values[GMS_VAL_LEVEL] == GMS_SV_EPS) {

                        QString name = QString(Indx[0])+"."+QString(Indx[1])+"."+QString(Indx[2]);
                        RawTask task;
                        if (tasks.contains(name)){
                            task =  tasks.value(name);
                        }
                        task.color = 0;
                        tasks.insert(name, task);
                    } else {
                        QString name = QString(Indx[0])+"."+QString(Indx[1])+"."+QString(Indx[2]);
                        RawTask task;
                        if (tasks.contains(name)){
                            task =  tasks.value(name);
                        }
                        task.color = Values[GMS_VAL_LEVEL];
                        tasks.insert(name, task);
                    }
                }
            }
        }

    }
    qDebug("Found %d tasks.\n", tasks.count());

    if (ui->checkBox->isChecked()){
        foreach (QString flowVar, flowVars) {

            if (!gdxFindSymbol(PGX, flowVar.toLatin1().data(), &VarNr)) {
                qDebug("**** Could not find variable %s \n",flowVar.toLatin1().data());
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
            qDebug("%s has %d records\n", flowVar.toLatin1().data(), NrRecs);


            while (gdxDataReadStr(PGX, Indx, Values, &N)) {

                if (strcmp(Indx[4], crSym.toLatin1().data()) == 0 && flowVar == crVar) {
                    if (Values[GMS_VAL_LEVEL] != GMS_SV_EPS) {
                        QString name = QString(Indx[0])+"."+QString(Indx[1])+"."+QString(Indx[2])+"."+QString(Indx[3]);
                        RawFlow flow;
                        if (flows.contains(name)){
                            flow =  flows.value(name);
                        }
                        flow.cr = Values[GMS_VAL_LEVEL];
                        flows.insert(name, flow);
                    }
                }
                if (strcmp(Indx[4], prSym.toLatin1().data()) == 0 && flowVar == prVar) {
                    if (Values[GMS_VAL_LEVEL] != GMS_SV_EPS) {
                        QString name = QString(Indx[0])+"."+QString(Indx[1])+"."+QString(Indx[2])+"."+QString(Indx[3]);
                        RawFlow flow;
                        if (flows.contains(name)){
                            flow =  flows.value(name);
                        }
                        flow.pr = Values[GMS_VAL_LEVEL];
                        flows.insert(name, flow);
                    }
                }
                if (strcmp(Indx[4], flowAmountSym.toLatin1().data()) == 0 && flowVar == flowAmountVar) {
                    if (Values[GMS_VAL_LEVEL] != GMS_SV_EPS) {
                        QString name = QString(Indx[0])+"."+QString(Indx[1])+"."+QString(Indx[2])+"."+QString(Indx[3]);
                        RawFlow flow;
                        if (flows.contains(name)){
                            flow =  flows.value(name);
                        }
                        flow.amount = Values[GMS_VAL_LEVEL];
                        flows.insert(name, flow);
                    }
                }
            }

        }
        qDebug("Found %d flows.\n", flows.count());
    }


    gdxDataReadDone(PGX);

    reset();
    foreach(QString name, tasks.keys()){
        QStringList split = name.split(".");
        QString color = "random";
        if (tasks.value(name).color != -1){
            //char * ccolor = "#000000";
            //sprintf(ccolor, "#%06x", (int)tasks.value(name).color);
            color = "#"+QString::number((int)tasks.value(name).color, 16);
        }
        qDebug()<<"ay"<<color;
        addTask(split[0],split[1], tasks.value(name).start,tasks.value(name).end,tasks.value(name).amount, color);
    }

    foreach(QString name, flows.keys()){
        QStringList split = name.split(".");
        addFlow(split[0],split[1],split[2],split[3],flows.value(name).pr,flows.value(name).cr,flows.value(name).amount);
    }
    createRepresentation();
    if ((ErrNr = gdxClose(PGX))) {
        qDebug("**** Fatal I/O Error = %d when calling %s\n", ErrNr, "gdxClose");
    }

    if (!gdxFree(&PGX)) {
        qDebug("Problems unloading the GDX DLL\n");

    }
}

std::string Wizard::FindGAMS()
{
    string GAMSDir = "";
#if __APPLE__
    QStringList str_split = QString(getenv("PATH")).split(":");
    foreach (QString pth, str_split) {
        qDebug()<<pth;
        if (pth.split("/",QString::SkipEmptyParts).last() == "sysdir"){
            return pth.toStdString();
        }
    }
    return "";
#elif __unix__
    string GAMSDirLD = "";

    vector<string> str_split = getenvAndSplit("PATH");
    for(vector<string>::iterator it = str_split.begin(); it != str_split.end(); ++it)
    {
        if(exists(path(*it) / path("gams")))
        {
            GAMSDir = *it;
            break;
        }
    }
    if ( !GAMSDir.empty() && DebugLevel == 0 )
        return GAMSDir;

    str_split = getenvAndSplit("LD_LIBRARY_PATH");
    for(vector<string>::iterator it = str_split.begin(); it != str_split.end(); ++it)
    {
        if(exists(path(*it) / path("gams")))
        {
            GAMSDirLD = *it;
            break;
        }
    }
    if (GAMSDir.empty())
        return GAMSDirLD;
    else if ((!GAMSDirLD.empty()) && (GAMSDir == GAMSDirLD))
    {
        cout << "--- Warning: Found GAMS system directory " << GAMSDir << " in PATH and a different one" << endl;
        cout << "---          in (DY)LD_LIBRARY_PATH (" << GAMSDirLD << "). The latter is ignored." << endl;
    }

    return GAMSDir;
#else

    DWORD dwType = REG_SZ;
    HKEY hKey = 0;
    char value[1024] = { NULL };
    DWORD value_length = 1024;
    const wchar_t* subkey = L"gams.location";
    RegOpenKey(HKEY_CURRENT_USER, subkey, &hKey);
    if ( 0 == hKey )
    {
        RegOpenKey(HKEY_CLASSES_ROOT, subkey, &hKey);
    }
    if ( 0 == hKey )
        return "C:/Cannot/Find/GAMS/System/Directory/";

    RegQueryValueEx(hKey, NULL, NULL, &dwType, (LPBYTE)&value, &value_length);
    RegCloseKey(hKey);
    return value;
#endif

}
void Wizard::selectSysdir(){
    QString newSysdir = QFileDialog::getExistingDirectory(this, "Select the GAMS Sysdir");
    if (newSysdir == ""){
        return;
    }
    Sysdir = newSysdir;
    ui->sysdirLabel->setText(Sysdir);
}

QString Wizard::getSysdir(){
    QString gamsdir = QString::fromStdString(FindGAMS());
    qDebug()<<gamsdir;
    return gamsdir;
}

