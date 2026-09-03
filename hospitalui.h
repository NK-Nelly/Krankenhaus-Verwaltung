#ifndef HOSPITALUI_H
#define HOSPITALUI_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include <QCompleter>

class Hospital;
class Stay;

QT_BEGIN_NAMESPACE
namespace Ui {
class HospitalUI;
}
QT_END_NAMESPACE

class HospitalUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit HospitalUI(Hospital* hospitalPtr,QWidget *parent = nullptr);
    ~HospitalUI() override;
    void updatePatientTree(); // Helper method to sync backend data to the screen


private slots:
    void on_actionLese_ICD_Texte_triggered();

    void on_actionLese_Aufenthalte_triggered();

    void on_actionPatienten_anzeigen_triggered();

    void on_patientTreeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_patientTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_aktuelleDiagnoseInput_textChanged(const QString &arg1);

    void on_anamneseTableWidget_cellChanged(int row, int column);

    void on_bemerkungenTextEdit_textChanged();

    void on_addBPButton_clicked();

    void on_transferPatientButton_clicked();

    void on_dischargePatientButton_clicked();

    void on_scheduleProcedureButton_clicked();
    void on_difficultiesTextEdit_textChanged();

private:
    Ui::HospitalUI *ui;
    Hospital* hospital;
    std::string convertInsuranceLetter(const std::string& insNum);
    int calculateMod10(const std::string& digits);
    bool checkInsuranceNumber(const std::string& insNum, bool& digit10_Ok,
                              bool& digit20_Ok, int& correct10, int& correct20);
    void updateChartDisplay(Stay* clinicalCase); // Helper to paint the line chart

    QCompleter* icdCompleter;
    QCompleter* opsCompleter;
};
#endif // HOSPITALUI_H
