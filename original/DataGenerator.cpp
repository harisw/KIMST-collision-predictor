#include "DataGenerator.h"

bool isExist(unordered_map<int, int> &m, int key)
{
	// Key is not present
	if (m.find(key) == m.end())
		return 0;

	return 1;
}

void DataGenerator::Generate(TPRTree* tree, vector<CEntry*> &dataset, int objNum, CEntry* my_ship)
{
	unordered_map<int, int> posDictionary;
	
	int x, y, z, vx, vy, vz;
	CEntry *new_entry;
	/*for (int j = 0; j < objNum; j++) {*/
	int i = 1;
	ofstream outfile("dataset.txt");
	outfile << "0 " << my_ship->m_x << " " << my_ship->m_y << " " << my_ship->m_vx << " " << my_ship->m_vy << endl;
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

		vx = rand() % MAX_VX - (rand() % MIN_VX);
		vy = rand() % MAX_VY - (rand() % MIN_VY);
		vz = 0;
		
		//new_entry = new CEntry(i, 0, x, y, z, vx, vy, vz);
		tree->Insert(CEntry(i, 0, x, y, z, vx, vy, vz));
		dataset.push_back(new_entry);
		cout << "Entry #" << i << " ( " << x << ", " << y << " )  speed : " << vx << " " << vy << endl;
		outfile << i << " " << x << " " << y << " " << vx << " " << vy << endl;
		i++;
	}

	return;
}
