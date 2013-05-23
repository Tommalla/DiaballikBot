#ifndef BOTCONTANTS_H
#define BOTCONTANTS_H
#include <vector>

using namespace std;

const int BUFFER_SIZE = 30;
const int INF = 999999;

//MCTSAI:
const int gamesQtyMultiplier = 2;
const int winPoints = 2;
const int drawPoints = 1;
const int losePoints = 0;

const int initialPlayoutsLimit = 2000;	//quantity of playouts for the whole tree at the beginning
const int initialExpansionBorder = 15;

const vector<pair<double, int> > expansionBorders({{0.0, 15}, {0.6, 14}});
const vector<pair<double, int> > playoutLimits({{0.0, 5000}, {0.7, 6000}, {0.85, 8000}});

#endif