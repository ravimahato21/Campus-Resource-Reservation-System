#ifndef RESERVATION_LIST_H
#define RESERVATION_LIST_H

#include "Reservation.h"

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
    ReservationList();
    ~ReservationList();

    ReservationList(const ReservationList&) = delete;
    ReservationList& operator=(const ReservationList&) = delete;

    void add(const Reservation& reservation);
    bool removeByID(int reservationID, Reservation& removed);
    Reservation* searchByID(int reservationID);
    const Reservation* searchByID(int reservationID) const;
    bool containsID(int reservationID) const;
    void displayAll() const;
    void displayByStudentID(int studentID) const;
    int getSize() const;
    bool isEmpty() const;

    template <typename Func>
    void forEach(Func func) const {
        Node* current = head;
        while (current != nullptr) {
            func(current->data);
            current = current->next;
        }
    }
};

#endif
