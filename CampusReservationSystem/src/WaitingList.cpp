#include "WaitingList.h"
#include <iostream>

using namespace std;

WaitingList::WaitingList() : front(nullptr), rear(nullptr), size(0) {}

// Handling waiting list.
WaitingList::~WaitingList() {
    while (front != nullptr) {
        Node* temp = front;
        front = front->next;
        delete temp;
    }
    rear = nullptr;
}

// Adding waiting requests to the end of the FIFO queue.
void WaitingList::enqueue(const Reservation& request) {
    Node* newNode = new Node(request);

    if (rear == nullptr) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }

    ++size;
}

// Removing the earliest queued request matching the available resource.
bool WaitingList::dequeueForResource(const string& resourceID,
                                     Reservation& nextRequest) {
    Node* current = front;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->request.getResourceID() == resourceID) {
            nextRequest = current->request;

            if (previous == nullptr) {
                front = current->next;
            } else {
                previous->next = current->next;
            }

            if (current == rear) {
                rear = previous;
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

// Checking reservation id.
bool WaitingList::containsReservationID(int reservationID) const {
    Node* current = front;
    while (current != nullptr) {
        if (current->request.getReservationID() == reservationID) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Displaying each entry stored in the current collection.
void WaitingList::displayAll() const {
    if (front == nullptr) {
        cout << "Waiting list is empty.\n";
        return;
    }

    cout << "\n===== WAITING LIST =====\n";
    Node* current = front;
    int position = 1;

    while (current != nullptr) {
        cout << "Queue Position: " << position++ << '\n';
        current->request.display();
        current = current->next;
    }
}

int WaitingList::getCount() const { return size; }

// Counting for resource.
int WaitingList::getCountForResource(const string& resourceID) const {
    int count = 0;
    Node* current = front;

    while (current != nullptr) {
        if (current->request.getResourceID() == resourceID) {
            ++count;
        }
        current = current->next;
    }

    return count;
}

bool WaitingList::isEmpty() const { return front == nullptr; }
