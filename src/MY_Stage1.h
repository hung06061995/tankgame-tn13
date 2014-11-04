#pragma once
//First stage

//nickname: Finding Nemo
//Description: Finds the way to the headquarter in which each tank uses the least ammunition. 
//If there are such many ways existing, finds the shortest way to the headquarter

#include "MY_IStage.h"

typedef vector<vector<int> > vectorInt2d;
typedef vector<vector<pair<int, int> > > vectorPair2d;
const int infinity = 999999999;

class MY_Stage1 :
	public MY_IStage
{
protected:
	bool endStage;
	//return the next position of each cell on the map
	void getInfoByDijkstra(const vectorInt2d& mapHP, vectorPair2d& nextPos, vectorInt2d& dis, vectorInt2d& amm);
	void getNextPosition(vectorPair2d& nextPos, vectorInt2d& dis, vectorInt2d& amm);
	
public:
	MY_Stage1();
	MY_Stage1(IPlayerInfo *playerInfo, IGameInfo *gameInfo);

	~MY_Stage1();

	bool isEndStage();
	Command nextMove();
};

