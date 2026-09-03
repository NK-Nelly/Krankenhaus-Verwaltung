/********************************************************************************
** Form generated from reading UI file 'hospitalui.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOSPITALUI_H
#define UI_HOSPITALUI_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HospitalUI
{
public:
    QAction *actionLese_Aufenthalte;
    QAction *actionLese_ICD_Texte;
    QAction *actionPatienten_anzeigen;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *patientTreeWidget;
    QVBoxLayout *verticalLayout;
    QTextBrowser *patientInfoTextBrowser;
    QTabWidget *stayTabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *diagnoseTextEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *aktuelleDiagnoseInput;
    QLineEdit *aktuelleDiagnoseText;
    QLabel *label_2;
    QTableWidget *anamneseTableWidget;
    QLabel *label_3;
    QTextEdit *bemerkungenTextEdit;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QFormLayout *formLayout;
    QLabel *label_6;
    QComboBox *targetDepartmentComboBox;
    QLabel *label_7;
    QDateEdit *transferDateEdit;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *transferPatientButton;
    QPushButton *dischargePatientButton;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QDateEdit *bpDateEdit;
    QSpinBox *systolicSpinBox;
    QLabel *label_4;
    QSpinBox *diastolicSpinBox;
    QLabel *label_5;
    QPushButton *addBPButton;
    QChartView *chartContainerWidget;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_5;
    QTableWidget *proceduresTableWidget;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_8;
    QLineEdit *opsCodeLineEdit;
    QLabel *label_9;
    QComboBox *resourceTypeComboBox;
    QLabel *label_10;
    QDateEdit *procedureDateEdit;
    QLabel *label_11;
    QSpinBox *durationSpinBox;
    QPushButton *scheduleProcedureButton;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_12;
    QLineEdit *diagnosisLineEdit;
    QLabel *label_13;
    QLineEdit *durationLineEdit;
    QLabel *label_14;
    QListWidget *proceduresListWidget;
    QLabel *label_15;
    QPlainTextEdit *difficultiesTextEdit;
    QMenuBar *menubar;
    QMenu *menuPraktikum2;
    QMenu *menuDatei;
    QMenu *menuAufenthalte;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *HospitalUI)
    {
        if (HospitalUI->objectName().isEmpty())
            HospitalUI->setObjectName("HospitalUI");
        HospitalUI->resize(529, 571);
        actionLese_Aufenthalte = new QAction(HospitalUI);
        actionLese_Aufenthalte->setObjectName("actionLese_Aufenthalte");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/recycle.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionLese_Aufenthalte->setIcon(icon);
        actionLese_ICD_Texte = new QAction(HospitalUI);
        actionLese_ICD_Texte->setObjectName("actionLese_ICD_Texte");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/folder.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionLese_ICD_Texte->setIcon(icon1);
        actionPatienten_anzeigen = new QAction(HospitalUI);
        actionPatienten_anzeigen->setObjectName("actionPatienten_anzeigen");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/people.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionPatienten_anzeigen->setIcon(icon2);
        centralwidget = new QWidget(HospitalUI);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        patientTreeWidget = new QTreeWidget(centralwidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        patientTreeWidget->setHeaderItem(__qtreewidgetitem);
        patientTreeWidget->setObjectName("patientTreeWidget");

        horizontalLayout->addWidget(patientTreeWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        patientInfoTextBrowser = new QTextBrowser(centralwidget);
        patientInfoTextBrowser->setObjectName("patientInfoTextBrowser");
        patientInfoTextBrowser->setMaximumSize(QSize(16777215, 120));

        verticalLayout->addWidget(patientInfoTextBrowser);

        stayTabWidget = new QTabWidget(centralwidget);
        stayTabWidget->setObjectName("stayTabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        diagnoseTextEdit = new QTextEdit(tab);
        diagnoseTextEdit->setObjectName("diagnoseTextEdit");
        diagnoseTextEdit->setMaximumSize(QSize(16777215, 150));
        diagnoseTextEdit->setReadOnly(true);

        verticalLayout_2->addWidget(diagnoseTextEdit);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(tab);
        label->setObjectName("label");

        horizontalLayout_2->addWidget(label);

        aktuelleDiagnoseInput = new QLineEdit(tab);
        aktuelleDiagnoseInput->setObjectName("aktuelleDiagnoseInput");

        horizontalLayout_2->addWidget(aktuelleDiagnoseInput);

        aktuelleDiagnoseText = new QLineEdit(tab);
        aktuelleDiagnoseText->setObjectName("aktuelleDiagnoseText");
        aktuelleDiagnoseText->setReadOnly(true);

        horizontalLayout_2->addWidget(aktuelleDiagnoseText);

        horizontalLayout_2->setStretch(1, 2);
        horizontalLayout_2->setStretch(2, 6);

        verticalLayout_2->addLayout(horizontalLayout_2);

        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");

        verticalLayout_2->addWidget(label_2);

        anamneseTableWidget = new QTableWidget(tab);
        if (anamneseTableWidget->columnCount() < 2)
            anamneseTableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        anamneseTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        anamneseTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (anamneseTableWidget->rowCount() < 4)
            anamneseTableWidget->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        anamneseTableWidget->setItem(0, 1, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        anamneseTableWidget->setItem(1, 1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        anamneseTableWidget->setItem(2, 1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        anamneseTableWidget->setItem(3, 1, __qtablewidgetitem5);
        anamneseTableWidget->setObjectName("anamneseTableWidget");
        anamneseTableWidget->setMaximumSize(QSize(16777215, 120));
        anamneseTableWidget->setRowCount(4);

        verticalLayout_2->addWidget(anamneseTableWidget);

        label_3 = new QLabel(tab);
        label_3->setObjectName("label_3");

        verticalLayout_2->addWidget(label_3);

        bemerkungenTextEdit = new QTextEdit(tab);
        bemerkungenTextEdit->setObjectName("bemerkungenTextEdit");
        bemerkungenTextEdit->setMaximumSize(QSize(16777215, 60));

        verticalLayout_2->addWidget(bemerkungenTextEdit);

        groupBox = new QGroupBox(tab);
        groupBox->setObjectName("groupBox");
        groupBox->setMinimumSize(QSize(0, 140));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName("verticalLayout_4");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_6 = new QLabel(groupBox);
        label_6->setObjectName("label_6");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_6);

        targetDepartmentComboBox = new QComboBox(groupBox);
        targetDepartmentComboBox->setObjectName("targetDepartmentComboBox");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, targetDepartmentComboBox);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName("label_7");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_7);

        transferDateEdit = new QDateEdit(groupBox);
        transferDateEdit->setObjectName("transferDateEdit");
        transferDateEdit->setCalendarPopup(true);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, transferDateEdit);


        verticalLayout_4->addLayout(formLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        transferPatientButton = new QPushButton(groupBox);
        transferPatientButton->setObjectName("transferPatientButton");

        horizontalLayout_4->addWidget(transferPatientButton);

        dischargePatientButton = new QPushButton(groupBox);
        dischargePatientButton->setObjectName("dischargePatientButton");

        horizontalLayout_4->addWidget(dischargePatientButton);


        verticalLayout_4->addLayout(horizontalLayout_4);


        verticalLayout_2->addWidget(groupBox);

        verticalLayout_2->setStretch(0, 1);
        verticalLayout_2->setStretch(1, 1);
        stayTabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        bpDateEdit = new QDateEdit(tab_2);
        bpDateEdit->setObjectName("bpDateEdit");
        bpDateEdit->setCalendarPopup(true);

        horizontalLayout_3->addWidget(bpDateEdit);

        systolicSpinBox = new QSpinBox(tab_2);
        systolicSpinBox->setObjectName("systolicSpinBox");
        systolicSpinBox->setMinimum(50);
        systolicSpinBox->setMaximum(250);
        systolicSpinBox->setValue(120);

        horizontalLayout_3->addWidget(systolicSpinBox);

        label_4 = new QLabel(tab_2);
        label_4->setObjectName("label_4");

        horizontalLayout_3->addWidget(label_4);

        diastolicSpinBox = new QSpinBox(tab_2);
        diastolicSpinBox->setObjectName("diastolicSpinBox");
        diastolicSpinBox->setMinimum(30);
        diastolicSpinBox->setMaximum(150);
        diastolicSpinBox->setValue(80);

        horizontalLayout_3->addWidget(diastolicSpinBox);

        label_5 = new QLabel(tab_2);
        label_5->setObjectName("label_5");

        horizontalLayout_3->addWidget(label_5);

        addBPButton = new QPushButton(tab_2);
        addBPButton->setObjectName("addBPButton");

        horizontalLayout_3->addWidget(addBPButton);


        verticalLayout_3->addLayout(horizontalLayout_3);

        chartContainerWidget = new QChartView(tab_2);
        chartContainerWidget->setObjectName("chartContainerWidget");

        verticalLayout_3->addWidget(chartContainerWidget);

        verticalLayout_3->setStretch(1, 1);
        stayTabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        verticalLayout_6 = new QVBoxLayout(tab_3);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName("verticalLayout_5");
        proceduresTableWidget = new QTableWidget(tab_3);
        if (proceduresTableWidget->columnCount() < 5)
            proceduresTableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        proceduresTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        proceduresTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        proceduresTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        proceduresTableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        proceduresTableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem10);
        proceduresTableWidget->setObjectName("proceduresTableWidget");

        verticalLayout_5->addWidget(proceduresTableWidget);

        groupBox_2 = new QGroupBox(tab_3);
        groupBox_2->setObjectName("groupBox_2");
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName("gridLayout");
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName("label_8");

        gridLayout->addWidget(label_8, 0, 0, 1, 1);

        opsCodeLineEdit = new QLineEdit(groupBox_2);
        opsCodeLineEdit->setObjectName("opsCodeLineEdit");

        gridLayout->addWidget(opsCodeLineEdit, 0, 2, 1, 1);

        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName("label_9");

        gridLayout->addWidget(label_9, 1, 0, 1, 1);

        resourceTypeComboBox = new QComboBox(groupBox_2);
        resourceTypeComboBox->setObjectName("resourceTypeComboBox");

        gridLayout->addWidget(resourceTypeComboBox, 1, 2, 1, 1);

        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName("label_10");

        gridLayout->addWidget(label_10, 2, 0, 1, 2);

        procedureDateEdit = new QDateEdit(groupBox_2);
        procedureDateEdit->setObjectName("procedureDateEdit");
        procedureDateEdit->setCalendarPopup(true);

        gridLayout->addWidget(procedureDateEdit, 2, 2, 1, 1);

        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName("label_11");

        gridLayout->addWidget(label_11, 3, 0, 1, 1);

        durationSpinBox = new QSpinBox(groupBox_2);
        durationSpinBox->setObjectName("durationSpinBox");

        gridLayout->addWidget(durationSpinBox, 3, 2, 1, 1);

        scheduleProcedureButton = new QPushButton(groupBox_2);
        scheduleProcedureButton->setObjectName("scheduleProcedureButton");

        gridLayout->addWidget(scheduleProcedureButton, 4, 1, 1, 2);


        verticalLayout_5->addWidget(groupBox_2);

        verticalLayout_5->setStretch(0, 1);
        verticalLayout_5->setStretch(1, 1);

        verticalLayout_6->addLayout(verticalLayout_5);

        stayTabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        verticalLayout_7 = new QVBoxLayout(tab_4);
        verticalLayout_7->setObjectName("verticalLayout_7");
        label_12 = new QLabel(tab_4);
        label_12->setObjectName("label_12");

        verticalLayout_7->addWidget(label_12);

        diagnosisLineEdit = new QLineEdit(tab_4);
        diagnosisLineEdit->setObjectName("diagnosisLineEdit");
        diagnosisLineEdit->setReadOnly(true);

        verticalLayout_7->addWidget(diagnosisLineEdit);

        label_13 = new QLabel(tab_4);
        label_13->setObjectName("label_13");

        verticalLayout_7->addWidget(label_13);

        durationLineEdit = new QLineEdit(tab_4);
        durationLineEdit->setObjectName("durationLineEdit");
        durationLineEdit->setReadOnly(true);

        verticalLayout_7->addWidget(durationLineEdit);

        label_14 = new QLabel(tab_4);
        label_14->setObjectName("label_14");

        verticalLayout_7->addWidget(label_14);

        proceduresListWidget = new QListWidget(tab_4);
        proceduresListWidget->setObjectName("proceduresListWidget");

        verticalLayout_7->addWidget(proceduresListWidget);

        label_15 = new QLabel(tab_4);
        label_15->setObjectName("label_15");

        verticalLayout_7->addWidget(label_15);

        difficultiesTextEdit = new QPlainTextEdit(tab_4);
        difficultiesTextEdit->setObjectName("difficultiesTextEdit");

        verticalLayout_7->addWidget(difficultiesTextEdit);

        stayTabWidget->addTab(tab_4, QString());

        verticalLayout->addWidget(stayTabWidget);


        horizontalLayout->addLayout(verticalLayout);

        HospitalUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(HospitalUI);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 529, 17));
        menuPraktikum2 = new QMenu(menubar);
        menuPraktikum2->setObjectName("menuPraktikum2");
        menuDatei = new QMenu(menubar);
        menuDatei->setObjectName("menuDatei");
        menuAufenthalte = new QMenu(menubar);
        menuAufenthalte->setObjectName("menuAufenthalte");
        HospitalUI->setMenuBar(menubar);
        statusbar = new QStatusBar(HospitalUI);
        statusbar->setObjectName("statusbar");
        HospitalUI->setStatusBar(statusbar);
        toolBar = new QToolBar(HospitalUI);
        toolBar->setObjectName("toolBar");
        HospitalUI->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        menubar->addAction(menuPraktikum2->menuAction());
        menubar->addAction(menuDatei->menuAction());
        menubar->addAction(menuAufenthalte->menuAction());
        menuDatei->addAction(actionLese_Aufenthalte);
        menuDatei->addAction(actionLese_ICD_Texte);
        menuAufenthalte->addAction(actionPatienten_anzeigen);
        toolBar->addAction(actionLese_ICD_Texte);
        toolBar->addAction(actionLese_Aufenthalte);
        toolBar->addAction(actionPatienten_anzeigen);

        retranslateUi(HospitalUI);

        stayTabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(HospitalUI);
    } // setupUi

    void retranslateUi(QMainWindow *HospitalUI)
    {
        HospitalUI->setWindowTitle(QCoreApplication::translate("HospitalUI", "HospitalUI", nullptr));
        actionLese_Aufenthalte->setText(QCoreApplication::translate("HospitalUI", "Lese Aufenthalte", nullptr));
        actionLese_ICD_Texte->setText(QCoreApplication::translate("HospitalUI", "Lese ICD Texte", nullptr));
        actionPatienten_anzeigen->setText(QCoreApplication::translate("HospitalUI", "Patienten anzeigen", nullptr));
        label->setText(QCoreApplication::translate("HospitalUI", "Aktuelle Diagnose:", nullptr));
        label_2->setText(QCoreApplication::translate("HospitalUI", "Anamnese: ", nullptr));
        QTableWidgetItem *___qtablewidgetitem = anamneseTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("HospitalUI", "ICD-Code", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = anamneseTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("HospitalUI", "Bezeichnung", nullptr));

        const bool __sortingEnabled = anamneseTableWidget->isSortingEnabled();
        anamneseTableWidget->setSortingEnabled(false);
        anamneseTableWidget->setSortingEnabled(__sortingEnabled);

        label_3->setText(QCoreApplication::translate("HospitalUI", "Bemerkungen:", nullptr));
        groupBox->setTitle(QCoreApplication::translate("HospitalUI", "Verlegung und  Entlassung", nullptr));
        label_6->setText(QCoreApplication::translate("HospitalUI", "Zielabteilung:", nullptr));
        label_7->setText(QCoreApplication::translate("HospitalUI", "Aktionsdatum: ", nullptr));
        transferPatientButton->setText(QCoreApplication::translate("HospitalUI", "Patient verlegen", nullptr));
        dischargePatientButton->setText(QCoreApplication::translate("HospitalUI", "Patient entlassen", nullptr));
        stayTabWidget->setTabText(stayTabWidget->indexOf(tab), QCoreApplication::translate("HospitalUI", " Diagnose und Anamnese", nullptr));
        label_4->setText(QCoreApplication::translate("HospitalUI", "Sys (mmHg)", nullptr));
        label_5->setText(QCoreApplication::translate("HospitalUI", "Dia (mmHg)", nullptr));
        addBPButton->setText(QCoreApplication::translate("HospitalUI", "Wert hinzuf\303\274gen", nullptr));
        stayTabWidget->setTabText(stayTabWidget->indexOf(tab_2), QCoreApplication::translate("HospitalUI", "Kurve", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = proceduresTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("HospitalUI", "OPS-Code", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = proceduresTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("HospitalUI", "Datum", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = proceduresTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("HospitalUI", "Uhrzeit", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = proceduresTableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("HospitalUI", "Dauer (Std.)", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = proceduresTableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("HospitalUI", "Resource", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("HospitalUI", "GroupBox", nullptr));
        label_8->setText(QCoreApplication::translate("HospitalUI", "OPS-Code:", nullptr));
        label_9->setText(QCoreApplication::translate("HospitalUI", "Kategorie:", nullptr));
        label_10->setText(QCoreApplication::translate("HospitalUI", "Datumswunsch:", nullptr));
        label_11->setText(QCoreApplication::translate("HospitalUI", "Dauer (Std):", nullptr));
        scheduleProcedureButton->setText(QCoreApplication::translate("HospitalUI", "Termin buchen", nullptr));
        stayTabWidget->setTabText(stayTabWidget->indexOf(tab_3), QCoreApplication::translate("HospitalUI", "Operationen und Prozeduren", nullptr));
        label_12->setText(QCoreApplication::translate("HospitalUI", "Aktuelle Diagnose des letzten Aufenthaltes:", nullptr));
        label_13->setText(QCoreApplication::translate("HospitalUI", "Gesamte Aufenthaltsdauer:", nullptr));
        label_14->setText(QCoreApplication::translate("HospitalUI", "Geplante oder durchgef\303\274hrte Operationen/Diagnostiken:", nullptr));
        label_15->setText(QCoreApplication::translate("HospitalUI", "Besondere Erschwernisse / Komplikationen:", nullptr));
        stayTabWidget->setTabText(stayTabWidget->indexOf(tab_4), QCoreApplication::translate("HospitalUI", "Abrechnung", nullptr));
        menuPraktikum2->setTitle(QCoreApplication::translate("HospitalUI", "Praktikum2", nullptr));
        menuDatei->setTitle(QCoreApplication::translate("HospitalUI", "Datei", nullptr));
        menuAufenthalte->setTitle(QCoreApplication::translate("HospitalUI", "Aufenthalte", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("HospitalUI", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HospitalUI: public Ui_HospitalUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOSPITALUI_H
