#include "patient.h"
#include <algorithm>
#include <cctype>

Patient::Patient(long id, string birth, string fName, string lName, string insurance, Gender g)
    : patientID(id), birthDate(birth), firstName(fName), lastName(lName),
    insuranceNumber(insurance), gender(g)  {}
long Patient::getPatientID() const{ return patientID;}

string Patient::getInsuranceNumber() const{ return insuranceNumber;}

void Patient::addStay(std::shared_ptr<Stay> s)
{
    stays.push_back(s);
}

std::vector<Stay *> Patient::getStays() const  // sorted stay
{
    std::vector<Stay*> sortedList;

    for (const auto& s : stays) {
        sortedList.push_back(s.get());
    }

    std::sort(sortedList.begin(), sortedList.end(), [](const Stay* a, const Stay* b) {
        return a->getAdmissionDate() < b->getAdmissionDate();
    });

    return sortedList;
}

// const std::vector<std::unique_ptr<Stay> >& Patient::getStays() const  {
//     return stays;
// }

string Patient::getFirstName() const{return firstName;}

string Patient::getLastName() const{return lastName;}

string Patient::getBirthDate() const{return birthDate;}

const std::vector<std::shared_ptr<Stay> > &Patient::getSharedStays() const
{
    return stays;
}

string Patient::showOverview()
{
    //std::string gStr = (gender == Gender::male ? "männlich" : (gender == Gender::female ? "weiblich" : "divers"));
    string gStr;
    if(gender == Gender::male){
        gStr = "maennlich";
    }else if (gender == Gender::female){
        gStr = "weiblich";
    }else{
        gStr = "divers";  // this correspond to Gender::nonBinary
    }
    string result = "Patient*in " + std::to_string(patientID) + ": " + firstName + lastName;
    result += ", Versicherungsnummer: " + insuranceNumber + "\n";
    result += "Geschlecht: " + gStr + ", Geburtsdatum: " + birthDate + "\nAufenhalte:\n";

    for(const auto& s : stays){  // auto -> std::unique_ptr<Stay>&
        result += s->showDetails();
    }
    return result;
}


void Patient::transferPatient(Stay* activeStay, Department* targetDept, const std::string& transferDate, long nextStayID)
{
    if (!activeStay || !targetDept) return;

    //  Close the current active stay
    activeStay->setDischargeDate(transferDate);

    //  Spawn the new stay at the target department inheriting data profiles
    auto newStay = std::make_shared<Stay>(
        this->patientID,
        nextStayID,
        transferDate,   // Admission date matches previous discharge date
        "",             // Empty discharge date means it is ongoing ("laufend")
        activeStay->getMainDiagnosisCode(),
        activeStay->getMainDiagnosisText(),
        targetDept
        );

    //  Keep them bundled under the EXACT SAME case group bracket folder ID
    newStay->setTotalStayId(activeStay->getTotalStayId());

    // 4. Wire the required smart pointer sequential chain tracking links
    for (const auto& s : stays) {
        if (s.get() == activeStay) {
            newStay->setPredecessor(s);
            s->setSuccessor(newStay);
            break;
        }
    }

    stays.push_back(newStay);
}

void Patient::dischargePatient(Stay* activeStay, const std::string& dischargeDate)
{
    if (!activeStay) return;

    // Simply stamp the definitive checkout date to end the hospitalization timeline
    activeStay->setDischargeDate(dischargeDate);
}

bool Patient::validateInsuranceNumber(const string &insuranceNumber)
{
    // Must be exactly 20 characters long
    if (insuranceNumber.length() != 20) {
        return false;
    }

    //  The first character must be an uppercase letter (A-Z)
    if (!std::isupper(static_cast<unsigned char>(insuranceNumber[0]))) {
        return false;
    }

    //  All remaining 19 characters must be numeric digits (0-9)
    for (size_t i = 1; i < insuranceNumber.length(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(insuranceNumber[i]))) {
            return false;
        }
    }

    return true;
}