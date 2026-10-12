#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

// Managing Reservation operations and maintaining their associated data.
class Reservation {
private:
    int reservationID;
    int studentID;
    std::string studentName;
    std::string resourceID;
    std::string reservationDate;

public:
    // Handling reservation.
    Reservation();
    Reservation(int reservationID,
                int studentID,
                const std::string& studentName,
                const std::string& resourceID,
                const std::string& reservationDate);

    // Retrieving reservation id.
    int getReservationID() const;
    // Retrieving student id.
    int getStudentID() const;
    // Retrieving student name.
    const std::string& getStudentName() const;
    // Retrieving resource id.
    const std::string& getResourceID() const;
    // Retrieving reservation date.
    const std::string& getReservationDate() const;

    // Displaying the stored state.
    void display() const;
};

#endif
