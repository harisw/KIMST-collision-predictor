#pragma once
#ifndef DATA_GEN
#define DATA_GEN

#define AREA_L 1001
#define AREA_W 1001
#define MAX_VX 31
#define MAX_VY 31
#include <set>
#include <array>
#include <unordered_map>
#include "TPR/CEntry.h"
using namespace std;


class DataGenerator
{
	static void Generate(vector<CEntry*> dataset, int objNum);
};

#endif // !DATA_GEN