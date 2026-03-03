# Aircraft Cargo Management (C++)

## Description
A C++ console application that reads cargo data from a text file, validates each record, and loads cargo onto Boeing 737 and 767 aircraft while enforcing maximum payload limits.


---

## Features

- Object-oriented design using inheritance (`Cargo` base class with `Boeing737` and `Boeing767` subclasses)
- Reads and parses structured cargo records from a file
- Validates cargo type, aircraft type, and ULD abbreviation
- Enforces maximum aircraft payload limits  
  - Boeing 737: 46,000 lbs  
  - Boeing 767: 116,000 lbs
- Rejects cargo that exceeds aircraft weight capacity
- Calculates and displays total loaded weight per aircraft
- Uses C++ STL components such as `vector`, `string`, and file streams

---

## Input Format

Each line in the input file must follow one of the formats below:

Container <ABBR> <ULD_ID> <PLANE> <WEIGHT> <DEST>  
Pallet <ABBR> <ULD_ID> <PLANE> <WEIGHT> <DEST>  
Combo Flat <ABBR> <ULD_ID> <PLANE> <WEIGHT> <DEST>  

Example:

Container AYF ULD001 737 1200 LAX  
Pallet AKE ULD200 767 8000 JFK  

---

## Build

```bash
g++ -std=c++17 -O2 -Wall aircraftcargo_management.cpp -o cargo
```

---

## Run

```bash
./cargo
```

Enter the input filename when prompted.

---

## Notes

If loading a cargo unit exceeds the aircraft’s maximum weight, it will be automatically rejected and a message will be displayed.
