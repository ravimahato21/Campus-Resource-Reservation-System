#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Resource.h"
#include <string>
#include <vector>

// Managing ResourceManager operations and maintaining their associated data.
class ResourceManager {
private:
    std::vector<Resource> resources;

    // Partitioning resources around a name-based quicksort pivot.
    int partitionByName(int low, int high);
    // Sorting resource names recursively using quicksort.
    void quickSortByName(int low, int high);
    // Partitioning resources around a popularity-based quicksort pivot.
    int partitionByPopularity(int low, int high);
    // Sorting resources recursively by popularity.
    void quickSortByPopularity(int low, int high);

public:
    // Loading resource records and interpreting stored availability values.
    bool loadFromFile(const std::string& filename);
    // Displaying each entry stored in the current collection.
    void displayAll() const;
    bool addResource(const std::string& id, const std::string& name,
                     const std::string& type, bool available);
    // Finding and removing the matching resource record.
    bool removeResource(const std::string& id);
    // Writing the current resource records for loading in a later session.
    bool saveToFile(const std::string& filename) const;

    // Searching by id.
    Resource* searchByID(const std::string& resourceID);
    // Searching by id.
    const Resource* searchByID(const std::string& resourceID) const;

    // Sorting by name.
    void sortByName();
    // Sorting by popularity.
    void sortByPopularity();

    // Retrieving available count.
    int getAvailableCount() const;
    // Retrieving unavailable count.
    int getUnavailableCount() const;
    // Retrieving resource count.
    int getResourceCount() const;
    // Retrieving resources.
    const std::vector<Resource>& getResources() const;
};

#endif
