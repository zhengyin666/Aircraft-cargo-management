#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

const int MAXLOAD737 = 46000;
const int MAXLOAD767 = 116000;

class Cargo {
protected:
    string uldtype;
    string abbrev;
    string uldid;
    int aircraft;
    double weight;
    string destination;

public:
    // Default constructor
    Cargo();

    Cargo(const string& uldtype,
          const string& abbrev,
          const string& uldid,
          const int& aircraft,
          const double& weight,
          const string& destination);

    // Copy constructor
    Cargo(const Cargo& other)
        : uldtype(other.uldtype),
          abbrev(other.abbrev),
          uldid(other.uldid),
          aircraft(other.aircraft),
          weight(other.weight),
          destination(other.destination) {}

    // Destructor
    ~Cargo();

    // Mutators
    void setUldType(const string& type) { uldtype = type; }
    void setAbbrev(const string& abrv) { abbrev = abrv; }
    void setUldId(const string& uld) { uldid = uld; }
    void setAircraft(const int& ac) { aircraft = ac; }
    void setWeight(const double& wt, const string& wttype);
    void setDestination(const string& dest) { destination = dest; }

    // Getters
    string getUldType() const { return uldtype; }
    string getAbbrev() const { return abbrev; }
    string getUldId() const { return uldid; }
    int getAircraft() const { return aircraft; }
    double getWeight() const { return weight; }
    string getDestination() const { return destination; }

    virtual void maxweight(double&, double) = 0;
};

// 737 derived
class Boeing737 : public Cargo {
    using Cargo::Cargo;

private:
    int total737wt;

public:
    void maxweight(double& totwt737, double wt) override {
        if (totwt737 > MAXLOAD737) {
            totwt737 -= wt;  // remove if overweight
            throw runtime_error("too heavy for 737, removed\n\n");
        }
    }
};

// 767 derived
class Boeing767 : public Cargo {
    using Cargo::Cargo;

private:
    int total767wt;

public:
    void maxweight(double& totwt767, double wt) override {
        if (totwt767 > MAXLOAD767) {
            totwt767 -= wt;  // remove if overweight
            throw runtime_error("too heavy for 767, removed\n\n");
        }
    }
};

void load737(vector<Boeing737>& vectorptr737,
             const string type,
             const string abrv,
             const string id,
             const int craft,
             const double wt,
             const string dest,
             double& totwt737)  // added for use by maxweight, not const so it can be changed
{
    Boeing737 unitobj737(type, abrv, id, craft, wt, dest);  // constructor from Cargo class
    unitobj737.maxweight(totwt737, wt);                    // if too heavy, throw exception
    vectorptr737.push_back(unitobj737);                    // if ok, push onto vector
}

void load767(vector<Boeing767>& vectorptr767,
             const string type,
             const string abrv,
             const string id,
             const int craft,
             const double wt,
             const string dest,
             double& totwt767)
{
    Boeing767 unitobj767(type, abrv, id, craft, wt, dest);  // constructor from Cargo class
    unitobj767.maxweight(totwt767, wt);                    // if too heavy, throw exception
    vectorptr767.push_back(unitobj767);
}

void checkPlane(int);
void checkType(string);
void checkAbrv(string, string, int);
void vectorprint(vector<Boeing737> vectorptr737, vector<Boeing767> vectorptr767);

Cargo::Cargo() {
    uldtype = "none";
    abbrev = "no abbreviation";
    uldid = "no id";
    aircraft = 0;
    weight = 0;
    destination = "no destination";
}

Cargo::Cargo(const string& type,
             const string& abrv,
             const string& uld,
             const int& ac,
             const double& wt,
             const string& dest) {
    uldtype = type;
    abbrev = abrv;
    uldid = uld;
    aircraft = ac;
    weight = wt;
    destination = dest;
}

Cargo::~Cargo() {
    cout << "\nCargo destructor called\n";
}

void Cargo::setWeight(const double& wt, const string& wttype) {
    weight = wt;
}

void output(Cargo& cargo) {
    cout << fixed << showpoint << setprecision(2);
    cout << "***********************************************************\n";
    cout << setw(19) << "Unit load type: " << cargo.getUldType() << endl;
    cout << setw(19) << "Unit abbreviation: " << cargo.getAbbrev() << endl;
    cout << setw(19) << "Unit load ID: " << cargo.getUldId() << endl;
    cout << setw(19) << "Aircraft type: " << cargo.getAircraft() << endl;
    cout << setw(19) << "Unit Weight: " << cargo.getWeight() << " pounds" << endl;
    cout << setw(19) << "Destination code: " << cargo.getDestination() << endl;
    cout << "***********************************************************\n";
}

void input() {
    string cargostring;
    string type1;
    string type2;
    string abrv;
    string uld;
    int plane;
    double weight;
    string dest;
    string datafile;

    ifstream inputFile;

    double totwt737 = 0;
    double totwt767 = 0;

    vector<Boeing737> vectorptr737;
    vector<Boeing767> vectorptr767;

    // file open and verification
    do {
        try {
            inputFile.clear();
            cout << "\nWhat's the name of your data file?\n";
            getline(cin, datafile);
            cout << "datafile is " << datafile << endl;

            inputFile.open(datafile);
            if (!inputFile) {
                throw runtime_error(datafile + " Bad filename\n\n");
            }

            cout << datafile << " is open\n\n";
        } catch (runtime_error& excpt) {
            cout << datafile << " " << excpt.what();
        }
    } while (!inputFile);

    // file read/parse
    while (getline(inputFile, cargostring)) {  // read in one line
        try {
            istringstream cargoISS(cargostring);  // place line in the input stringstream
            cargoISS >> type1;

            if (type1.compare("Combo") == 0) {  // looking for Combo
                cargoISS >> type2 >> abrv >> uld >> plane >> weight >> dest;

                if (type2.compare("Flat") != 0) {
                    throw runtime_error("bad type for Combo Flat\n\n");
                }

                type1 = type1 + " " + type2;  // valid Combo Flat
            } else {
                cargoISS >> abrv >> uld >> plane >> weight >> dest;  // finish reading last of data
            }
        } catch (runtime_error& excpt) {
            cout << excpt.what();
        }

        // data validation
        try {
            checkType(type1);    // check for Container, Pallet, Combo Flat
            checkPlane(plane);   // 737 or 767

            if (plane == 737) {
                checkAbrv(type1, abrv, plane);
                totwt737 += weight;
                load737(vectorptr737, type1, abrv, uld, plane, weight, dest, totwt737);
            }

            if (plane == 767) {
                checkAbrv(type1, abrv, plane);
                totwt767 += weight;
                load767(vectorptr767, type1, abrv, uld, plane, weight, dest, totwt767);
            }

        } catch (runtime_error& excpt) {
            cout << excpt.what();
        }
    }

    inputFile.close();

    vectorprint(vectorptr737, vectorptr767);
    cout << "total weight for 737 load is " << totwt737 << endl;
    cout << "total weight for 767 load is " << totwt767 << endl;
}

void checkType(string type1) {
    if (type1 != "Container" && type1 != "Pallet" && type1 != "Combo Flat") {
        throw runtime_error(type1 + " Bad type, not Container, Pallet or Combo Flat\n");
    }
}

void checkPlane(int air) {
    if (air != 737 && air != 767) {
        throw runtime_error(to_string(air) + " Bad plane type\n");
    }
}

void checkAbrv(string type1, string abrv, int plane) {
    if (plane == 737) {
        if (abrv != "AYF" && abrv != "AYK" && abrv != "AAA" && abrv != "AYY" &&
            abrv != "PAG" && abrv != "PMC" && abrv != "PLA" && abrv != "CFE" &&
            abrv != "CFK" && abrv != "CFM") {
            throw runtime_error(type1 + " " + abrv + " Bad type for 737\n");
        }
    }

    if (plane == 767) {
        if (abrv != "AKE" && abrv != "APE" && abrv != "AKC" && abrv != "AQP" &&
            abrv != "AQF" && abrv != "AAP" && abrv != "P1P" && abrv != "P6P" &&
            abrv != "CFQ" && abrv != "CFM" && abrv != "CFP") {
            throw runtime_error(type1 + " " + abrv + "Bad type for 767\n");
        }
        cout << "stub for abrv check\n";
    }
}

void vectorprint(vector<Boeing737> vectorptr737, vector<Boeing767> vectorptr767) {
    for (int i = 0; i < (int)vectorptr737.size(); i++) {
        cout << "\n737 unit number " << i + 1 << endl;
        output(vectorptr737[i]);
    }

    for (int i = 0; i < (int)vectorptr767.size(); i++) {
        cout << "\n767 unit number " << i + 1 << endl;
        output(vectorptr767[i]);
    }
}

int main() {
    input();  // Call the input function from the main function with no arguments.
    return 0;
}