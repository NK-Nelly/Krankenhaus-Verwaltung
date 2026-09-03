#ifndef CLINICALINFO_H
#define CLINICALINFO_H
#include <iostream>
#include <string>

using  std::string, std::cout,std::endl;

class ClinicalInfo {
public:
    virtual ~ClinicalInfo() = default;

    // Pure virtual method for polymorphic UI text extraction
    virtual string getSpecializedInfo() const = 0;
};

#endif // CLINICALINFO_H



/*
 * Why? (The Pros)

Flexibility (Aufgabe 3b / Transfers): In Aufgabe 3 (b), you are required to handle patient transfers (Verlegungen) between clinics.
If you use an Information Object, a patient transfer simply means swapping out or adding an info object component.
If you use inheritance, moving a patient from Cardiology to Oncology means you have to physically destroy a StayCardiology object
and allocate a brand-new StayOncology object, disrupting your predecessor/successor chain links!

*/