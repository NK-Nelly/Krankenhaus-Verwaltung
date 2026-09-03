#ifndef ONCOLOGYINFO_H
#define ONCOLOGYINFO_H

#include "clinicalinfo.h"

class OncologyInfo : public ClinicalInfo {
private:
    string info;
public:
    OncologyInfo();
    std::string getSpecializedInfo() const override;
};

#endif // ONCOLOGYINFO_H
