#pragma once
#ifndef DATA_GEN
#define DATA_GEN

#define AREA_L 250
#define AREA_W 250
#define MIN_VX 50
#define MIN_VY 50
#define MAX_VX 50
#define MAX_VY 50
#include <set>
#include <array>
#include <unordered_map>
#include "TPR/TPRTree.h"
#include "TPR/CEntry.h"
using namespace std;


class DataGenerator
{
public:
	static void Generate(TPRTree* tree, vector<CEntry*> &dataset, int objNum, CEntry* my_ship);
};

#endif // !DATA_GEN