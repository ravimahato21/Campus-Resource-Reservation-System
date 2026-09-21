#include "ResourceManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

using namespace std;

namespace {
string trim(const string& value) {
    const size_t first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    const size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

bool parseAvailability(const string& value) {
    const string cleaned = trim(value);
    return cleaned == "Available" || cleaned == "available" ||
           cleaned == "1" || cleaned == "true" || cleaned == "True";
}
}

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

Resource* ResourceManager::searchByID(const string& resourceID) {
    // Linear search: O(n)
    for (Resource& resource : resources) {
        if (resource.getResourceID() == resourceID) {
            return &resource;
        }
    }
    return nullptr;
}

const Resource* ResourceManager::searchByID(const string& resourceID) const {
    // Linear search: O(n)
    for (const Resource& resource : resources) {
        if (resource.getResourceID() == resourceID) {
            return &resource;
        }
    }
    return nullptr;
}

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

void ResourceManager::quickSortByName(int low, int high) {
    if (low < high) {
        const int pivot = partitionByName(low, high);
        quickSortByName(low, pivot - 1);
        quickSortByName(pivot + 1, high);
    }
}

int ResourceManager::partitionByPopularity(int low, int high) {
    const int pivot = resources[high].getRequestCount();
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        // Descending order for popularity.
        if (resources[j].getRequestCount() >= pivot) {
            ++i;
            swap(resources[i], resources[j]);
        }
    }

    swap(resources[i + 1], resources[high]);
    return i + 1;
}

void ResourceManager::quickSortByPopularity(int low, int high) {
    if (low < high) {
        const int pivot = partitionByPopularity(low, high);
        quickSortByPopularity(low, pivot - 1);
        quickSortByPopularity(pivot + 1, high);
    }
}

void ResourceManager::sortByName() {
    if (!resources.empty()) {
        quickSortByName(0, static_cast<int>(resources.size()) - 1);
    }
}

void ResourceManager::sortByPopularity() {
    if (!resources.empty()) {
        quickSortByPopularity(0, static_cast<int>(resources.size()) - 1);
    }
}

int ResourceManager::getAvailableCount() const {
    int count = 0;
    for (const Resource& resource : resources) {
        if (resource.isAvailable()) ++count;
    }
    return count;
}

int ResourceManager::getUnavailableCount() const {
    return static_cast<int>(resources.size()) - getAvailableCount();
}

int ResourceManager::getResourceCount() const {
    return static_cast<int>(resources.size());
}

const vector<Resource>& ResourceManager::getResources() const {
    return resources;
}
