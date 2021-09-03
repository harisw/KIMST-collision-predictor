#pragma once
#include<vector>
#include"TPR/CEntry.h"
using namespace std;
class Vessel;

class Predictor
{
public:
	static int simu_time;
	static void naivePredict(Vessel* _myVessel, vector<CEntry*> &dataset);
	static void TPRPredict(Vessel* _myVessel, vector<CEntry*>& dataset);
	static void TPRwithFilterPredict(Vessel* _myVessel, vector<CEntry*>& dataset);
};