#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;
// for putting a point structure
struct Point {
    double x, y;   // data value
    int label;     // class (0 or 1)
};

// distance finding function
double getDistance(Point a, Point b) {
    return sqrt( (a.x - b.x)*(a.x - b.x)
               + (a.y - b.y)*(a.y - b.y) );
}
int main() {
    // Training data
    vector<Point> data = {
        {1, 2, 0},
        {2, 3, 0},
        {3, 3, 1},
        {6, 5, 1},
        {7, 7, 1}
    };

    // To classify it
    Point test = {3, 2, -1};

    int K = 3;  // nearest neighbour

    // put the value of distance and class
    vector<pair<double, int>> dist;

    // AA point distance ber kora lagbe
    for (auto p : data) {
        double d = getDistance(p, test);
        dist.push_back({d, p.label});
    }

    // distance according to sort
    sort(dist.begin(), dist.end());

    int count0 = 0, count1 = 0;

    // for nearest k 
    for (int i = 0; i < K; i++) {
        if (dist[i].second == 0)
            count0++;
        else
            count1++;
    }

    // max tai ans
    if (count0 > count1)
        cout << "Point in Class 0" << endl;
    else
        cout << "Point in Class 1" << endl;

    return 0;
}
