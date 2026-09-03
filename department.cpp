#include "department.h"

Department::Department(std::string deptName): name(deptName) {}

std::string Department::getName() const {  return name; }
