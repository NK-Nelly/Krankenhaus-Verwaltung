#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>

class Department
{
    std::string name;
public:
    Department(std::string deptName);
    virtual ~Department() = default;
    std::string getName() const;
};

#endif // DEPARTMENT_H
