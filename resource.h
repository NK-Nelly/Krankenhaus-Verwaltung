#ifndef RESOURCE_H
#define RESOURCE_H


#include <string>
#include <map>
#include <vector>
#include <QDate>

enum class ResourceType {
    OperatingRoom,
    MRI,
    CT
};

// Represents a confirmed booking appointment block
struct BookingSlot {
    std::string date;     // yyyyMMdd
    int startHour;        // 8 to 15
    int durationHours;
    std::string opsCode;
    long patientID;
};

class HospitalResource {
protected:
    std::string idName;
    ResourceType type;
    // Map of Date -> Vector of 24 booleans representing hours booked (index 8 to 15 are valid)
    std::map<std::string, std::vector<bool>> hourlySchedule;
    //  A permanent historical log of WHO booked WHAT on this specific machine/room
    std::vector<BookingSlot> confirmedBookings;

public:
    HospitalResource(std::string id, ResourceType t) : idName(id), type(t) {}
    virtual ~HospitalResource() = default;

    std::string getIdName() const { return idName; }
    ResourceType getType() const { return type; }
  const std::vector<BookingSlot>& getConfirmedBookings() const { return confirmedBookings; }
    // Tries to book a time slot. Returns the start hour if successful, -1 if unavailable.
    int tryBookSlot(const std::string& date, int durationHours, const std::string& opsCode, long pID) {
        // Initialize the day schedule map row if it does not exist yet (24 hours slots, default false)
        if (hourlySchedule.find(date) == hourlySchedule.end()) {
            hourlySchedule[date] = std::vector<bool>(24, false);
        }

        auto& dayTimeline = hourlySchedule[date];

        // Search for a contiguous segment inside the 8:00 to 16:00 block
        for (int hour = 8; hour <= 16 - durationHours; ++hour) {
            bool segmentAvailable = true;
            for (int offset = 0; offset < durationHours; ++offset) {
                if (dayTimeline[hour + offset]) {
                    segmentAvailable = false;
                    break;
                }
            }

            if (segmentAvailable) {
                // Book the slots!
                for (int offset = 0; offset < durationHours; ++offset) {
                    dayTimeline[hour + offset] = true;
                }

                // 2. ◄ UTILIZING THE PARAMETERS: Save a complete tracking slot receipt entry
                BookingSlot receipt;
                receipt.date = date;
                receipt.startHour = hour;
                receipt.durationHours = durationHours;
                receipt.opsCode = opsCode;   // Used here!
                receipt.patientID = pID;     // Used here!

                confirmedBookings.push_back(receipt);

                return hour; // Return the confirmed starting time
            }
        }
        return -1; // Completely booked out
    }
};

// Subclasses matching requirement categories
class OperatingRoom : public HospitalResource {
public:
    OperatingRoom(std::string name) : HospitalResource(name, ResourceType::OperatingRoom) {}
};

class MRIScanner : public HospitalResource {
public:
    MRIScanner(std::string name) : HospitalResource(name, ResourceType::MRI) {}
};

class CTScanner : public HospitalResource {
public:
    CTScanner(std::string name) : HospitalResource(name, ResourceType::CT) {}
};


#endif // RESOURCE_H
