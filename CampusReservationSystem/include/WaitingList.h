#ifndef WAITING_LIST_H
#define WAITING_LIST_H

#include "Reservation.h"
#include <string>

// Managing WaitingList operations and maintaining their associated data.
class WaitingList {
private:
    struct Node {
        Reservation request;
        Node* next;
        explicit Node(const Reservation& reservation)
            : request(reservation), next(nullptr) {}
    };

    Node* front;
    Node* rear;
    int size;

public:
    // Handling waiting list.
    WaitingList();
    // Handling waiting list.
    ~WaitingList();

    // Handling waiting list.
    WaitingList(const WaitingList&) = delete;
    WaitingList& operator=(const WaitingList&) = delete;

    // Appending a request to the rear of the FIFO waiting queue.
    void enqueue(const Reservation& request);
    // Finding and removing the oldest waiting request matching a resource.
    bool dequeueForResource(const std::string& resourceID, Reservation& nextRequest);
    // Checking reservation id.
    bool containsReservationID(int reservationID) const;
    // Displaying each entry stored in the current collection.
    void displayAll() const;
    // Counting the stored state.
    int getCount() const;
    // Counting for resource.
    int getCountForResource(const std::string& resourceID) const;
    // Checking empty.
    bool isEmpty() const;

    template <typename Func>
    void forEach(Func func) const {
        Node* current = front;
        while (current != nullptr) {
            // Handling func.
            func(current->request);
            current = current->next;
        }
    }
};

#endif
