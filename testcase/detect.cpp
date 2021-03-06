#include <iostream>
#include "testcase.h"

using namespace std;
using namespace DMFB;

void TestCase::exampleDetect()
{
    cout << "Solving detect example..." << endl;

    Profile p;
    Droplet droplet1("droplet1");
    p.addDroplet(droplet1);
    vector<Droplet> dropletVec1;
    dropletVec1.push_back(droplet1);
    p.addDispenser(dropletVec1);
    p.addDetector(droplet1, 5);
    p.setSize(2, 3);
    p.setTime(6);

    Solver s(p);
    Timer t;
    s.setObjective("prove");
    
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_detect.txt");
    t.print("example_detect_time.txt");

    cout << endl;
}