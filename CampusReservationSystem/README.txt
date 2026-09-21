Campus Resource Reservation System
==================================

Team Members:
- Ravi Mahato
- Nathan Kazadi
- Jeremy Brown

GitHub Repository:
https://github.com/ravimahato21/Campus-Resource-Reservation-System

OVERVIEW
This C++ project manages campus resources, active reservations, waiting lists,
cancellations, undo operations, searching, sorting, file input/output, and reports.

REQUIRED DATA STRUCTURES USED
- Vector: Resource inventory
- Singly Linked List: Active reservations
- Queue: Waiting list requests (FIFO for each resource)
- Stack: Cancellation history / undo

ALGORITHMS
- Linear Search: Resource ID and reservation ID lookup
- Quick Sort: Sort resources by name or request popularity

FILES
include/   Header files
src/       Implementation files and main.cpp
data/      Input/output text files

BUILD ON UNT CSE CELL
From the project root directory:

g++ -std=c++17 -Wall -Wextra -pedantic src/*.cpp -Iinclude -o campus_reservation

RUN
./campus_reservation

INPUT FILE FORMAT
resources.txt
resourceID|resourceName|resourceType|availabilityStatus

reservations.txt
reservationID|studentID|studentName|resourceID|reservationDate

FEATURES
1. Load resource data from file
2. Display and search resources
3. Create reservations
4. Place requests in a waiting queue when a resource is unavailable
5. Cancel active reservations
6. Automatically assign a freed resource to the first waiting request for that resource
7. Store cancellations on a stack
8. Undo the most recent cancellation when the resource is available
9. Search reservations by reservation ID or student ID
10. Quick-sort resources by name or popularity
11. Generate availability, active reservation, utilization, most-reserved,
    most-requested, and waiting-list reports
12. Validate menu input, file opening, duplicate reservation IDs, invalid resource IDs,
    empty waiting lists, and empty cancellation history
13. Preserve the instructor-provided data/reservations.txt input file and save the current session to data/reservations_output.txt on exit

NOTE ABOUT UNDO
If a cancelled resource is immediately assigned to a waiting student, the original
reservation cannot be restored until that resource becomes available again. This
prevents two active reservations from using the same resource simultaneously.


Instructor-provided input files
-------------------------------
The project uses the supplied data/resources.txt and data/reservations.txt files unchanged.
The resource file format is:
resourceID|resourceName|resourceType|availabilityStatus

The reservation file format is:
reservationID|studentID|studentName|resourceID|reservationDate

The supplied reservation dates use MM/DD/YYYY. Existing reservations are loaded as
reservation records exactly as supplied. The availability status shown for each resource
comes from resources.txt. New session state is written to data/reservations_output.txt so
the original instructor-provided reservations.txt file is not overwritten.
