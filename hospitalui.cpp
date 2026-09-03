#include "hospitalui.h"
#include "ui_hospitalui.h"
#include "hospital.h"
#include "patient.h"
#include "department.h"

#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

HospitalUI::HospitalUI(Hospital* hospitalPtr, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HospitalUI)
    , hospital(hospitalPtr) // Initializing backend controller
{
    ui->setupUi(this);

    // AUTOMATIC LOADING VIA QT RESOURCE SYSTEM PATH PREFIX
    hospital->readICDData(":/data/icdCodes.txt");
    hospital->readCaseFile(":/data/stay.json");
    hospital->readOPSCodes(":/data/opsCodes.txt");

    // INITIALIZE TRANSFER & DISCHARGE UI CONTROLS
    // =========================================================================

    ui->transferDateEdit->setDate(QDate::currentDate()); // Default to today's date

    //  Clear out any accidental default design items
    ui->targetDepartmentComboBox->clear();

    //  Loop over the backend hospital department mapping to populate the rows
    for (const auto& pair : hospital->getDepartments()) {
        // pair.second is a std::shared_ptr<Department>
        std::string deptName = pair.second->getName();
        int enumValue = static_cast<int>(pair.first); // Its DepartmentType key index

        // Add text item to dropdown, storing the hidden structural integer enum index alongside it
        ui->targetDepartmentComboBox->addItem(QString::fromStdString(deptName), QVariant(enumValue));
    }

    // ANAMNESE TABLE COLUMN WIDTH CONFIGURATION

    // Set a clean, fixed starting width for the short ICD-Code inputs
    ui->anamneseTableWidget->setColumnWidth(0, 100);

    // Force the "Bezeichnung" column to automatically stretch and fill all remaining space!
    ui->anamneseTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->bpDateEdit->setDate(QDate::currentDate());

    // Initialize the Resource Category Dropdown options
    ui->resourceTypeComboBox->clear();
    ui->resourceTypeComboBox->addItem("Operationssaal"); // Index 0 -> ResourceType::OperatingRoom
    ui->resourceTypeComboBox->addItem("MRT");            // Index 1 -> ResourceType::MRI
    ui->resourceTypeComboBox->addItem("CT");             // Index 2 -> ResourceType::CT

    // Also fix the Date Picker to show the current date on startup instead of 2000
    ui->procedureDateEdit->setDate(QDate::currentDate());

    updatePatientTree();

    // =========================================================================
    // NEU FÜR AUFGABE 3(e): Autocomplete (QCompleter) einrichten
    // =========================================================================

    // 1. ICD-Code Completer vorbereiten
    QStringList icdList;
    // Wir holen alle eingelesenen ICD-Codes aus der Map des Backends
    for (const auto& pair : hospital->getIcdMap() ){
        icdList << QString::fromStdString(pair.first);
    }

    icdCompleter = new QCompleter(icdList, this);
    icdCompleter->setCaseSensitivity(Qt::CaseInsensitive); // Groß-/Kleinschreibung ignorieren
    icdCompleter->setFilterMode(Qt::MatchStartsWith);     // Sucht nach Codes, die so beginnen
    ui->aktuelleDiagnoseInput->setCompleter(icdCompleter);  // Dem UI-Feld zuweisen

    // 2. OPS-Code Completer vorbereiten
    QStringList opsList;
    // Wir holen alle eingelesenen OPS-Codes aus dem Backend-Set
    for (const auto& code : hospital->getOpsMap()) {
        opsList << QString::fromStdString(code.first);
    }

    opsCompleter = new QCompleter(opsList, this);
    opsCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    opsCompleter->setFilterMode(Qt::MatchStartsWith);
    ui->opsCodeLineEdit->setCompleter(opsCompleter);        // Dem UI-Feld zuweisen

}

HospitalUI::~HospitalUI()
{
    delete ui;
}

void HospitalUI::on_actionLese_ICD_Texte_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open ICD Catalog", "../../../" , "*.txt");

    if(!fileName.isEmpty()){

        // Explicitly check if the file can be opened cleanly
        std::ifstream file(fileName.toStdString());

        if(!file.is_open()){
            ui->statusbar->showMessage(tr("Fehler: ICD-Datei konnte nicht geoeffnet werden!"),4000);
            return;
        }

        file.close();
        hospital->readICDData(fileName);  // Parse Text file data into your backend database structure
        ui->statusbar->showMessage(tr("ICD_Daten wurde erflogreich geladen"),4000);

    }
}

void HospitalUI::on_actionLese_Aufenthalte_triggered()
{
    //  Check if the ICD map reference is loaded first to match backend rule requirements
    if(!hospital->hasICDData()){
        ui->statusbar->showMessage(tr("***Fehler*** ICD_Daten muessen zuerst geladen werden!"));
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Case File"),"../../../", "*.json");

    if (!fileName.isEmpty()) {
        std::ifstream file(fileName.toStdString());

        if (!file.is_open()) {
            ui->statusbar->showMessage(tr("Fehler: Aufenthaltsdatei konnte nicht gefunden werden!"), 4000);
            return;
        }
        file.close();
        hospital->readCaseFile(fileName.toStdString());  // Parse JSON file data into your backend database structure
        ui->statusbar->showMessage(tr("Aufenhaltsdaten erfolgreich geladen."), 4000);

    }
}

void HospitalUI::updatePatientTree()
{
    ui->patientTreeWidget->clear(); //Clear out any old visual text from the screen layout list

    ui->patientTreeWidget->setColumnCount(1);  // For 1 readable text track
    ui->patientTreeWidget->setHeaderLabel("Patientenstrucktur");


    for(Patient* p : hospital->getPatients()){

        QString pLabel = QString::fromStdString(p->getFirstName() + " " + p->getLastName());

        QTreeWidgetItem* patientBranch = new QTreeWidgetItem(ui->patientTreeWidget);
        patientBranch->setText(0,pLabel);

        // HIDDEN BACKSTAGE TAG (We fetch the clean string copy of the patient's insurance number)
        patientBranch->setData(0, Qt::UserRole, QString::fromStdString(p->getInsuranceNumber()));

        bool digit10_IsOk , digit20_IsOk ;
        int correct10, correct20;

        bool isValidInsurance = checkInsuranceNumber(p->getInsuranceNumber(), digit10_IsOk, digit20_IsOk, correct10, correct20);

        if (!isValidInsurance) {
            patientBranch->setForeground(0,Qt::red);
        }

        // Stays come automatically sorted by date!
        std::vector<Stay*> patientStays = p->getStays();

        // -----------------------------------------------------------------
        // PART B EXTENSION: Track created Total Stay brackets for this patient
        // -----------------------------------------------------------------
        // Maps the shared QUuid to its corresponding UI sub-bracket item pointer
        QMap<QUuid, QTreeWidgetItem*> caseFolderMap;

        int stayIndex = 0;
        bool currentStayIsActive = false;
        bool expandPatientBranch = false;

        for (Stay* s : patientStays) {

            QUuid caseId = s->getTotalStayId(); // Fetch the grouping ID introduced in Part A

            // If we haven't built a UI folder for this unique case chain yet, create it now!
            if(!caseFolderMap.contains(caseId)) {
                QTreeWidgetItem* totalStayBranch = new QTreeWidgetItem(patientBranch);

                // Format a clean, human-readable identifier label (using the first 8 characters of the UUID)
                QString caseLabel = QString("Krankenhausaufenthalt (Fall-ID: %1)").arg(caseId.toString().mid(1, 8).toUpper());
                totalStayBranch->setText(0,caseLabel);

                //  Speichert die echte UUID als versteckte Daten auf Ebene 2!
                totalStayBranch->setData(0, Qt::UserRole, caseId.toString());

                // Save this visual bracket item in our map lookup dictionary
                caseFolderMap[caseId] = totalStayBranch;
            }

            // Fetch the parent case item folder out of our tracking dictionary map
            QTreeWidgetItem* parentCaseFolder = caseFolderMap[caseId];

            // Build the individual Clinic Station row under its parent case folder
            std::string stayInterval = s->getAdmissionDate() + " bis ";

            if (s->getDischargeDate().empty()) {
                stayInterval += "laufend";
                currentStayIsActive = true; // -> Don not have a DischargeDate
                expandPatientBranch = true; // Flag that this entire patient lineage must expand
            } else {
                stayInterval += s->getDischargeDate();
            }

            QTreeWidgetItem* stayBranch = new QTreeWidgetItem(parentCaseFolder); // Nest stay under its Case folder!
            stayBranch->setText(0, QString::fromStdString("Aufenhalte: " + stayInterval));

            // Store the sorted position index value so we can pull it via itemClicked
            stayBranch->setData(0, Qt::UserRole, stayIndex);
            stayIndex++;

            if(currentStayIsActive){
                parentCaseFolder->setExpanded(true); // Open this specific Case Folder branch row
            }
        }

        // If this patient has an ongoing case timeline, expand their top-level node profile row too
        if (expandPatientBranch) {
            patientBranch->setExpanded(true);
        }
    }
}

void HospitalUI::on_actionPatienten_anzeigen_triggered()
{
    // CHECK if there are already patients in the list
    if(hospital->getPatients().empty()){
        ui->statusbar->showMessage("Fehler: Keine Daten vorhanden. Bitte zuerst ICD Dateien laden!");
        return;
    }

    updatePatientTree(); // show the Patientlist
    ui->statusbar->showMessage("Patientenliste erfolgreich gelanden.");
}

string HospitalUI::convertInsuranceLetter(const string &insNum)
{
    if(insNum.empty() || !std::isalpha(insNum[0])) {return insNum;} // CHECK if first Zeichen of Insurance nummer is a letter

    char letter = std::toupper(insNum[0]); // toupper convert letter to uppercase
    int position = letter - 'A' + 1;

    std::string replacement;
    if(position < 10){
        replacement = "0" + std::to_string(position);
    }else{
        replacement = std::to_string(position);
    }
    return replacement + insNum.substr(1);
}

int HospitalUI::calculateMod10(const std::string& digits)
{
    int sum = 0;
    bool multiplyByTwo = false; // Start false because index 0 multiplies by 1

    for (size_t i = 0; i < digits.length(); ++i) {
        int n = digits[i] - '0';  //if digits[i] is '7' (ASCII 55), then 55 - 48 = 7

        // Safety check: ensure we are only calculating numbers
        if (n < 0 || n > 9) continue;
        if (multiplyByTwo) {
            n *= 2;
            if (n > 9) {
                n = (n % 10) + (n / 10);  //quelsumme berechnen
            }
        }

        sum += n;
        multiplyByTwo = !multiplyByTwo; // Toggle for the next digit
    }


    return sum % 10;   // last sum digit
}

bool HospitalUI::checkInsuranceNumber(const std::string& insNum, bool& digit10_IsOk,
                                      bool& digit20_IsOk, int& correct10, int& correct20){

    std::string numericStr = convertInsuranceLetter(insNum);

    if(numericStr.length() < 21){
        digit10_IsOk = false;
        digit20_IsOk = false;
        return false;
    }
    std::string part1 = numericStr.substr(0,10); // start at index 0 with length 10 ( so from index 0 - 9)
    correct10 = calculateMod10(part1);  // correct10 is the prufziffer the insNum muss have
    int actual10 = numericStr[10] - '0';
    digit10_IsOk = (actual10 == correct10);

    std::string part2 = numericStr.substr(0, 20);
    correct20 = calculateMod10(part2);
    int actual20 = numericStr[20] - '0';
    digit20_IsOk = (actual20 == correct20);

    return (digit10_IsOk && digit20_IsOk);
}

void HospitalUI::on_patientTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // Tells the compiler to silence the unused parameter warning safely
    Q_UNUSED(column);

    if (item->parent() != nullptr) { return; }  // Ignore if user double-clicked a stay branch

    // 1. THE HIDDEN EXTRACTION:
    QString hiddenInsNum = item->data(0,Qt::UserRole).toString();
    if (hiddenInsNum.isEmpty()) return;
    std::string insNum = hiddenInsNum.toStdString();


    bool digit10_IsOk = false, digit20_IsOk = false;
    int correct10 = 0, correct20 = 0;

    bool isValid = checkInsuranceNumber(insNum, digit10_IsOk, digit20_IsOk, correct10, correct20);

    if (isValid)  {return;  }

    QString popupMessage = QString("Die Versicherungsnummer %1 ist ungueltig!\n\n").arg(hiddenInsNum);

    // If the personal data section check-digit flag is false, add specific breakdown notes
    if (!digit10_IsOk) {
        popupMessage += QString("-> Ziffer 10 (Pruefziffer Individualteil) ist FALSCH.\n");
        // insNum[9] is the 10th character in the file; correct10 is our computed expectation
        popupMessage += QString("   Aktuell: %1 | Erwartet: %2\n\n").arg(insNum[9]).arg(correct10);
    }

    // If the total string verification check-digit flag is false, add structural breakdown notes
    if (!digit20_IsOk) {
        popupMessage += QString("-> Ziffer 20 (Gesamtpruefziffer) ist FALSCH.\n");
        // insNum[19] is the 20th character in the file; correct20 is our computed expectation
        popupMessage += QString("   Aktuell: %1 | Erwartet: %2\n").arg(insNum[19]).arg(correct20);
    }

    QMessageBox::warning(this, "Pruefziffer-Fehler Details", popupMessage);

}


void HospitalUI::on_patientTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    // Tells the compiler to silence the unused parameter warning safely
    Q_UNUSED(column);

    if (!item) return;

    // -------------------------------------------------------------------------
    // DETERMINING TREE TIERS
    // -------------------------------------------------------------------------
    QTreeWidgetItem* tier2Parent = item->parent();      // Parent of clicked item
    QTreeWidgetItem* tier1Grandparent = nullptr;        // Grandparent of clicked item
    if (tier2Parent) {
        tier1Grandparent = tier2Parent->parent();
    }

    // =========================================================================
    // CASE 1: User clicked Tier 1 (Patient Top-Level Root Row)
    // =========================================================================
    if (tier2Parent == nullptr) {
        // Clear old clinical forms completely
        ui->diagnoseTextEdit->clear();
        ui->aktuelleDiagnoseInput->clear();
        ui->aktuelleDiagnoseText->clear();
        ui->bemerkungenTextEdit->clear();

        ui->anamneseTableWidget->blockSignals(true);
        ui->anamneseTableWidget->clearContents();
        ui->anamneseTableWidget->setRowCount(4);
        ui->anamneseTableWidget->blockSignals(false);

        ui->chartContainerWidget->setChart(new QChart());


        // Extract raw data from patient node
        QVariant insuranceVar = item->data(0, Qt::UserRole);
        if (!insuranceVar.isValid() || insuranceVar.toString().isEmpty()) return;

        std::string searchInsNum = insuranceVar.toString().toStdString();
        Patient* p = hospital->findPatientByInsurance(searchInsNum);

        if (p == nullptr) return;

        // Render Demographic Data
        std::string patientOverview = "=== PATIENTENSTAMMDATEN ===\n";
        patientOverview += "\nName: " + p->getFirstName() + " " + p->getLastName() + "\n";
        patientOverview += "Geburtsdatum: " + p->getBirthDate() + "\n";
        patientOverview += "Versicherungsnummer: " + p->getInsuranceNumber();

        ui->patientInfoTextBrowser->setText(QString::fromStdString(patientOverview));

        ui->stayTabWidget->setEnabled(false);  // Tabs komplett sperren!

        return; // CRITICAL FIX: Stop execution immediately so it doesn't fall through!
    }

    // =========================================================================
    // CASE 2: User clicked Tier 2 (Gesamtaufenthalt Intermediate Folder)
    // =========================================================================
    if (tier2Parent != nullptr && tier1Grandparent == nullptr) {
        // . Force the UI to switch over to the Abrechnung tab page (Index 3)

        ui->stayTabWidget->setEnabled(true);
        ui->stayTabWidget->setCurrentIndex(3);

        // -----------------------------------------------------------------
        //  Alle anderen Tabs in den Initialzustand versetzen
        // -----------------------------------------------------------------

        // Tab 1 (Diagnose und Anamnese) leeren
        ui->diagnoseTextEdit->clear();
        ui->aktuelleDiagnoseInput->clear();
        ui->aktuelleDiagnoseText->clear();
        ui->bemerkungenTextEdit->clear();

        ui->anamneseTableWidget->blockSignals(true);
        ui->anamneseTableWidget->clearContents();
        ui->anamneseTableWidget->setRowCount(4); // Zurück auf Standard-Zeilenanzahl
        ui->anamneseTableWidget->blockSignals(false);

        // Tab 2 (Kurve) leeren: Einen leeren Chart zuweisen
        ui->chartContainerWidget->setChart(new QChart());

        // Tab 3 (Operationen und Prozeduren) leeren
        ui->proceduresTableWidget->clearContents();
        ui->proceduresTableWidget->setRowCount(0);

        // . Resolve Patient pointer via Tier 1 parent node
        std::string searchInsNum = tier2Parent->data(0, Qt::UserRole).toString().toStdString();
        Patient* p = hospital->findPatientByInsurance(searchInsNum);
        if (p == nullptr) return;

        // Extract the targeted Case UUID stored inside this specific folder item
        QUuid folderCaseId = QUuid::fromString(item->data(0, Qt::UserRole).toString());

        //  Gather ONLY the stays that belong to this continuous treatment chain
        std::vector<Stay*> groupedStays;
        for (Stay* s : p->getStays()) {
            if (s->getTotalStayId() == folderCaseId) {
                groupedStays.push_back(s);
            }
        }
        if (groupedStays.empty()) return;

        //  "Die aktuelle Diagnose des letzten Aufenthaltes"
        Stay* lastStay = groupedStays.back();

        std::string diagCode = lastStay->getMainDiagnosisCode();
        std::string diagText = hospital->getICDText(diagCode);

        // . Formatieren: Falls Text vorhanden, hänge ihn in Klammern an, sonst nur den Code anzeigen
        QString fullDiagnosisDisplay = QString("%1 (%2)")
                                           .arg(QString::fromStdString(diagCode))
                                           .arg(QString::fromStdString(diagText));

        ui->diagnosisLineEdit->setText(fullDiagnosisDisplay);

        //  REQUIREMENT: "Die gesamte Aufenthaltsdauer" (Sum of all connected stays)
        long long totalDays = 0;
        for (Stay* s : groupedStays) {
            QDate admission = QDate::fromString(QString::fromStdString(s->getAdmissionDate()), "yyyyMMdd");
            QDate discharge = s->getDischargeDate().empty() ?
                                  QDate::currentDate() :
                                  QDate::fromString(QString::fromStdString(s->getDischargeDate()), "yyyyMMdd");
            totalDays += admission.daysTo(discharge);
        }
        ui->durationLineEdit->setText(QString("%1 Tage").arg(totalDays));

        // . REQUIREMENT: "Alle geplanten oder durchgeführten Operationen und Diagnostiken"
        ui->proceduresListWidget->clear();
        for (Stay* s : groupedStays) {
            for (const auto& proc : s->getProcedures()) {
                QString itemRow = QString("[%1] Am %2 um %3 (%4 Std.) - %5")
                .arg(QString::fromStdString(proc.opsCode))
                    .arg(QString::fromStdString(proc.date))
                    .arg(QString::fromStdString(proc.startTime))
                    .arg(proc.duration)
                    .arg(QString::fromStdString(proc.allocatedResource));
                ui->proceduresListWidget->addItem(itemRow);
            }
        }

        // . REQUIREMENT: "Ein Feld, in dem besondere Erschwernisse eingetragen werden können."
        ui->difficultiesTextEdit->blockSignals(true);
        ui->difficultiesTextEdit->setPlainText(QString::fromStdString(lastStay->getDifficulties()));
        ui->difficultiesTextEdit->blockSignals(false);

        return; // Stop execution immediately!
    }

    // =========================================================================
    // CASE 3: User clicked Tier 3 (Individual Clinic Station Stay Row)
    // =========================================================================
    if (tier1Grandparent != nullptr) {
        ui->stayTabWidget->setEnabled(true);

        // Index 0 ist "Diagnose und Anamnese"
        ui->stayTabWidget->setCurrentIndex(0);

        // Since we are on Tier 3, tier1Grandparent is guaranteed to be the Patient root node!
        std::string searchInsNum = tier1Grandparent->data(0, Qt::UserRole).toString().toStdString();
        Patient* p = hospital->findPatientByInsurance(searchInsNum);

        if (p == nullptr) return;

        // Refresh Demographic Banner automatically for Parent
        // -----------------------------------------------------------------
        std::string patientOverview = "=== PATIENTENSTAMMDATEN ===\n";
        patientOverview += "\nName: " + p->getFirstName() + " " + p->getLastName() + "\n";
        patientOverview += "Geburtsdatum: " + p->getBirthDate() + "\n";
        patientOverview += "Versicherungsnummer: " + p->getInsuranceNumber();

        ui->patientInfoTextBrowser->setText(QString::fromStdString(patientOverview));

        int targetIndex = item->data(0, Qt::UserRole).toInt();

        std::vector<Stay*> sortedStays = p->getStays();

        if (targetIndex >= 0 && targetIndex < static_cast<int>(sortedStays.size())) {
            Stay* clinicalCase = sortedStays[targetIndex];

            // Formulate our 1st Section block (Admission details) requested in Part A
            std::string admLog = "Fachabteilung: " + (clinicalCase->getDepartment() ? clinicalCase->getDepartment()->getName() : "Keine Zuweisung") + "\n";
            admLog += "Aufnahmediagnose Code: " + clinicalCase->getMainDiagnosisCode() + "\n";
            admLog += "Bezeichnung: " + clinicalCase->getMainDiagnosisText();

            // Push text directly to your repurposed read-only text field
            ui->diagnoseTextEdit->setText(QString::fromStdString(admLog));


            // Flush old user inputs from previous tabs
            ui->aktuelleDiagnoseInput->clear();
            ui->aktuelleDiagnoseText->clear();

            // -----------------------------------------------------------------
            // PART B LOADER: Recover and render historical Anamnese Table rows
            // -----------------------------------------------------------------
            ui->anamneseTableWidget->blockSignals(true); // Deactivate loop hazards
            ui->anamneseTableWidget->clearContents();

            const auto& savedCodes = clinicalCase->getAnamneseCodes();

            // Compute exact row allocation needed (minimum 4 rows for space)
            int neededRows = std::max(4, static_cast<int>(savedCodes.size() + 1));
            ui->anamneseTableWidget->setRowCount(neededRows);

            // Rebuild the data grid slots row-by-row
            for (size_t i = 0; i < savedCodes.size(); ++i) {
                std::string code = savedCodes[i];

                // Insert Code Cell (Column 0 - Editable)
                QTableWidgetItem* itemCode = new QTableWidgetItem(QString::fromStdString(code));
                ui->anamneseTableWidget->setItem(i, 0, itemCode);

                // Insert Description Cell (Column 1 - Enforced Read-Only Lock)
                std::string desc = hospital->getICDText(code);
                QTableWidgetItem* itemDesc = new QTableWidgetItem(QString::fromStdString(desc));
                itemDesc->setFlags(itemDesc->flags() & ~Qt::ItemIsEditable);
                ui->anamneseTableWidget->setItem(i, 1, itemDesc);
            }

            // Protect and prepare remaining empty trailing rows
            for (int r = savedCodes.size(); r < ui->anamneseTableWidget->rowCount(); ++r) {
                QTableWidgetItem* lockItem = new QTableWidgetItem();
                lockItem->setFlags(lockItem->flags() & ~Qt::ItemIsEditable);
                ui->anamneseTableWidget->setItem(r, 1, lockItem);
            }
            ui->anamneseTableWidget->blockSignals(false); // Restore tracking alerts

            // -----------------------------------------------------------------
            // PART C LOADER: Recover and render free-form "Bemerkungen" Comments
            // -----------------------------------------------------------------
            ui->bemerkungenTextEdit->blockSignals(true); // Stop loop alerts during loading phases
            ui->bemerkungenTextEdit->setPlainText(QString::fromStdString(clinicalCase->getNotes()));
            ui->bemerkungenTextEdit->blockSignals(false);

            // -----------------------------------------------------------------
            // LOADER: Repaint chronological Line Graphs instantly
            // -----------------------------------------------------------------
            updateChartDisplay(clinicalCase);

            // Aufgabe4//

            ui->proceduresTableWidget->clearContents();
            ui->proceduresTableWidget->setRowCount(0);

            const auto& historyLog = clinicalCase->getProcedures();
            for (size_t i = 0; i < historyLog.size(); ++i) {
                ui->proceduresTableWidget->insertRow(i);
                ui->proceduresTableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(historyLog[i].opsCode)));
                ui->proceduresTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(historyLog[i].date)));
                ui->proceduresTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(historyLog[i].startTime)));
                ui->proceduresTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(historyLog[i].duration)));
                ui->proceduresTableWidget->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(historyLog[i].allocatedResource)));
            }
        }

    }
}



void HospitalUI::on_aktuelleDiagnoseInput_textChanged(const QString &arg1)
{
    std::string typedCode = arg1.toStdString();  // arg1 is the typedcode

    // Call the helper method we added to hospital.cpp to search the icdMap
    std::string description = hospital->getICDText(typedCode);

    if (!description.empty()) {
        ui->aktuelleDiagnoseText->setText(QString::fromStdString(description));
    } else {
        ui->aktuelleDiagnoseText->setText("Code im Katalog unbekannt...");
    }
}


void HospitalUI::on_anamneseTableWidget_cellChanged(int row, int column)
{

    // Rule: We only care if the user modified Column 0 (The ICD-Code input slot)
    if (column != 0) return;

    // CRITICAL: Temporarily disconnect table signals so our programmatic writing
    // inside Column 1 doesn't re-trigger this function into an infinite crash loop!
    ui->anamneseTableWidget->blockSignals(true);

    // Grab the cell item token that the user just typed in
    QTableWidgetItem* codeItem = ui->anamneseTableWidget->item(row, 0);

    if (codeItem != nullptr) {
        // Normalize whatever the user typed (strip spaces, turn to uppercase)
        std::string typedCode = codeItem->text().trimmed().toUpper().toStdString();

        // Re-set the item text to our clean, normalized version
        codeItem->setText(QString::fromStdString(typedCode));

        // Search our dictionary mapping via our backend hospital database helper method
        std::string catalogText = hospital->getICDText(typedCode);

        // Fetch or allocate the description cell right next to it (Column 1)
        QTableWidgetItem* textItem = ui->anamneseTableWidget->item(row, 1);
        if (textItem == nullptr) {
            textItem = new QTableWidgetItem();
            ui->anamneseTableWidget->setItem(row, 1, textItem);
        }

        // Security Lockdown: Force Column 1 to be read-only by removing the Editable flag
        textItem->setFlags(textItem->flags() & ~Qt::ItemIsEditable);

        if (!typedCode.empty() && !catalogText.empty()) {
            // Valid code match found! Fill it in gracefully.
            textItem->setText(QString::fromStdString(catalogText));
            textItem->setForeground(Qt::black); // Standard text color

            // =========================================================================
            // DYNAMIC GROWING LOGIC: If this is the last row, add a new one automatically!
            // =========================================================================
            int totalRows = ui->anamneseTableWidget->rowCount();
            if (row == totalRows - 1) {
                ui->anamneseTableWidget->insertRow(totalRows); // Appends a new blank row at the bottom

                // BULLETPROOF LAYER: Immediately create and lock Column 1 of the brand-new row
                QTableWidgetItem* newLockedItem = new QTableWidgetItem();
                newLockedItem->setFlags(newLockedItem->flags() & ~Qt::ItemIsEditable);
                ui->anamneseTableWidget->setItem(totalRows, 1, newLockedItem);
            }
        }
        else if (typedCode.empty()) {
            // User cleared the code field, so clear the description too
            textItem->setText("");
        }
        else {
            // Validation Warning: Show a clear warning message text string if invalid!
            textItem->setText("WARNUNG: Ungueltiger ICD-Code!");
            textItem->setForeground(Qt::red); // Color it red for clear professional warning
        }
    }



    // =========================================================================
    // PERSISTENCE SAVE ENGINE: Extract all rows and save to active Stay object
    // =========================================================================
    QTreeWidgetItem* currentItem = ui->patientTreeWidget->currentItem();
    if (currentItem != nullptr && currentItem->parent() != nullptr) {
        QString stayText = currentItem->text(0);
        std::string insNum = currentItem->parent()->data(0, Qt::UserRole).toString().toStdString();
        Patient* p = hospital->findPatientByInsurance(insNum);

        if (p != nullptr) {
            for (Stay* clinicalCase : p->getStays()) {
                QString matchPattern = QString::fromStdString(clinicalCase->getAdmissionDate());
                if (stayText.contains(matchPattern)) {

                    clinicalCase->clearAnamneseCodes(); // Reset old collection array
                    // Loop through every row and extract valid typed codes
                    for (int r = 0; r < ui->anamneseTableWidget->rowCount(); ++r) {
                        QTableWidgetItem* item0 = ui->anamneseTableWidget->item(r, 0);
                        if (item0 != nullptr && !item0->text().trimmed().isEmpty()) {
                            clinicalCase->addAnamneseCode(item0->text().trimmed().toUpper().toStdString());
                        }
                    }
                    break;
                }
            }
        }
    }

    // Re-enable interactive table signal operational triggers safely
    ui->anamneseTableWidget->blockSignals(false);
}


void HospitalUI::on_bemerkungenTextEdit_textChanged()
{
    // Find the currently selected row branch item inside our Tree View
    QTreeWidgetItem* currentItem = ui->patientTreeWidget->currentItem();
    if (currentItem == nullptr || currentItem->parent() == nullptr) return; // Guard protection

    // Extract the parent's insurance number to locate the Patient object block
    std::string searchInsNum = currentItem->parent()->data(0, Qt::UserRole).toString().toStdString();
    Patient* p = hospital->findPatientByInsurance(searchInsNum);
    if (p == nullptr) return;

    // Retrieve the target chronological index data marker
    int targetIndex = currentItem->data(0, Qt::UserRole).toInt();
    std::vector<Stay*> sortedStays = p->getStays(); // Leverages your Option 2 sorted engine!

    if (targetIndex >= 0 && targetIndex < static_cast<int>(sortedStays.size())) {
        Stay* clinicalCase = sortedStays[targetIndex];

        // Grab the text typed into the UI box and save it directly into the Stay data block!
        std::string typedNotes = ui->bemerkungenTextEdit->toPlainText().toStdString();
        clinicalCase->setNotes(typedNotes);
    }
}


/* void HospitalUI::on_bemerkungenTextEdit_textChanged()
{
    // 1. Get the currently active row in the tree view
    QTreeWidgetItem* currentItem = ui->patientTreeWidget->currentItem();
    if (currentItem == nullptr || currentItem->parent() == nullptr) return;

    // 2. Extract the stay row's text (e.g., "Aufenthalte: 20260613 bis laufend")
    QString stayText = currentItem->text(0);

    // 3. Find the owner Patient object profile
    std::string insNum = currentItem->parent()->data(0, Qt::UserRole).toString().toStdString();
    Patient* p = hospital->findPatientByInsurance(insNum);
    if (p == nullptr) return;

    // 4. Iterate through the actual stays to find the matching memory address block
    for (Stay* clinicalCase : p->getStays()) {
        QString matchPattern = QString::fromStdString(clinicalCase->getAdmissionDate());

        // If the tree item text contains this stay's admission date, we found our exact match!
        if (stayText.contains(matchPattern)) {

            // Grab the fresh text and save it directly into the stay's private member variable
            std::string typedNotes = ui->bemerkungenTextEdit->toPlainText().toStdString();
            clinicalCase->setNotes(typedNotes);
            break; // Match found, safely exit the loop
        }
    }
}

*/

void HospitalUI::updateChartDisplay(Stay* clinicalCase)
{
    //  Initialize a clean, empty QChart object wrapper
    QChart* chart = new QChart();
    chart->setTitle("Blutdruckverlauf (mmHg)");
    chart->setAnimationOptions(QChart::AllAnimations); // Makes chart lines render with animations

    //  Allocate separate series for both lines
    QLineSeries* systolicSeries = new QLineSeries();
    systolicSeries->setName("Systolisch");
    systolicSeries->setPen(QPen(Qt::red, 3)); // Make systolic line thick and red
    systolicSeries->setPointsVisible(true);

    QLineSeries* diastolicSeries = new QLineSeries();
    diastolicSeries->setName("Diastolisch");
    diastolicSeries->setPen(QPen(Qt::blue, 3)); // Make diastolic line thick and blue
    diastolicSeries->setPointsVisible(true);
    //  Keep track of date labels along our X-Axis categories string index list
    QStringList categories;
    const auto& bpHistory = clinicalCase->getBpHistory();

    int index = 0;
    for (const auto& record : bpHistory) {
        // Map the data points onto a numerical chronological index scale
        systolicSeries->append(index, record.systolic);
        diastolicSeries->append(index, record.diastolic);

        // Convert standard C++ string date over to Qt list
        categories << QString::fromStdString(record.date);
        index++;
    }

    //  Attach the data curves to our master chart manager
    chart->addSeries(systolicSeries);
    chart->addSeries(diastolicSeries);

    //  Configure the Horizontal X-Axis (Chronological Dates)
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    systolicSeries->attachAxis(axisX);
    diastolicSeries->attachAxis(axisX);

    //  Configure the Vertical Y-Axis (Pressure values range in mmHg)
    QValueAxis* axisY = new QValueAxis();
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Druckwert (mmHg)");
    axisY->setRange(40, 220); // Standard safe viewing limits for clinical monitoring
    chart->addAxis(axisY, Qt::AlignLeft);
    systolicSeries->attachAxis(axisY);
    diastolicSeries->attachAxis(axisY);

    // 7. Bind the newly configured chart straight onto your layout container canvas slot!
    ui->chartContainerWidget->setChart(chart);
    ui->chartContainerWidget->setRenderHint(QPainter::Antialiasing); // Smooths jagged line edges
}

void HospitalUI::on_addBPButton_clicked()
{
    // 1. Identify which patient stay item branch is currently selected in our navigation tree
    QTreeWidgetItem* currentItem = ui->patientTreeWidget->currentItem();
    if (currentItem == nullptr || currentItem->parent() == nullptr) {
        QMessageBox::warning(this, "Eingabefehler", "Bitte waehlen Sie zuerst einen gueltigen Aufenthalt aus!");
        return;
    }

    // 2. Resolve target patient profile block data structures via owner insurance key string
    std::string insNum = currentItem->parent()->data(0, Qt::UserRole).toString().toStdString();
    Patient* p = hospital->findPatientByInsurance(insNum);
    if (p == nullptr) return;

    // 3. Grab corresponding stay instance from our pre-sorted list collection pointer matching row ID
    int targetIndex = currentItem->data(0, Qt::UserRole).toInt();
    std::vector<Stay*> sortedStays = p->getStays();

    if (targetIndex >= 0 && targetIndex < static_cast<int>(sortedStays.size())) {
        Stay* clinicalCase = sortedStays[targetIndex];

        // 4. Extract data values directly from the user interface widgets
        std::string inputDate = ui->bpDateEdit->date().toString("dd.MM.yyyy").toStdString();
        int sysValue = ui->systolicSpinBox->value();
        int diaValue = ui->diastolicSpinBox->value();

        // 5. Append the record safely into our modern backend vector smart-pointer structure layer!
        clinicalCase->addBloodPressureRecord(inputDate, sysValue, diaValue);

        // 6. Refresh the chart canvas area instantly to show the newly added line metrics!
        updateChartDisplay(clinicalCase);
    }
}

void HospitalUI::on_transferPatientButton_clicked()
{
    QTreeWidgetItem* currentItem = ui->patientTreeWidget->currentItem();
    if (!currentItem) return;

    // Traverse up to verify a Tier 3 individual stay station row was selected
    QTreeWidgetItem* tier2Parent = currentItem->parent();
    if (!tier2Parent) return;
    QTreeWidgetItem* tier1Grandparent = tier2Parent->parent();
    if (!tier1Grandparent) return;

    std::string insNum = tier1Grandparent->data(0, Qt::UserRole).toString().toStdString();
    int stayIndex = currentItem->data(0, Qt::UserRole).toInt();

    Patient* p = hospital->findPatientByInsurance(insNum);
    if (!p) return;

    Stay* selectedStay = p->getStays()[stayIndex];

    // Guard Clause: Cannot transfer someone from a case that is already closed!
    if (!selectedStay->getDischargeDate().empty()) {
        QMessageBox::warning(this, "Fehler", "Patienten können nur aus laufenden Aufenthalten verlegt werden.");
        return;
    }

    // Convert QDate safely to text string payload format
    std::string actionDate = ui->transferDateEdit->date().toString("yyyyMMdd").toStdString();

    int deptIndex = ui->targetDepartmentComboBox->currentData().toInt();
    DepartmentType targetType = static_cast<DepartmentType>(deptIndex);
    Department* targetDept = hospital->getDepartments().at(targetType).get();

    if (selectedStay->getDepartment() == targetDept) {
        QMessageBox::warning(this, "Fehler", "Patient ist bereits in dieser Fachabteilung.");
        return;
    }

    static long globalStayIDCounter = 9000;
    p->transferPatient(selectedStay, targetDept, actionDate, globalStayIDCounter++);

    updatePatientTree(); // Re-render tree structure layout view automatically
    QMessageBox::information(this, "Erfolg", "Patient wurde erfolgreich verlegt.");
}


void HospitalUI::on_dischargePatientButton_clicked()
{
    QTreeWidgetItem* currentItem = ui->patientTreeWidget->currentItem();
    if (!currentItem) return;

    QTreeWidgetItem* tier2Parent = currentItem->parent();
    if (!tier2Parent) return;
    QTreeWidgetItem* tier1Grandparent = tier2Parent->parent();
    if (!tier1Grandparent) return;

    std::string insNum = tier1Grandparent->data(0, Qt::UserRole).toString().toStdString();
    int stayIndex = currentItem->data(0, Qt::UserRole).toInt();

    Patient* p = hospital->findPatientByInsurance(insNum);
    if (!p) return;

    Stay* selectedStay = p->getStays()[stayIndex];

    if (!selectedStay->getDischargeDate().empty()) {
        QMessageBox::warning(this, "Fehler", "Dieser Aufenthalt ist bereits beendet.");
        return;
    }

    std::string actionDate = ui->transferDateEdit->date().toString("yyyyMMdd").toStdString();

    // Execute total hospital discharge tracking assignment
    p->dischargePatient(selectedStay, actionDate);

    updatePatientTree(); // Re-render updates
    QMessageBox::information(this, "Erfolg", "Patient wurde erfolgreich aus dem Krankenhaus entlassen.");
}

void HospitalUI::on_scheduleProcedureButton_clicked()
{
    // 1. Resolve currently highlighted active stay context
    QTreeWidgetItem* currentItem = ui->patientTreeWidget->currentItem();
    if (!currentItem || !currentItem->parent() || !currentItem->parent()->parent()) {
        QMessageBox::warning(this, "Planungsfehler", "Bitte wählen Sie zuerst einen gültigen Aufenthalt aus.");
        return;
    }

    std::string insNum = currentItem->parent()->parent()->data(0, Qt::UserRole).toString().toStdString();
    int stayIndex = currentItem->data(0, Qt::UserRole).toInt();

    Patient* p = hospital->findPatientByInsurance(insNum);
    if (!p) return;

    Stay* selectedStay = p->getStays()[stayIndex];

    // Check if case is closed already
    if (!selectedStay->getDischargeDate().empty()) {
        QMessageBox::warning(this, "Planungsfehler", "Für bereits beendete Aufenthalte können keine Termine gebucht werden.");
        return;
    }

    // Eingaben aus dem UI auslesen
    std::string opsStr = ui->opsCodeLineEdit->text().trimmed().toStdString();
    if (opsStr.empty()) {
        QMessageBox::warning(this, "Eingabefehler", "Bitte geben Sie einen gültigen OPS-Code ein.");
        return;
    }

    //Check if the code is valid (exit in our opsCode.txt)
    if (!hospital->isValidOPSCode(opsStr)) {
        QMessageBox::critical(this, "Validierungsfehler",
                              QString::fromStdString("Der eingegebene OPS-Code '" + opsStr + "' ist ungültig oder existiert nicht im System!"));
        return; // Stop the booking process completely!
    }

    std::string dateStr = ui->procedureDateEdit->date().toString("yyyyMMdd").toStdString();
    int duration = ui->durationSpinBox->value();

    // Aktuelle Fachabteilung des Patienten auslesen
    std::string currentDept = selectedStay->getDepartment() ? selectedStay->getDepartment()->getName() : "";


    // =========================================================================
    //AUFGABE 3(f): Klassifizierung des OPS-Codes & Plausibilitätsprüfungen
    // =========================================================================

    // Helper checks to handle Umlaut spelling discrepancies dynamically
    bool isOrthopedics = (currentDept == "Orthopädie" || currentDept == "Orthopaedie");
    bool isSurgery = (currentDept == "Allgemeine Chirurgie" || currentDept == "Allgemeine Chirugie");

    // =========================================================================
    // TARGETED 2-DIGIT CLASSIFICATION: Checks digits immediately after "-"
    // =========================================================================
    bool isOperation = false;
    bool isMRI = false;
    bool isCT = false;

    size_t hyphenPos = opsStr.find('-');

    if (hyphenPos != std::string::npos && opsStr.length() > hyphenPos + 2) {
        std::string nextTwoDigits = opsStr.substr(hyphenPos + 1, 2);

        try {
            int X = std::stoi(nextTwoDigits);

            // --- 1. Check for Surgery (5-01 <= X <= 5-99) ---
            if (opsStr[0] == '5') {
                if (X >= 1 && X < 99) {
                    isOperation = true;
                }
            }
            // --- 2. Check for MRI (3-80 <= X <= 3-84) ---
            else if (opsStr.rfind("3-8", 0) == 0) {
                if (X >= 80 && X < 84) {
                    isMRI = true;
                }
            }
            // --- 3. Check for CT (3-20 <= X <= 3-26) ---
            else if (opsStr.rfind("3-2", 0) == 0) {
                if (X >= 20 && X < 26) {
                    isCT = true;
                }
            }
        }
        catch (...) {
            // Safety fallback
        }
    }

    // =========================================================================
    // RULES VERIFICATION
    // =========================================================================

    int resourceIndex = ui->resourceTypeComboBox->currentIndex(); // 0 = OP, 1 = MRT, 2 = CT

    // --- Rule 1: Surgery can only be scheduled in General Surgery or Orthopedics ---
    if (isOperation) {
        // Strict Cross-Check: Prevent booking a "5-" code with "MRT" or "CT" categories!
        if (resourceIndex != 0) {
            QMessageBox::critical(this, "Validierungsfehler",
                                  "Operationen (OPS-Codes beginnend mit 5-) müssen zwingend mit der Kategorie 'Operationssaal' gebucht werden!");
            return;
        }

        if (!isSurgery && !isOrthopedics) {
            QMessageBox::critical(this, "Planungsfehler",
                                  "Operationen (OPS 5-01 bis 5-99) dürfen nur in 'Allgemeine Chirurgie' oder 'Orthopädie' geplant werden!");
            return;
        }
    }
    // --- Rule 2: MRIs and CTs can be scheduled anywhere EXCEPT General Surgery ---
    else if (isMRI || isCT) {
        if (isSurgery) {
            QMessageBox::critical(this, "Planungsfehler",
                                  "MRT- und CT-Planungen sind in der 'Allgemeinen Chirurgie' nicht zulässig!");
            return;
        }

        // Match ComboBox Category checks
        if (isMRI && resourceIndex != 1) {
            QMessageBox::warning(this, "Eingabefehler", "Für eine MRT-Prozedur müssen Sie die Kategorie 'MRT' wählen!");
            return;
        }
        if (isCT && resourceIndex != 2) {
            QMessageBox::warning(this, "Eingabefehler", "Für eine CT-Prozedur müssen Sie die Kategorie 'CT' wählen!");
            return;
        }
    }
    // --- Rule 3: Other procedures can be scheduled anywhere EXCEPT General Surgery ---
    else {
        if (isSurgery) {
            QMessageBox::critical(this, "Planungsfehler",
                                  "In der 'Allgemeinen Chirurgie' dürfen keine sonstigen Prozeduren geplant werden!");
            return;
        }


    }

    // =========================================================================
    // 3. Buchungsprozess starten (Unterscheidung: Zentralressource nötig oder nicht)
    // =========================================================================
    std::string timeDisplayStr;
    std::string resourceName;

    if (isOperation || isMRI || isCT) {
        // Benötigt eine physische Zentralressource aus dem Krankenhaus-Pool
        int resourceIndex = ui->resourceTypeComboBox->currentIndex();
        ResourceType resourceType = static_cast<ResourceType>(resourceIndex);

        HospitalResource* allocatedAsset = hospital->allocateFirstAvailableResource(
            resourceType, dateStr, duration, opsStr, p->getPatientID()
            );

        if (!allocatedAsset) {
            QMessageBox::critical(this, "Ressourcenkonflikt",
                                  "Zu diesem Datum steht keine freie Zentralressource mit ausreichender Kapazität zur Verfügung!");
            return;
        }

        int bookedHour = allocatedAsset->getConfirmedBookings().back().startHour;
        timeDisplayStr = std::to_string(bookedHour) + ":00 Uhr";
        resourceName = allocatedAsset->getIdName();
    }
    else {
        // Sonstige Prozedur: Benötigt KEINE zentrale Ressource und wird einfach eingetragen
        timeDisplayStr = "08:00 Uhr (Standard)"; // Dummy-Uhrzeit für rein dokumentarische Zwecke
        resourceName = "Keine Zentralressource benötigt"; //
    }

    //  Erfolgreiche Buchung im Modell abspeichern
    ProcedureLog logEntry{opsStr, dateStr, timeDisplayStr, duration, resourceName};
    selectedStay->addProcedure(logEntry);

    //  Die Tabelle auf dem Tab sofort aktualisieren
    int rowCount = ui->proceduresTableWidget->rowCount();
    ui->proceduresTableWidget->insertRow(rowCount);
    ui->proceduresTableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString::fromStdString(opsStr)));
    ui->proceduresTableWidget->setItem(rowCount, 1, new QTableWidgetItem(QString::fromStdString(dateStr)));
    ui->proceduresTableWidget->setItem(rowCount, 2, new QTableWidgetItem(QString::fromStdString(timeDisplayStr)));
    ui->proceduresTableWidget->setItem(rowCount, 3, new QTableWidgetItem(QString::number(duration)));
    ui->proceduresTableWidget->setItem(rowCount, 4, new QTableWidgetItem(QString::fromStdString(resourceName)));

    QMessageBox::information(this, "Buchung Bestätigt",
                             QString::fromStdString("Erfolgreich eingetragen: " + opsStr + " für " + timeDisplayStr + " (" + resourceName + ")"));
}

void HospitalUI::on_difficultiesTextEdit_textChanged()
{
    QTreeWidgetItem* currentItem = ui->patientTreeWidget->currentItem();
    if (!currentItem) return;

    QTreeWidgetItem* tier2Parent = currentItem->parent();
    QTreeWidgetItem* tier1Grandparent = nullptr;
    if (tier2Parent) {
        tier1Grandparent = tier2Parent->parent();
    }

    // If we are looking at a Tier 2 folder
    if (tier2Parent != nullptr && tier1Grandparent == nullptr) {
        std::string searchInsNum = tier2Parent->data(0, Qt::UserRole).toString().toStdString();
        Patient* p = hospital->findPatientByInsurance(searchInsNum);
        if (!p) return;

        QUuid folderCaseId = QUuid::fromString(currentItem->data(0, Qt::UserRole).toString());

        // Save the text to the last stay belonging to this Case UUID group
        std::vector<Stay*> patientStays = p->getStays();
        for (auto it = patientStays.rbegin(); it != patientStays.rend(); ++it) {
            if ((*it)->getTotalStayId() == folderCaseId) {
                (*it)->setDifficulties(ui->difficultiesTextEdit->toPlainText().toStdString());
                break;
            }
        }
    }
}

