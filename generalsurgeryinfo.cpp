#include "generalsurgeryinfo.h"

GeneralSurgeryinfo::GeneralSurgeryinfo() {}

string  GeneralSurgeryinfo::getSpecializedInfo() const
{
   return "Chirurgie Daten: " + operationTheaterAllocated ;
}
