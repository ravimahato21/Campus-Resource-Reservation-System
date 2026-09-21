#include "Reservation.h"
#include <iostream>

using namespace std;

Reservation::Reservation()
    : reservationID(0), studentID(0), studentName(""),
      resourceID(""), reservationDate("") {}

Reservation::Reservation(int reservationID,
                         int studentID,
                         const string& studentName,
                         const string& resourceID,
                         const string& reservationDate)
    : reservationID(reservationID), studentID(studentID),
      studentName(studentName), resourceID(resourceID),
      reservationDate(reservationDate) {}

int Reservation::getReservationID() const { return reservationID; }
int Reservation::getStudentID() const { return studentID; }
const string& Reservation::getStudentName() const { return studentName; }
const string& Reservation::getResourceID() const { return resourceID; }
const string& Reservation::getReservationDate() const { return reservationDate; }

void Reservation::display() const {
    cout << "Reservation ID: " << reservationID << '\n'
         << "Student ID: " << studentID << '\n'
         << "Student Name: " << studentName << '\n'
         << "Resource ID: " << resourceID << '\n'
         << "Reservation Date: " << reservationDate << '\n'
         << "-----------------------------------\n";
}
