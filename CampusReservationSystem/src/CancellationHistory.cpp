#include "CancellationHistory.h"
#include <iostream>

using namespace std;

CancellationHistory::CancellationHistory() : top(nullptr), size(0) {}

CancellationHistory::~CancellationHistory() {
    while (top != nullptr) {
        Node* temp = top;
        top = top->next;
        delete temp;
    }
}

void CancellationHistory::push(const Reservation& reservation) {
    Node* newNode = new Node(reservation);
    newNode->next = top;
    top = newNode;
    ++size;
}

bool CancellationHistory::peek(Reservation& reservation) const {
    if (top == nullptr) {
        return false;
    }
    reservation = top->data;
    return true;
}

bool CancellationHistory::pop(Reservation& reservation) {
    if (top == nullptr) {
        return false;
    }

    Node* temp = top;
    reservation = temp->data;
    top = top->next;
    delete temp;
    --size;
    return true;
}

bool CancellationHistory::containsReservationID(int reservationID) const {
    Node* current = top;
    while (current != nullptr) {
        if (current->data.getReservationID() == reservationID) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void CancellationHistory::displayAll() const {
    if (top == nullptr) {
        cout << "Cancellation history is empty.\n";
        return;
    }

    cout << "\n===== CANCELLATION HISTORY =====\n";
    Node* current = top;
    while (current != nullptr) {
        current->data.display();
        current = current->next;
    }
}

int CancellationHistory::getSize() const { return size; }
bool CancellationHistory::isEmpty() const { return top == nullptr; }
