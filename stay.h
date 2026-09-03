#ifndef STAY_H
#define STAY_H

#include <string>
#include <QtCharts/QChart> // Add this include at the top!
#include <QUuid>    // Required for Case UUIDs
#include <memory>  // Required for smart pointers

#include "clinicalinfo.h"

class Department;  // Forward Declarartion

using std::string ;

// Structure to represent a single blood pressure measurement entry
struct BloodPressureRecord {
    std::string date;
    int systolic;
    int diastolic;
};

struct ProcedureLog {
    std::string opsCode;
    std::string date;
    std::string startTime;
    int duration;
    std::string allocatedResource;
};

class Stay {
private:
    long patientID;
    long stayID;
    string admissionDate;
    string dischargeDate;
    string mainDiagnosisCode;
    string mainDiagnosisText;
    Department* department;
    std::string notes; // for the Bemerkung in UI

    std::vector<BloodPressureRecord> bpHistory; // Stores curve (sys and dia)
    std::vector<std::string> anamneseCodes; // Stores ICD-Codes

    QUuid totalStayId; // shared ID grouping linked treatements together
    std::weak_ptr<Stay> predecessor;   // Prev clinic station link
    std::weak_ptr<Stay> successor;     // Prev clinic station link

    // =========================================================================
    // AUFGABE 3(a) ADDITION: Composition Pointer
    // =========================================================================
    std::unique_ptr<ClinicalInfo> specializedInfo;

    std::vector<ProcedureLog> procedureHistory; // Store successful bookings

    string difficulties;

public:
    Stay(long pID, long sID, string admDate, string disDate, string code, string text, Department* dept);
    ~Stay() = default;
    string showDetails();
    Department *getDepartment() const;
    string getAdmissionDate() const;
    string getDischargeDate() const;
    string getMainDiagnosisText() const;
    string getMainDiagnosisCode() const;
    std::string getNotes() const;
    void setNotes(const std::string &newNotes);
    void addBloodPressureRecord(const std::string& date, int sys, int dia);
    void addAnamneseCode(const std::string& code);
    void clearAnamneseCodes() { anamneseCodes.clear(); }
    std::vector<BloodPressureRecord> getBpHistory() const;
    std::vector<std::string> getAnamneseCodes() const;

    // Setters and Getters for the tracking linkages
    void setTotalStayId(const QUuid& id);
    QUuid getTotalStayId() const;

    std::shared_ptr<Stay> getPredecessor() const;
    void setPredecessor(std::shared_ptr<Stay> pred);

    std::shared_ptr<Stay> getSuccessor() const ;
    void setSuccessor(std::shared_ptr<Stay> succ);

    ClinicalInfo*getSpecializedInfo() const;
    void setSpecializedInfo(std::unique_ptr<ClinicalInfo> info);
    void setDischargeDate(const string &newDischargeDate);
    void addProcedure(const ProcedureLog& log);
    const std::vector<ProcedureLog>& getProcedures() const;
    void setDifficulties(const string &newDifficulties);
    string getDifficulties() const;
};

#endif // STAY_H
