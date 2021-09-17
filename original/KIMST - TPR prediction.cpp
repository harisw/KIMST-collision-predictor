#include "DataGenerator.h"
using namespace std;


int simu_time;
struct Line
{
public:
    CPosition* p0;
    CPosition* p1;
    Line() {}
    Line(CPosition* _p0, CPosition* _p1) {
        p0 = _p0;
        p1 = _p1;
    }
};

double getDistance(CPosition* p, CPosition* q)
{
    return sqrt(pow(p->x - q->x, 2) + pow(p->y - q->y, 2));
}

void readFiletoEntry(string filename, vector<CEntry*>& dataset)
{
    ifstream infile(filename);
    int id, x, y, vx, vy;
    while (infile >> id >> x >> y >> vx >> vy) {
        CEntry* newEntry = new CEntry(id, 0, x, y, 0, vx, vy, 0);
        dataset.push_back(newEntry);
    }
}

class Vessel

{
public:
    CPosition* loc0;
    CPosition* currentLoc;
    CPosition* locH;
    double vx;
    double vy;
    double d;
    vector<Line*> bt;
    Vessel(CPosition* _loc0, double _vx, double _vy, double _d) {
        loc0 = _loc0;
        currentLoc = loc0;
        vx = _vx;
        vy = _vy;
        d = _d;
    }

    void setFinalLoc()
    {
        double xH, yH;
        xH = loc0->x + (simu_time * vx);
        yH = loc0->y + (simu_time * vy);
        locH = new CPosition(xH, yH);
    }

    void setTrajectoryBuffer()
    {
        CPosition* p1 = new CPosition();
        CPosition* p2 = new CPosition();
        CPosition* p3 = new CPosition();
        CPosition* p4 = new CPosition();
        double x, y;

        p1->x = loc0->x - (d / 2);		//GOING UP CASE
        p1->y = loc0->y - d;

        p2->x = loc0->x + (d / 2);
        p2->y = loc0->y - d;

        p3->x = loc0->x - (d / 2);
        p3->y = loc0->y + d;

        p4->x = loc0->x + (d / 2);
        p4->y = loc0->y + d;

        bt.insert(bt.end(), { new Line(p1, p2), new Line(p1, p3),
            new Line(p2, p4), new Line(p3, p4) });
    }

    void updateCurrentLoc()
    {
        currentLoc->x += vx;
        currentLoc->y += vy;
    }
};

void naivePredict(Vessel* myVessel, vector<CEntry*>& dataset)
{
    int t = 0;
    double vx = myVessel->vx;
    double vy = myVessel->vy;
    CPosition* vesselLoc0 = myVessel->loc0;
    while (t < simu_time) {
        for (int j = 0; j < dataset.size(); j++) {
            if (getDistance(myVessel->currentLoc, dataset[j]->currentLoc) < myVessel->d)
                cout << "t" << j << " COLLision on obj#" << dataset[j]->getID() << endl;

            dataset[j]->updateCurrentLoc();
        }
        myVessel->updateCurrentLoc();
        t++;
    }
}

void TPRPredict(Vessel* _myVessel, vector<CEntry*>& dataset)
{

}

void TPRwithFilterPredict(Vessel* _myVessel, vector<CEntry*>& dataset)
{

}

int solution = 0;
int main()
{
    bool isMoving = true;
    
    //readDataFile(myTree);
    ////DataGenerator::Generate(myTree, datasets, objNum, my_ship);
    //cout << "Object count : " << myTree->getObjectCount() << endl;
    //cout << "Dataset count : " << datasets.size() << endl;

    switch (solution)
    {
    case 0:
        if (isMoving) {
            CPosition* initPos = new CPosition((double)AREA_L / 2, (double)AREA_W / 2);
            Vessel* myVessel = new Vessel(initPos, 15, 15, 20);
            vector<CEntry*> dataset;
            readFiletoEntry("dataset_best.txt", dataset);
            simu_time = 100;
            naivePredict(myVessel, dataset);
        }
        break;
    case 1:
        break;
    case 2:
        break;
    default:
        break;
    }

}