#include "ReservationList.h"
#include <iostream>

using namespace std;

ReservationList::ReservationList() : head(nullptr), size(0) {}

// Handling reservation list.
ReservationList::~ReservationList() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// Adding a reservation to the active linked list.
void ReservationList::add(const Reservation& reservation) {
    Node* newNode = new Node(reservation);

    if (head == nullptr) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
    ++size;
}

// Removing a matching reservation from the active list.
bool ReservationList::removeByID(int reservationID, Reservation& removed) {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->data.getReservationID() == reservationID) {
            removed = current->data;

            if (previous == nullptr) {
                head = current->next;
            } else {
                previous->next = current->next;
            }

            delete current;
            --size;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

// Searching by id.
Reservation* ReservationList::searchByID(int reservationID) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.getReservationID() == reservationID) {
            return &current->data;
        }
        current = current->next;
    }
    return nullptr;
}

// Searching by id.
const Reservation* ReservationList::searchByID(int reservationID) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.getReservationID() == reservationID) {
            return &current->data;
        }
        current = current->next;
    }
    return nullptr;
}

// Checking id.
bool ReservationList::containsID(int reservationID) const {
    return searchByID(reservationID) != nullptr;
}

// Displaying each entry stored in the current collection.
void ReservationList::displayAll() const {
    if (head == nullptr) {
        cout << "No active reservations.\n";
        return;
    }

    cout << "\n===== ACTIVE RESERVATIONS =====\n";
    Node* current = head;
    while (current != nullptr) {
        current->data.display();
        current = current->next;
    }
}

// Displaying by student id.
void ReservationList::displayByStudentID(int studentID) const {
    bool found = false;
    Node* current = head;

    while (current != nullptr) {
        if (current->data.getStudentID() == studentID) {
            current->data.display();
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        cout << "No active reservations found for student ID "
             << studentID << ".\n";
    }
}

int ReservationList::getSize() const { return size; }
bool ReservationList::isEmpty() const { return head == nullptr; }
