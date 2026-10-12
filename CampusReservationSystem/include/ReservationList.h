#ifndef RESERVATION_LIST_H
#define RESERVATION_LIST_H

#include "Reservation.h"

// Managing ReservationList operations and maintaining their associated data.
class ReservationList {
private:
    struct Node {
        Reservation data;
        Node* next;
        explicit Node(const Reservation& reservation)
            : data(reservation), next(nullptr) {}
    };

    Node* head;
    int size;

public:
    // Handling reservation list.
    ReservationList();
    // Handling reservation list.
    ~ReservationList();

    // Handling reservation list.
    ReservationList(const ReservationList&) = delete;
    ReservationList& operator=(const ReservationList&) = delete;

    // Appending a reservation to the linked active-reservation list.
    void add(const Reservation& reservation);
    // Finding and unlinking a reservation with the requested identifier.
    bool removeByID(int reservationID, Reservation& removed);
    // Searching by id.
    Reservation* searchByID(int reservationID);
    // Searching by id.
    const Reservation* searchByID(int reservationID) const;
    // Checking id.
    bool containsID(int reservationID) const;
    // Displaying each entry stored in the current collection.
    void displayAll() const;
    // Displaying by student id.
    void displayByStudentID(int studentID) const;
    // Counting the stored state.
    int getSize() const;
    // Checking empty.
    bool isEmpty() const;

    template <typename Func>
    void forEach(Func func) const {
        Node* current = head;
        while (current != nullptr) {
            // Handling func.
            func(current->data);
            current = current->next;
        }
    }
};

#endif
