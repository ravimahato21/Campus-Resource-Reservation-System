#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

// Representing one reservable campus resource.
// Managing Resource operations and maintaining their associated data.
class Resource {
private:
    std::string resourceID;
    std::string resourceName;
    std::string resourceType;
    bool available;
    int requestCount;      // Counting all requests, including waiting-list entries.
    int reservationCount;  // Counting requests promoted into active reservations.

public:
    // Handling resource.
    Resource();
    Resource(const std::string& id,
             const std::string& name,
             const std::string& type,
             bool isAvailable);

    // Retrieving resource id.
    const std::string& getResourceID() const;
    // Retrieving resource name.
    const std::string& getResourceName() const;
    // Retrieving resource type.
    const std::string& getResourceType() const;
    // Checking available.
    bool isAvailable() const;
    // Retrieving request count.
    int getRequestCount() const;
    // Retrieving reservation count.
    int getReservationCount() const;

    // Updating available.
    void setAvailable(bool value);
    // Increasing request count.
    void incrementRequestCount();
    // Increasing reservation count.
    void incrementReservationCount();
    // Displaying the stored state.
    void display() const;
};

#endif
