#include "Predictor.h"
#include "Vessel.h"

void Predictor::naivePredict(Vessel* myVessel, vector<CEntry*>& dataset)
{
	int t = 0;
	double vx = myVessel->vx;
	double vy = myVessel->vy;
	PointF* vesselLoc0 = myVessel->loc0;
	while (t < simu_time) {
		for (int j = 0; j < dataset.size(); j++) {
			if (getDistance(myVessel->currentLoc, dataset[j]->currentLoc) < _myVessel->d)
				cout << "t" << j << " COLLision on obj#" << dataset[j]->getID() << endl;

			dataset[j]->updateCurrentLoc();
		}
		myVessel->updateCurrentLoc();
		t++;
	}
}

void Predictor::TPRPredict(Vessel* _myVessel, vector<CEntry*>& dataset)
{

}

void Predictor::TPRwithFilterPredict(Vessel* _myVessel, vector<CEntry*>& dataset)
{

}