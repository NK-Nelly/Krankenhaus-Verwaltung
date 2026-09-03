QT += widgets
QT += charts

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cardiology.cpp \
    cardiologyinfo.cpp \
    department.cpp \
    gastroenterology.cpp \
    gastroenterologyinfo.cpp \
    generalsurgery.cpp \
    generalsurgeryinfo.cpp \
    hospital.cpp \
    internalmedicine.cpp \
    internalmedicineinfo.cpp \
    main.cpp \
    hospitalui.cpp \
    oncology.cpp \
    oncologyinfo.cpp \
    orthopedics.cpp \
    orthopedicsinfo.cpp \
    patient.cpp \
    stay.cpp

HEADERS += \
    Gender.h \
    cardiology.h \
    cardiologyinfo.h \
    clinicalInfo.h \
    department.h \
    departmentType.h \
    gastroenterology.h \
    gastroenterologyinfo.h \
    generalsurgery.h \
    generalsurgeryinfo.h \
    hospital.h \
    hospitalui.h \
    internalmedicine.h \
    internalmedicineinfo.h \
    json.hpp \
    oncology.h \
    oncologyinfo.h \
    orthopedics.h \
    orthopedicsinfo.h \
    patient.h \
    resource.h \
    stay.h

FORMS += \
    hospitalui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
