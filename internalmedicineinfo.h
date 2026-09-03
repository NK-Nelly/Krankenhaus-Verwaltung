#ifndef INTERNALMEDICINEINFO_H
#define INTERNALMEDICINEINFO_H

#include "clinicalInfo.h"

class InternalMedicineInfo: public ClinicalInfo
{
private:
    string info;
public:
    InternalMedicineInfo();
    string getSpecializedInfo() const override;
};

#endif // INTERNALMEDICINEINFO_H
