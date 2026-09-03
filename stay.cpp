#include "stay.h"
#include "department.h"
// Include all 6 sub-classes
#include "generalsurgeryinfo.h"
#include "cardiologyinfo.h"
#include "internalmedicineinfo.h"
#include "orthopedicsinfo.h"
#include "oncologyinfo.h"
#include "gastroenterologyinfo.h"


void Stay::setDifficulties(const string &newDifficulties)
{
    difficulties = newDifficulties;
}

string Stay::getDifficulties() const
{
    return difficulties;
}

Stay::Stay(long pID, long sID, string admDate, string disDate, string code, string text, Department* dept)
    :patientID(pID),stayID(sID), admissionDate(admDate), dischargeDate(disDate),
    mainDiagnosisCode(code), mainDiagnosisText(text), department(dept), notes(""), difficulties("") // Initialized cleanly as empty text!
{
    // Generates a random, non-zero unique ID by default on allocation
    this->totalStayId = QUuid::createUuid();

    // Mapping specialized data containers based on your existing department assignments
    if (dept != nullptr) {
        std::string deptName = dept->getName();

        if (deptName == "Allgemeine Chirurgie" || deptName == "GeneralSurgery") {
            specializedInfo = std::make_unique<GeneralSurgeryinfo>();
        } else if (deptName == "Kardiologie" || deptName == "Cardiology") {
            specializedInfo = std::make_unique<CardiologyInfo>();
        } else if (deptName == "Innere Medizin" || deptName == "Internal Medicine") {
            specializedInfo = std::make_unique<InternalMedicineInfo>();
        } else if (deptName == "Orthopädie" || deptName == "Orthopedics") {
            specializedInfo = std::make_unique<OrthopedicsInfo>();
        } else if (deptName == "Onkologie" || deptName == "Oncology") {
            specializedInfo = std::make_unique<OncologyInfo>();
        } else if (deptName == "Gastroenterologie" || deptName == "Gastroenterology") {
            specializedInfo = std::make_unique<GastroenterologyInfo>();
        }
    }

}


Department *Stay::getDepartment() const
{
    return department;
}

string Stay::getAdmissionDate() const{ return admissionDate;}

string Stay::getDischargeDate() const{ return dischargeDate;}

string Stay::getMainDiagnosisText() const{return mainDiagnosisText;}

string Stay::getMainDiagnosisCode() const{ return mainDiagnosisCode;}

string Stay::getNotes() const{return notes;}

void Stay::setNotes(const std::string &newNotes){ notes = newNotes;}

void Stay::addBloodPressureRecord(const string &date, int sys, int dia)
{
    BloodPressureRecord record = { date, sys, dia };
    bpHistory.push_back(record);
}

void Stay::addAnamneseCode(const string &code)
{
    anamneseCodes.push_back(code);
}

std::vector<BloodPressureRecord> Stay::getBpHistory() const
{
    return bpHistory;
}

std::vector<string> Stay::getAnamneseCodes() const
{
    return anamneseCodes;
}

QUuid Stay::getTotalStayId() const
{
    return totalStayId;
}

ClinicalInfo*Stay::getSpecializedInfo() const
{
    return specializedInfo.get();
}

void Stay::setSpecializedInfo(std::unique_ptr<ClinicalInfo> info)
{
    specializedInfo = std::move(info);
}


void Stay::setTotalStayId(const QUuid &id)
{
    totalStayId = id ;
}

string Stay::showDetails()
{
    string res = std::to_string(stayID) + ": von " + admissionDate + " bis ";
    res += (dischargeDate.empty()? "laufend" : dischargeDate) + "\n";
    res += "Aufnahmediagnose: " + mainDiagnosisCode + " " + mainDiagnosisText + "\n";
    res +="Abteilung: " + department->getName() + "\n";

    return res;
}

std::shared_ptr<Stay> Stay::getPredecessor() const
{
    // uses .lock() to safely convert back to std::shared_ptr (returns nullptr if expired).
    return predecessor.lock();
}

void Stay::setPredecessor(std::shared_ptr<Stay> pred)
{
    predecessor = pred;
}

std::shared_ptr<Stay> Stay::getSuccessor() const
{
    return successor.lock();
}

void Stay::setSuccessor(std::shared_ptr<Stay> succ)
{
   successor = succ;
}


void Stay::setDischargeDate(const string &newDischargeDate)
{
    dischargeDate = newDischargeDate;
}

void Stay::addProcedure(const ProcedureLog &log)
{
    procedureHistory.push_back(log);
}

const std::vector<ProcedureLog> &Stay::getProcedures() const
{
    return procedureHistory;
}