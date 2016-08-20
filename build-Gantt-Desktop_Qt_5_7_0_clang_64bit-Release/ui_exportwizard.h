/********************************************************************************
** Form generated from reading UI file 'exportwizard.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTWIZARD_H
#define UI_EXPORTWIZARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWizard>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_ExportWizard
{
public:
    QWizardPage *wizardPage1;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_3;
    QCheckBox *allflows;
    QDoubleSpinBox *vscale;
    QCheckBox *labels;
    QComboBox *color;
    QComboBox *format;
    QLabel *label_2;
    QCheckBox *filter;
    QLabel *label_4;
    QSpinBox *interval;

    void setupUi(QWizard *ExportWizard)
    {
        if (ExportWizard->objectName().isEmpty())
            ExportWizard->setObjectName(QStringLiteral("ExportWizard"));
        ExportWizard->resize(517, 327);
        wizardPage1 = new QWizardPage();
        wizardPage1->setObjectName(QStringLiteral("wizardPage1"));
        gridLayout = new QGridLayout(wizardPage1);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(wizardPage1);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(wizardPage1);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        allflows = new QCheckBox(wizardPage1);
        allflows->setObjectName(QStringLiteral("allflows"));

        gridLayout->addWidget(allflows, 5, 0, 1, 3);

        vscale = new QDoubleSpinBox(wizardPage1);
        vscale->setObjectName(QStringLiteral("vscale"));
        vscale->setSingleStep(1);
        vscale->setValue(5);

        gridLayout->addWidget(vscale, 1, 1, 1, 1);

        labels = new QCheckBox(wizardPage1);
        labels->setObjectName(QStringLiteral("labels"));
        labels->setChecked(true);

        gridLayout->addWidget(labels, 6, 0, 1, 2);

        color = new QComboBox(wizardPage1);
        color->setObjectName(QStringLiteral("color"));

        gridLayout->addWidget(color, 2, 1, 1, 1);

        format = new QComboBox(wizardPage1);
        format->setObjectName(QStringLiteral("format"));
        format->setMinimumSize(QSize(30, 0));

        gridLayout->addWidget(format, 0, 1, 1, 1);

        label_2 = new QLabel(wizardPage1);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        filter = new QCheckBox(wizardPage1);
        filter->setObjectName(QStringLiteral("filter"));
        filter->setChecked(false);

        gridLayout->addWidget(filter, 4, 0, 1, 3);

        label_4 = new QLabel(wizardPage1);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        interval = new QSpinBox(wizardPage1);
        interval->setObjectName(QStringLiteral("interval"));
        interval->setMaximumSize(QSize(16777215, 16777215));
        interval->setMinimum(10);
        interval->setMaximum(16777215);
        interval->setValue(50);

        gridLayout->addWidget(interval, 3, 1, 1, 1);

        ExportWizard->addPage(wizardPage1);

        retranslateUi(ExportWizard);

        QMetaObject::connectSlotsByName(ExportWizard);
    } // setupUi

    void retranslateUi(QWizard *ExportWizard)
    {
        ExportWizard->setWindowTitle(QApplication::translate("ExportWizard", "Wizard", 0));
        wizardPage1->setTitle(QApplication::translate("ExportWizard", "Export wizard", 0));
        label->setText(QApplication::translate("ExportWizard", "Format", 0));
        label_3->setText(QApplication::translate("ExportWizard", "Task colors", 0));
        allflows->setText(QApplication::translate("ExportWizard", "Show all flows", 0));
        labels->setText(QApplication::translate("ExportWizard", "Label Tasks", 0));
        color->clear();
        color->insertItems(0, QStringList()
         << QApplication::translate("ExportWizard", "Default", 0)
         << QApplication::translate("ExportWizard", "By Task", 0)
         << QApplication::translate("ExportWizard", "By Unit", 0)
         << QApplication::translate("ExportWizard", "By Amount", 0)
        );
        format->clear();
        format->insertItems(0, QStringList()
         << QApplication::translate("ExportWizard", "PDF", 0)
         << QApplication::translate("ExportWizard", "PNG", 0)
        );
        label_2->setText(QApplication::translate("ExportWizard", "Vertical Scaling", 0));
        filter->setText(QApplication::translate("ExportWizard", "Show all tasks (ignore filter)", 0));
        label_4->setText(QApplication::translate("ExportWizard", "Ruler Interval", 0));
    } // retranslateUi

};

namespace Ui {
    class ExportWizard: public Ui_ExportWizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTWIZARD_H
