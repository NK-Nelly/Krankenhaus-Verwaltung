#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>
#include <QString>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include "departmentType.h"
#include "resource.h"

class Patient;
class Department;

using std::string;

class Hospital
{
private:
    string name;
    std::vector<std::unique_ptr<Patient>>patients;
    std::unordered_map<string,string>icdMap;
    std::unordered_map<string, string>opsMap;
    std::map<DepartmentType,std::unique_ptr<Department>> departments;
std::vector<std::unique_ptr<HospitalResource>> centralResources;

public:
    Hospital(string n);
    ~Hospital() ;
    void readCaseFile(string fileName);
    void userDialog();
    void readICDData(const QString& fileName);
    void printPatientList();
    bool hasICDData()const;
    Patient* findPatientByInsurance(const string& insNur);
    std::vector<Patient*> getPatients() const;
    std::string getICDText(const std::string& code) const;
    const std::map<DepartmentType, std::unique_ptr<Department>>& getDepartments() const;  // use const reference bcs without it , you will be trying
                                                                                          //    to use a copy function that has been explicitly deleted!"
    std::string getOPSText(const std::string& code) const;
    bool readOPSCodes(const QString& filename);
    const std::vector<std::unique_ptr<HospitalResource>>& getCentralResources() const;
    // Core Router: Searches for the first free asset matching a given ResourceType category
    HospitalResource* allocateFirstAvailableResource(ResourceType targetType,
                                                     const std::string& date, int duration, const std::string& ops, long pID);
bool isValidOPSCode(const std::string& code) const;

    std::unordered_map<string, string> getIcdMap() const;
std::unordered_map<string, string> getOpsMap() const;
};

#endif // HOSPITAL_H
