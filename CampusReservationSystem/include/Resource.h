#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

// Represents one reservable campus resource.
class Resource {
private:
    std::string resourceID;
    std::string resourceName;
    std::string resourceType;
    bool available;
    int requestCount;      // All requests, including requests placed on a waiting list.
    int reservationCount;  // Requests that actually became active reservations.

public:
    Resource();
    Resource(const std::string& id,
             const std::string& name,
             const std::string& type,
             bool isAvailable);

    const std::string& getResourceID() const;
    const std::string& getResourceName() const;
    const std::string& getResourceType() const;
    bool isAvailable() const;
    int getRequestCount() const;
    int getReservationCount() const;

    void setAvailable(bool value);
    void incrementRequestCount();
    void incrementReservationCount();
    void display() const;
};

#endif
