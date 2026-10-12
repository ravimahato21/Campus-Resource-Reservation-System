#include "ReservationList.h"
#include <iostream>

using namespace std;

ReservationList::ReservationList() : head(nullptr), size(0) {}

ReservationList::~ReservationList() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

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

bool ReservationList::containsID(int reservationID) const {
    return searchByID(reservationID) != nullptr;
}

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
