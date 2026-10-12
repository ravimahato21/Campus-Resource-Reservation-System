#include "ResourceManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

using namespace std;

namespace {
// Trimming the stored state.
string trim(const string& value) {
    const size_t first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    const size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

// Parsing availability.
bool parseAvailability(const string& value) {
    const string cleaned = trim(value);
    return cleaned == "Available" || cleaned == "available" ||
           cleaned == "1" || cleaned == "true" || cleaned == "True";
}
}

// Loading resource details from the stored data file.
bool ResourceManager::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open resource file: " << filename << '\n';
        return false;
    }

    resources.clear();
    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        ++lineNumber;
        if (trim(line).empty() || line[0] == '#') continue;

        string id, name, type, status;
        stringstream ss(line);

        if (!getline(ss, id, '|') ||
            !getline(ss, name, '|') ||
            !getline(ss, type, '|') ||
            !getline(ss, status)) {
            cerr << "Warning: Skipping invalid resource line "
                 << lineNumber << ".\n";
            continue;
        }

        id = trim(id);
        name = trim(name);
        type = trim(type);
        status = trim(status);

        if (id.empty() || name.empty() || type.empty()) {
            cerr << "Warning: Skipping incomplete resource line "
                 << lineNumber << ".\n";
            continue;
        }

        if (searchByID(id) != nullptr) {
            cerr << "Warning: Duplicate resource ID " << id
                 << " skipped.\n";
            continue;
        }

        resources.emplace_back(id, name, type, parseAvailability(status));
    }

    return true;
}

// Displaying the currently loaded resource records.
void ResourceManager::displayAll() const {
    if (resources.empty()) {
        cout << "No resources loaded.\n";
        return;
    }

    cout << "\n===== CAMPUS RESOURCES =====\n";
    for (const Resource& resource : resources) {
        resource.display();
    }
}

// Finding a resource by its unique identifier.
Resource* ResourceManager::searchByID(const string& resourceID) {
    // Searching resource IDs linearly in O(n).
    for (Resource& resource : resources) {
        if (resource.getResourceID() == resourceID) {
            return &resource;
        }
    }
    return nullptr;
}

// Searching by id.
const Resource* ResourceManager::searchByID(const string& resourceID) const {
    // Searching resource IDs linearly in O(n).
    for (const Resource& resource : resources) {
        if (resource.getResourceID() == resourceID) {
            return &resource;
        }
    }
    return nullptr;
}

// Partitioning resources around a name-based quicksort pivot.
int ResourceManager::partitionByName(int low, int high) {
    const string pivot = resources[high].getResourceName();
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (resources[j].getResourceName() <= pivot) {
            ++i;
            swap(resources[i], resources[j]);
        }
    }

    swap(resources[i + 1], resources[high]);
    return i + 1;
}

// Sorting resource names recursively using quicksort.
void ResourceManager::quickSortByName(int low, int high) {
    if (low < high) {
        const int pivot = partitionByName(low, high);
        quickSortByName(low, pivot - 1);
        quickSortByName(pivot + 1, high);
    }
}

// Partitioning resources around a popularity-based quicksort pivot.
int ResourceManager::partitionByPopularity(int low, int high) {
    const int pivot = resources[high].getRequestCount();
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        // Sorting popularity in descending order.
        if (resources[j].getRequestCount() >= pivot) {
            ++i;
            swap(resources[i], resources[j]);
        }
    }

    swap(resources[i + 1], resources[high]);
    return i + 1;
}

// Sorting resources recursively by popularity.
void ResourceManager::quickSortByPopularity(int low, int high) {
    if (low < high) {
        const int pivot = partitionByPopularity(low, high);
        quickSortByPopularity(low, pivot - 1);
        quickSortByPopularity(pivot + 1, high);
    }
}

// Sorting resource records alphabetically by name.
void ResourceManager::sortByName() {
    if (!resources.empty()) {
        quickSortByName(0, static_cast<int>(resources.size()) - 1);
    }
}

// Sorting resource records by request popularity.
void ResourceManager::sortByPopularity() {
    if (!resources.empty()) {
        quickSortByPopularity(0, static_cast<int>(resources.size()) - 1);
    }
}

// Retrieving available count.
int ResourceManager::getAvailableCount() const {
    int count = 0;
    for (const Resource& resource : resources) {
        if (resource.isAvailable()) ++count;
    }
    return count;
}

// Retrieving unavailable count.
int ResourceManager::getUnavailableCount() const {
    return static_cast<int>(resources.size()) - getAvailableCount();
}

// Retrieving resource count.
int ResourceManager::getResourceCount() const {
    return static_cast<int>(resources.size());
}

// Retrieving resources.
const vector<Resource>& ResourceManager::getResources() const {
    return resources;
}

// Adding a resource after validating its fields and unique identifier.
bool ResourceManager::addResource(const string& id, const string& name,
                                  const string& type, bool available) {
    if (id.empty() || name.empty() || type.empty() ||
        id.find('|') != string::npos || name.find('|') != string::npos ||
// Handling find.
        type.find('|') != string::npos || searchByID(id) != nullptr) return false;
    resources.emplace_back(id, name, type, available);
    return true;
}

// Removing the resource matching the requested identifier.
bool ResourceManager::removeResource(const string& id) {
    for (auto it = resources.begin(); it != resources.end(); ++it) {
        if (it->getResourceID() == id) {
            resources.erase(it);
            return true;
        }
    }
    return false;
}

// Saving updated resource records for subsequent sessions.
bool ResourceManager::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file) return false;
    for (const Resource& resource : resources) {
        file << resource.getResourceID() << '|' << resource.getResourceName()
             << '|' << resource.getResourceType() << '|'
             << (resource.isAvailable() ? "Available" : "Unavailable") << '\n';
    }
    return static_cast<bool>(file);
}
