#ifndef ORTHOPEDICSINFO_H
#define ORTHOPEDICSINFO_H

#include "clinicalInfo.h"

class OrthopedicsInfo :public ClinicalInfo
{
private:
    string info;
public:
    OrthopedicsInfo();
    string getSpecializedInfo() const override;

};

#endif // ORTHOPEDICSINFO_H
