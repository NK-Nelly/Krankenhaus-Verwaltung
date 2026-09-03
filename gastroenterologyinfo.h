#ifndef GASTROENTEROLOGYINFO_H
#define GASTROENTEROLOGYINFO_H

#include "clinicalinfo.h"

class GastroenterologyInfo : public ClinicalInfo {
private:
   string info;
public:
    GastroenterologyInfo();
    std::string getSpecializedInfo() const override;
};

#endif // GASTROENTEROLOGYINFO_H
