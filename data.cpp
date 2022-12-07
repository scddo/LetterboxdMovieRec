#include <string>
#include <iostream>
#include <time.h>
using namespace std;

class Info {
public:
    string title, genre, year, pop;
    int rating;

};

struct Compare {
    bool operator()(Info const& p1, Info const& p2)
    {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        return p1.rating > p2.rating;
    }
};