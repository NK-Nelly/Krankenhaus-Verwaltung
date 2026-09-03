#ifndef CARDIOLOGYINFO_H
#define CARDIOLOGYINFO_H

#include "clinicalInfo.h"

class CardiologyInfo : public ClinicalInfo
{
private:
    string info;
public:
    CardiologyInfo();
    string getSpecializedInfo() const override;
};

#endif // CARDIOLOGYINFO_H
