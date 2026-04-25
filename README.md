# Town Management System Using AVL Tree and Hash Table

A C-based data structures project for managing Palestinian districts and towns using an AVL Tree and a Hash Table.

The system supports inserting, updating, deleting, searching, filtering, loading data from files, and saving updated data back to files.

---

## Features

### AVL Tree
- Insert a new town
- Update town information
- Delete a town
- Search for a town
- List towns in alphabetical order
- Filter towns by population
- Filter towns by municipality status
- Save data to file

### Hash Table
- Insert a town record
- Search for a town
- Delete a record
- Print the hash table
- Display table size and load factor
- Handle collisions using linear probing
- Rehash automatically when the load factor increases
- Save data to file

---

## System Structure

```mermaid
flowchart TD
    A[Program Start] --> B[Load Data From Files]
    B --> C{Choose Data Structure}

    C --> D[AVL Tree Module]
    C --> E[Hash Table Module]

    D --> D1[Insert Town]
    D --> D2[Update Town]
    D --> D3[Delete Town]
    D --> D4[Search Town]
    D --> D5[List Alphabetically]
    D --> D6[Filter by Population]
    D --> D7[Filter by Municipality]
    D --> D8[Save to File]

    E --> E1[Insert Record]
    E --> E2[Search Record]
    E --> E3[Delete Record]
    E --> E4[Print Hash Table]
    E --> E5[Show Load Factor]
    E --> E6[Rehash]
    E --> E7[Save to File]
