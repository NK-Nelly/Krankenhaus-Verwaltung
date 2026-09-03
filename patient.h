#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <vector>
#include <memory>
#include "Gender.h"
#include "stay.h"

using std::string;

class Patient {
private:
    long patientID;
    string birthDate;
    string firstName;
    string lastName;
    string insuranceNumber;
    Gender gender;

    std::vector<std::shared_ptr<Stay>> stays; // Liste von Aufenthalten

public:
    Patient(long id, string birth, string fName, string lName, string insurance, Gender g);
    string showOverview();
    long getPatientID() const;
    string getInsuranceNumber() const;
    void addStay(std::shared_ptr<Stay> s);
    ~Patient() = default;
    std::vector<Stay*> getStays() const;
    // const std::vector<std::unique_ptr<Stay> >& getStays() const;
    string getFirstName() const;
    string getLastName() const;
    string getBirthDate() const;

   const  std::vector<std::shared_ptr<Stay>>& getSharedStays() const  ;

    void transferPatient(Stay* activeStay, Department* targetDept, const std::string& transferDate, long nextStayID);
    void dischargePatient(Stay* activeStay, const std::string& dischargeDate);

   static bool validateInsuranceNumber(const std::string& insuranceNumber);
};

#endif // PATIENT_H
