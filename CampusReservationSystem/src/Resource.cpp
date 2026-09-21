#include "Resource.h"
#include <iostream>

using namespace std;

Resource::Resource()
    : resourceID(""), resourceName(""), resourceType(""),
      available(true), requestCount(0), reservationCount(0) {}

Resource::Resource(const string& id,
                   const string& name,
                   const string& type,
                   bool isAvailable)
    : resourceID(id), resourceName(name), resourceType(type),
      available(isAvailable), requestCount(0), reservationCount(0) {}

const string& Resource::getResourceID() const { return resourceID; }
const string& Resource::getResourceName() const { return resourceName; }
const string& Resource::getResourceType() const { return resourceType; }
bool Resource::isAvailable() const { return available; }
int Resource::getRequestCount() const { return requestCount; }
int Resource::getReservationCount() const { return reservationCount; }

void Resource::setAvailable(bool value) { available = value; }
void Resource::incrementRequestCount() { ++requestCount; }
void Resource::incrementReservationCount() { ++reservationCount; }

// Display all required resource fields plus usage statistics.
void Resource::display() const {
    cout << "Resource ID: " << resourceID << '\n'
         << "Resource Name: " << resourceName << '\n'
         << "Resource Type: " << resourceType << '\n'
         << "Availability Status: " << (available ? "Available" : "Unavailable") << '\n'
         << "Request Count: " << requestCount << '\n'
         << "Reservation Count: " << reservationCount << '\n'
         << "-----------------------------------\n";
}
