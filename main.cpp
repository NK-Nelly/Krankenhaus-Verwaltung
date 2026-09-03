#include "hospitalui.h"
#include "hospital.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Hospital myHospital("Klinikum Darmstadt");
    HospitalUI w(& myHospital);

    w.show();
    return QCoreApplication::exec();
}
