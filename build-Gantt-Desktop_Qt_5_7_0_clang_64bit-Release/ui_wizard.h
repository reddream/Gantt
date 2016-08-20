/********************************************************************************
** Form generated from reading UI file 'wizard.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIZARD_H
#define UI_WIZARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWizard>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_Wizard
{
public:
    QWizardPage *wizardPageLoad;
    QGridLayout *gridLayout_5;
    QLabel *loadedFileLabel;
    QPushButton *loadFileButton;
    QLabel *loadedConfigFileLabel;
    QLabel *sysdirLabel;
    QPushButton *loadSysdirButton;
    QPushButton *loadConfigButton;
    QWizardPage *wizardPageTasks;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QLabel *label;
    QLabel *label_2;
    QComboBox *selectStart;
    QLabel *label_4;
    QComboBox *selectEnd;
    QComboBox *selectAmount;
    QComboBox *selectColor;
    QWizardPage *wizardPageFlows;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QLabel *label_5;
    QLabel *label_6;
    QComboBox *selectFlowAmount;
    QLabel *label_7;
    QComboBox *selectPr;
    QComboBox *selectCr;
    QCheckBox *checkBox;
    QWizardPage *wizardPageSaveConfig;
    QFormLayout *formLayout;
    QPushButton *saveConfigButton;

    void setupUi(QWizard *Wizard)
    {
        if (Wizard->objectName().isEmpty())
            Wizard->setObjectName(QStringLiteral("Wizard"));
        Wizard->resize(713, 415);
        Wizard->setWizardStyle(QWizard::MacStyle);
        Wizard->setOptions(QWizard::NoCancelButton|QWizard::NoDefaultButton);
        wizardPageLoad = new QWizardPage();
        wizardPageLoad->setObjectName(QStringLiteral("wizardPageLoad"));
        gridLayout_5 = new QGridLayout(wizardPageLoad);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        loadedFileLabel = new QLabel(wizardPageLoad);
        loadedFileLabel->setObjectName(QStringLiteral("loadedFileLabel"));

        gridLayout_5->addWidget(loadedFileLabel, 0, 1, 1, 1);

        loadFileButton = new QPushButton(wizardPageLoad);
        loadFileButton->setObjectName(QStringLiteral("loadFileButton"));

        gridLayout_5->addWidget(loadFileButton, 0, 0, 1, 1);

        loadedConfigFileLabel = new QLabel(wizardPageLoad);
        loadedConfigFileLabel->setObjectName(QStringLiteral("loadedConfigFileLabel"));

        gridLayout_5->addWidget(loadedConfigFileLabel, 2, 1, 1, 1);

        sysdirLabel = new QLabel(wizardPageLoad);
        sysdirLabel->setObjectName(QStringLiteral("sysdirLabel"));

        gridLayout_5->addWidget(sysdirLabel, 1, 1, 1, 1);

        loadSysdirButton = new QPushButton(wizardPageLoad);
        loadSysdirButton->setObjectName(QStringLiteral("loadSysdirButton"));
        loadSysdirButton->setMaximumSize(QSize(100, 16777215));

        gridLayout_5->addWidget(loadSysdirButton, 1, 0, 1, 1);

        loadConfigButton = new QPushButton(wizardPageLoad);
        loadConfigButton->setObjectName(QStringLiteral("loadConfigButton"));

        gridLayout_5->addWidget(loadConfigButton, 2, 0, 1, 1);

        Wizard->addPage(wizardPageLoad);
        wizardPageTasks = new QWizardPage();
        wizardPageTasks->setObjectName(QStringLiteral("wizardPageTasks"));
        gridLayout = new QGridLayout(wizardPageTasks);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_3 = new QLabel(wizardPageTasks);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 3, 0, 1, 1);

        label = new QLabel(wizardPageTasks);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        label_2 = new QLabel(wizardPageTasks);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 2, 0, 1, 1);

        selectStart = new QComboBox(wizardPageTasks);
        selectStart->setObjectName(QStringLiteral("selectStart"));

        gridLayout_2->addWidget(selectStart, 1, 1, 1, 1);

        label_4 = new QLabel(wizardPageTasks);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 4, 0, 1, 1);

        selectEnd = new QComboBox(wizardPageTasks);
        selectEnd->setObjectName(QStringLiteral("selectEnd"));

        gridLayout_2->addWidget(selectEnd, 2, 1, 1, 1);

        selectAmount = new QComboBox(wizardPageTasks);
        selectAmount->setObjectName(QStringLiteral("selectAmount"));

        gridLayout_2->addWidget(selectAmount, 3, 1, 1, 1);

        selectColor = new QComboBox(wizardPageTasks);
        selectColor->setObjectName(QStringLiteral("selectColor"));

        gridLayout_2->addWidget(selectColor, 4, 1, 1, 1);


        gridLayout->addLayout(gridLayout_2, 1, 0, 1, 1);

        Wizard->addPage(wizardPageTasks);
        wizardPageFlows = new QWizardPage();
        wizardPageFlows->setObjectName(QStringLiteral("wizardPageFlows"));
        gridLayout_4 = new QGridLayout(wizardPageFlows);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_5 = new QLabel(wizardPageFlows);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_3->addWidget(label_5, 0, 0, 1, 1);

        label_6 = new QLabel(wizardPageFlows);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_3->addWidget(label_6, 1, 0, 1, 1);

        selectFlowAmount = new QComboBox(wizardPageFlows);
        selectFlowAmount->setObjectName(QStringLiteral("selectFlowAmount"));

        gridLayout_3->addWidget(selectFlowAmount, 0, 1, 1, 1);

        label_7 = new QLabel(wizardPageFlows);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_3->addWidget(label_7, 2, 0, 1, 1);

        selectPr = new QComboBox(wizardPageFlows);
        selectPr->setObjectName(QStringLiteral("selectPr"));

        gridLayout_3->addWidget(selectPr, 1, 1, 1, 1);

        selectCr = new QComboBox(wizardPageFlows);
        selectCr->setObjectName(QStringLiteral("selectCr"));

        gridLayout_3->addWidget(selectCr, 2, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 2, 0, 1, 1);

        checkBox = new QCheckBox(wizardPageFlows);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setChecked(true);

        gridLayout_4->addWidget(checkBox, 0, 0, 1, 1);

        Wizard->addPage(wizardPageFlows);
        wizardPageSaveConfig = new QWizardPage();
        wizardPageSaveConfig->setObjectName(QStringLiteral("wizardPageSaveConfig"));
        formLayout = new QFormLayout(wizardPageSaveConfig);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        saveConfigButton = new QPushButton(wizardPageSaveConfig);
        saveConfigButton->setObjectName(QStringLiteral("saveConfigButton"));

        formLayout->setWidget(0, QFormLayout::LabelRole, saveConfigButton);

        Wizard->addPage(wizardPageSaveConfig);

        retranslateUi(Wizard);

        QMetaObject::connectSlotsByName(Wizard);
    } // setupUi

    void retranslateUi(QWizard *Wizard)
    {
        Wizard->setWindowTitle(QApplication::translate("Wizard", "Wizard", 0));
        wizardPageLoad->setTitle(QApplication::translate("Wizard", "Select file", 0));
        wizardPageLoad->setSubTitle(QApplication::translate("Wizard", "Select the GDX file to load, the GAMS Sysdir and an optional config file.", 0));
        loadedFileLabel->setText(QApplication::translate("Wizard", "No file loaded. (obligatory)", 0));
        loadFileButton->setText(QApplication::translate("Wizard", "Load...", 0));
        loadedConfigFileLabel->setText(QApplication::translate("Wizard", "No config file loaded. (optional)", 0));
        sysdirLabel->setText(QApplication::translate("Wizard", "No Sysdir selected.  (obligatory)", 0));
        loadSysdirButton->setText(QApplication::translate("Wizard", "Select...", 0));
        loadConfigButton->setText(QApplication::translate("Wizard", "Load...", 0));
        wizardPageTasks->setTitle(QApplication::translate("Wizard", "Select symbols", 0));
        wizardPageTasks->setSubTitle(QApplication::translate("Wizard", "Select the symbol for each task attribute from a list of available symbols", 0));
        label_3->setText(QApplication::translate("Wizard", "Amount", 0));
        label->setText(QApplication::translate("Wizard", "Start", 0));
        label_2->setText(QApplication::translate("Wizard", "End", 0));
        label_4->setText(QApplication::translate("Wizard", "Color", 0));
        wizardPageFlows->setTitle(QApplication::translate("Wizard", "Select Flow Symbols", 0));
        wizardPageFlows->setSubTitle(QApplication::translate("Wizard", "Select the symbol for each task attribute from a list of available symbols", 0));
        label_5->setText(QApplication::translate("Wizard", "Amount", 0));
        label_6->setText(QApplication::translate("Wizard", "Production Rate", 0));
        label_7->setText(QApplication::translate("Wizard", "Consumption Rate", 0));
        checkBox->setText(QApplication::translate("Wizard", "Add flows", 0));
        wizardPageSaveConfig->setTitle(QApplication::translate("Wizard", "Save configuration", 0));
        wizardPageSaveConfig->setSubTitle(QApplication::translate("Wizard", "Save this configuration for later. (optional)", 0));
        saveConfigButton->setText(QApplication::translate("Wizard", "Save...", 0));
    } // retranslateUi

};

namespace Ui {
    class Wizard: public Ui_Wizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZARD_H
