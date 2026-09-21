#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Resource.h"
#include <string>
#include <vector>

class ResourceManager {
private:
    std::vector<Resource> resources;

    int partitionByName(int low, int high);
    void quickSortByName(int low, int high);
    int partitionByPopularity(int low, int high);
    void quickSortByPopularity(int low, int high);

public:
    bool loadFromFile(const std::string& filename);
    void displayAll() const;

    Resource* searchByID(const std::string& resourceID);
    const Resource* searchByID(const std::string& resourceID) const;

    void sortByName();
    void sortByPopularity();

    int getAvailableCount() const;
    int getUnavailableCount() const;
    int getResourceCount() const;
    const std::vector<Resource>& getResources() const;
};

#endif
