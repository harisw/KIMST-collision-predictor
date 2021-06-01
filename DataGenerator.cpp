#include "DataGenerator.h"

bool isExist(unordered_map<int, int> &m, int key)
{
	// Key is not present
	if (m.find(key) == m.end())
		return 0;

	return 1;
}

void DataGenerator::Generate(vector<CEntry*> dataset, int objNum)
{
	
	unordered_map<int, int> posDictionary;
	
	int x, y, z, vx, vy, vz;
	CEntry *new_entry;
	/*for (int j = 0; j < objNum; j++) {*/
	int i = 0;
	while(dataset.size() < objNum){
		while (1) {
			x = rand() % AREA_L;
			y = rand() % AREA_W;
			if (!isExist(posDictionary, x) || posDictionary[x] != y) {
				posDictionary[x] = y;
				break;
			}
		}
		z = 0;

		vx = rand() % MAX_VX + (-MAX_VX);
		vy = rand() % MAX_VY + (-MAX_VY);
		vz = 0;
		
		new_entry = new CEntry(i, 0, x, y, z, vx, vy, vz);
		dataset.push_back(new_entry);
		i++;
	}

	return;
}
