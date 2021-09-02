#include <iostream>
#include <Windows.h>
#include <fstream>
#include "DataGenerator.h"
#include "Util.h"
using namespace std;

int safe_distance = 50;
CEntry* my_ship;
vector<CEntry*> datasets;

double getDistance(CEntry* a, CEntry* b)
{
    double xDiff = pow(b->m_x - a->m_x, 2);
    double yDiff = pow(b->m_y - a->m_y, 2);
    double zDiff = pow(b->m_z - a->m_z, 2);

    return sqrt(xDiff + yDiff + zDiff);
}

CEntry* predictPosition(CEntry* a, int t)
{
    double newX, newY, newZ;
    double diffTime = (double)t - a->m_time;
    newX = a->m_x + (diffTime * a->m_vx);
    newY = a->m_y + (diffTime * a->m_vy);
    newZ = a->m_z + (diffTime * a->m_vz);
    return new CEntry(a->m_id, t, newX, newY, newZ, 0, 0, 0);
}


void predictSimulation(TPRTree* tree, CEntry* ourShip, double exec_time)
{
    int t = 1;
    vector<CEntry> overlapMBR;
    CEntry* predictedObj = nullptr;
    CEntry* predictedShip = nullptr;
    
    while (t <= exec_time) {
        cout << "TIME : " << t << endl;
        if (t % PREDICTION_INTERVAL == 0) {
            predictedShip = predictPosition(ourShip, t);

            if (t == PREDICTION_INTERVAL || t % QUERY_INTERVAL == 0) {
                overlapMBR.clear();
                tree->rangeQueryKNN4(predictedShip->m_x, predictedShip->m_y, predictedShip->m_z, QUERY_RADIUS, overlapMBR, t);
            }

            if (overlapMBR.size() != 0) {
                for (int j = 0; j < overlapMBR.size(); j++) {
                    predictedObj = predictPosition(&overlapMBR[j], t);
                    if (getDistance(predictedObj, predictedShip) < safe_distance)
                        cout << "COLLISION with " << predictedObj->m_id << " ( " << predictedObj->m_x << ", " << predictedObj->m_y << " )" << endl;
                }


            }
        }
        t++;
        //Sleep(50);
    }
}

void readDataFile(TPRTree* treeinp)
{
    ifstream infile("dataset_best.txt");
    int id, x, y, vx, vy;
    CEntry* new_entry;
    while (infile >> id >> x >> y >> vx >> vy) {
        if (id == 0) {
            //continue;
            my_ship = new CEntry(0, 0, x, y, 0.0, vx, vy, 0.0);
        }
        else {
            new_entry = new CEntry(id, 0, x, y, 0, vx, vy, 0);
            treeinp->Insert(CEntry(id, 0, x, y, 0, vx, vy, 0));
            datasets.push_back(new_entry);
        }
    }
}
int main()
{
    int objNum;
    double simu_time;
    TPRTree* myTree = new TPRTree();
    double x, y, vx, vy;

    x = AREA_L/2; y = AREA_W/2;
    vx = 15;
    vy = 15;
    int mode = 0;
    //objNum = 50;
    simu_time = 50;
    //my_ship = new CEntry(0, 0, x, y, 0.0, vx, vy, 0.0);
    /*cout << "Enter ship info (x, y, vx, vy) : ";
    cin >> x >> y >> vx >> vy;*/
    //cout << "Enter number of obj : ";
    //cin >> objNum;
    //cout << "Enter execution time : ";
    //cin >> simu_time;


    
    readDataFile(myTree);
    //DataGenerator::Generate(myTree, datasets, objNum, my_ship);
    cout << "Object count : " << myTree->getObjectCount() << endl;
    cout << "Dataset count : " << datasets.size() << endl;
    switch (mode)
    {
    case STATIC_MODE:
        break;
    case MOVING_MODE:
        break;
    default:
        break;
    }

    predictSimulation(myTree, my_ship, simu_time);
}