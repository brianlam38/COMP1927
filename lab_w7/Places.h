// Places.h ... interface to Places
// You should not change this
// (expect to add additional typedefs or #defines)

#ifndef PLACES_H
#define PLACES_H

// Types of places
typedef int PlaceType;

#define UNKNOWN                  0
#define LAND                     1
#define SEA                      2

// Types of connections between places
typedef int TransportID;

#define NONE                     0
#define ROAD                     1
#define RAIL                     2
#define BOAT                     3
#define ANY                      4

#define MIN_TRANSPORT            ROAD
#define MAX_TRANSPORT            BOAT

// Places
typedef int LocationID;

#define ADRIATIC_SEA             0
#define ALICANTE                 1
#define AMSTERDAM                2
#define ATHENS                   3
#define ATLANTIC_OCEAN           4
#define BARCELONA                5
#define BARI                     6
#define BAY_OF_BISCAY            7
#define BELGRADE                 8
#define BERLIN                   9
#define BLACK_SEA               10
#define BORDEAUX                11
#define BRUSSELS                12
#define BUCHAREST               13
#define BUDAPEST                14
#define CADIZ                   15
#define CAGLIARI                16
#define CASTLE_DRACULA          17
#define CLERMONT_FERRAND        18
#define COLOGNE                 19
#define CONSTANTA               20
#define DUBLIN                  21
#define EDINBURGH               22
#define ENGLISH_CHANNEL         23
#define FLORENCE                24
#define FRANKFURT               25
#define GALATZ                  26
#define GALWAY                  27
#define GENEVA                  28
#define GENOA                   29
#define GRANADA                 30
#define HAMBURG                 31
#define IONIAN_SEA              32
#define IRISH_SEA               33
#define KLAUSENBURG             34
#define LE_HAVRE                35
#define LEIPZIG                 36
#define LISBON                  37
#define LIVERPOOL               38
#define LONDON                  39
#define MADRID                  40
#define MANCHESTER              41
#define MARSEILLES              42
#define MEDITERRANEAN_SEA       43
#define MILAN                   44
#define MUNICH                  45
#define NANTES                  46
#define NAPLES                  47
#define NORTH_SEA               48
#define NUREMBURG               49
#define PARIS                   50
#define PLYMOUTH                51
#define PRAGUE                  52
#define ROME                    53
#define SALONICA                54
#define SANTANDER               55
#define SARAGOSSA               56
#define SARAJEVO                57
#define SOFIA                   58
#define ST_JOSEPH_AND_ST_MARYS  59
#define STRASBOURG              60
#define SWANSEA                 61
#define SZEGED                  62
#define TOULOUSE                63
#define TYRRHENIAN_SEA          64
#define VALONA                  65
#define VARNA                   66
#define VENICE                  67
#define VIENNA                  68
#define ZAGREB                  69
#define ZURICH                  70

#define MIN_MAP_LOCATION         0
#define MAX_MAP_LOCATION        70
#define NUM_MAP_LOCATIONS       71

// Other "locations"
#define CITY_UNKNOWN            100
#define SEA_UNKNOWN             101
#define HIDE                    102
#define DOUBLE_BACK_1           103
#define DOUBLE_BACK_2           104
#define DOUBLE_BACK_3           105
#define DOUBLE_BACK_4           106
#define DOUBLE_BACK_5           107
#define TELEPORT                108
#define UNKNOWN_LOCATION        -1
#define NOWHERE                 UNKNOWN_LOCATION


// given a Place number, return its name
char *idToName(int place);

// given a Place number, return its type
int idToType(int place);

// given a Place name, return its ID number
int nameToID(char *name);

// given a Place abbreviation, return its ID number
int abbrevToID(char *abbrev);

#define isLand(place)  (idToType(place) == LAND)
#define isSea(place)  (idToType(place) == SEA)

#define validPlace(pid) (pid >= MIN_MAP_LOCATION && pid <= MAX_MAP_LOCATION)

#endif
