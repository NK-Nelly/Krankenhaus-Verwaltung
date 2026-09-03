#include <iostream>
#include "hospital.h"
#include "patient.h"
#include "department.h"
#include "json.hpp"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDate>

#include "generalsurgery.h"
#include "cardiology\cardiology.h"
#include "internalmedicine.h"
#include "orthopedics.h"
#include "oncology.h"
#include "gastroenterology.h"

//#include <sstream>

using json = nlohmann::json ;

std::unordered_map<string, string> Hospital::getIcdMap() const
{
    return icdMap;
}

std::unordered_map<string, string> Hospital::getOpsMap() const
{
    return opsMap;
}

Hospital::Hospital(string n) : name(n) {
    // Abteilungen auf dem Heap anlegen und die Map befüllen
    departments[DepartmentType::GeneralSurgery] = std::make_unique<GeneralSurgery>();
    departments[DepartmentType::Cardiology] = std::make_unique<Cardiology>();
    departments[DepartmentType::InternalMedicine] = std::make_unique<InternalMedicine>();
    departments[DepartmentType::Orthopedics] = std::make_unique<Orthopedics>();
    departments[DepartmentType::Oncology] = std::make_unique<Oncology>();
    departments[DepartmentType::Gastroenterolgy] = std::make_unique<Gastroenterology>();

    std::cout << "Krankenhaus-Konstruktor: Alle 6 Fachabteilungen wurden erfolgreich initialisiert.\n";

    // =========================================================================
    // AUFGABE 4(b) RESOURCE ALIGNMENT POOL
    // =========================================================================
    centralResources.push_back(std::make_unique<OperatingRoom>("OP-Saal A"));
    centralResources.push_back(std::make_unique<OperatingRoom>("OP-Saal B"));

    centralResources.push_back(std::make_unique<MRIScanner>("MRT-Gerät 1"));
    centralResources.push_back(std::make_unique<MRIScanner>("MRT-Gerät 2"));

    centralResources.push_back(std::make_unique<CTScanner>("CT-Scanner X"));
    centralResources.push_back(std::make_unique<CTScanner>("CT-Scanner Y"));

}

Hospital::~Hospital() = default;



std::vector<Patient *> Hospital::getPatients() const
{
    std::vector<Patient*> out;
    for(auto& p : patients){
        out.push_back(p.get());
    }
    return out;
}

string Hospital::getICDText(const string &code) const
{
    // Search through the private map keys for a matching ICD string node position
    auto iterator = icdMap.find(code);

    // If the iterator did not hit the end of the map, it successfully found the code!
    if (iterator != icdMap.end()) {
        return iterator->second; // Return the value side of the pair (the text description)
    }

    // If the code wasn't found in the dictionary catalog, return an empty string fallback
    return "";
}


const std::map<DepartmentType, std::unique_ptr<Department> > &Hospital::getDepartments() const
{
    return departments;
}


const std::vector<std::unique_ptr<HospitalResource>>& Hospital::getCentralResources() const
{
    return centralResources;
}

string Hospital::getOPSText(const string &code) const
{
    auto it = opsMap.find(code);
    if (it != opsMap.end()) {
        return it->second; // Return the matching text description string
    }
    return "";
}


void Hospital::readICDData(const QString& fileName)
{

    QFile file(fileName); //  Create a QFile object bound to our resource path

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        std::cerr << fileName.toStdString() << " kann nicht geoeffnet werden\n"; return;
    }

    QTextStream in(&file);

    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList col = line.split(';');

        if(col.size() >= 9){
            icdMap[col[5].toStdString()] = col[8].toStdString();
        }
    }
    std::cout <<"ICD_Daten wurde erfolgreich geladen.\n";
    file.close();
}

void Hospital::readCaseFile(string fileName)
{
    //Check if the ICD_Data was called already
    if(!hasICDData()){
        std::cerr << "***Fehler*** ICD_Daten muessen zuerst geladen werden!\n"; return;
    }

    QFile qfile(QString::fromStdString(fileName));

    if(!qfile.open(QIODevice::ReadOnly | QIODevice::Text)){
        std::cerr << fileName <<" kann nicht geoeffnet werden\n"; return;
    }

    // Read entire file into a QString
    QString jsonText = qfile.readAll();
    qfile.close(); // Safely close file after parsing succeeds

    json j;
    try{
        // Convert QString → std::string → parse JSON
        j = json::parse(jsonText.toStdString());

    }catch(const json::parse_error& e){
        std::cerr << "\n*** JSON FORMAT FEHLER ***: DIE JSON_Datei ist fehlerhaft modifiziert oder beschaedigt\n";
        std::cerr <<"Details: " << e.what() << std::endl;
        return;       // Abort data processing, return to console menu smoothly
    }

    //Static Counters
    static long nextPatientID = 1;
    static long nextStayID = 1;

    for(auto& element : j){
        string insNum = element ["insuranceNumber"];

        Patient* p = findPatientByInsurance(insNum);

        if(p == nullptr){
            Gender gender;
            //Determine the gender from the JSON string
            string genderStr = element["gender"];
            if(genderStr == "M"){
                gender = Gender::male;
            }else if(genderStr == "W"){
                gender = Gender::female;
            }else{
                gender = Gender::nonBinary;
            }
            // neu Patient anlegen (Use std::make_unique instead of raw 'new')
            auto newPatient = std::make_unique<Patient>(nextPatientID++, element["birthDate"], element["firstName"],element["lastName"],insNum, gender);
            // Keep a temporary raw address location so we can attach stays to it below
            p = newPatient.get();
            patients.push_back(std::move(newPatient));
        }

        // Stay anlegen und verknuefen
        string code = element["mainDiagnosis"];
        string diagnosisText = icdMap[code];
        string admDate = element["admissionDate"];
        string disDate = element["dischargeDate"];
        int deptTypeInt =  element["departmentType"];

        DepartmentType deptType = static_cast<DepartmentType>(deptTypeInt);
        //passende Abteilung aus Hospital::departments holen

        Department* deptPtr = departments[deptType].get();
        auto newStay = std::make_shared<Stay>(p->getPatientID(), nextStayID++, admDate, disDate, code, diagnosisText, deptPtr);
        // =====================================================================
        // AUFGABE 2 TIMELINE LINKER
        // =====================================================================

        const auto& activeHistory = p->getSharedStays();
        if (!activeHistory.empty()) {
            std::shared_ptr<Stay> lastStay = activeHistory.back();

            // If the last stay is ongoing, or if this stay directly continues it
            //if (lastStay->getDischargeDate().empty() || lastStay->getDischargeDate() == admDate) {

             // Overwrite this stay's default random UUID with the existing case group UUID
             //  newStay->setTotalStayId(lastStay->getTotalStayId());

                // Establish the weak_ptr bidirectional chain connections
                newStay->setPredecessor(lastStay);
                lastStay->setSuccessor(newStay);
           // }
        }

        /* if (!activeHistory.empty()) {
    std::shared_ptr<Stay> lastStay = activeHistory.back();

    // Parse using the exact format from your stays.json
    QDate lastDischarge = QDate::fromString(QString::fromStdString(lastStay->getDischargeDate()), "yyyyMMdd");
    QDate newAdmission   = QDate::fromString(QString::fromStdString(admDate), "yyyyMMdd");

    // Check if the previous stay is running OR if it's a consecutive day transfer
    bool isOngoing = lastStay->getDischargeDate().empty() || lastStay->getDischargeDate() == "";
    bool isConsecutive = lastDischarge.isValid() && newAdmission.isValid() && lastDischarge.daysTo(newAdmission) <= 1;

    if (isOngoing || isConsecutive) {
        // Overwrite the new stay's ID with the existing group ID!
        newStay->setTotalStayId(lastStay->getTotalStayId());

        // Establish the double linked list connections
        newStay->setPredecessor(lastStay);
        lastStay->setSuccessor(newStay);
    }
}
*/



        // 4. Append into the patient container (Ensure Patient::addStay does NOT use std::move!)
        p->addStay(newStay);
    }


std::cout << "Aufenhaltsdaten wurde erfolgreich geladen. \n";
}

//****** MENU******
void Hospital::userDialog()
{
    int choice;
    while(true){
        std::cout << "\n1:Lese Aufenthaltsdatei \n2:Lese ICD-Daten \n3:Liste aller Daten\n0:Beende\nAuswahl: ";
        std::cin >> choice;

        //Eingabe pruffer (bei Zeichen)
        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Bitte Zahl eingeben" << std::endl; continue;
        }
        switch (choice) {
        case 1: readCaseFile("stay.json"); break;
        case 2: readICDData("icdCodes.txt"); break;
        case 3: printPatientList(); break;
        case 0: std::cout << "Programm beendet. Auf Wiedersehen!" << std::endl; return ;   // Program ends by 0_input
        default: std::cout <<"Ungueltige Eingabe, bitte erneut versuchen.\n"; break;
        }
    }
}

void Hospital::printPatientList()
{
    // Check if the list is empty
    if (patients.empty()) {
        std::cout << "\nDie Patientenliste ist leer. Bitte laden Sie zuerst Daten.\n";
        return; //
    }
    // Print PatientList
    for(auto& p : patients){
        std::cout <<   "\n----------------------------------------\n";
        std::cout << p->showOverview();
        std::cout << std::endl;
    }
}

bool Hospital::hasICDData() const
{
    return (!icdMap.empty());
}

Patient *Hospital::findPatientByInsurance(const string &insNur)
{
    for(const auto& existing : patients){
        if(existing->getInsuranceNumber() == insNur){
            return existing.get();  //found matching patient
        }
    }
    return nullptr; // No patient found with this insurance number
}

bool Hospital::readOPSCodes(const QString &filename)
{
    QFile file(filename);

    // Attempt to open the resource file container in read-only text mode
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error: Could not open embedded OPS file: "
                  << filename.toStdString() << std::endl;
        return false;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList col = line.split(';');

        if (col.size() >= 9) {
            opsMap[col[6].toStdString()] = col[8].toStdString();
        }

    }
    std::cout <<"OPS_Daten wurde erfolgreich geladen.\n";

    file.close(); // Cleanly release the file handle
    return true;
}

HospitalResource *Hospital::allocateFirstAvailableResource(ResourceType targetType, const string &date, int duration, const string &ops, long pID)
{
    for (const auto& res : centralResources) {
        if (res->getType() == targetType) {
            int confirmedHour = res->tryBookSlot(date, duration, ops, pID);
            if (confirmedHour != -1) {
                return res.get(); // Found a free slot and locked it! Return instance address
            }
        }
    }
    return nullptr; // Booked out completely across all matching resource machinery!
}

bool Hospital::isValidOPSCode(const string &code) const
{
    // Check if the trimmed code exists in your loaded OPS dictionary map
    return opsMap.find(code) != opsMap.end();
}