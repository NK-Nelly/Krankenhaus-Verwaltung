#ifndef GENERALSURGERYINFO_H
#define GENERALSURGERYINFO_H

#include "clinicalinfo.h"
#include <string>

class GeneralSurgeryinfo : public ClinicalInfo
{
private:
    string operationTheaterAllocated;
public:
    GeneralSurgeryinfo();
    string getSpecializedInfo() const override;
};

#endif // GENERALSURGERYINFO_H
