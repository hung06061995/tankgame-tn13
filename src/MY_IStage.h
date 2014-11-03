#pragma once

#include <vector>
#include <list>
#include <algorithm>
#include <utility>
#include <functional>
#include <queue>

#include "IPlayerInfo.h"
#include "IGameInfo.h"
#include "Command.h"

using namespace std;

//Stage's Interface
class MY_IStage
{
protected:
	IPlayerInfo* _playerInfo;
	IGameInfo* _gameInfo;

	//returns true if this position is my tank
	bool isMyTank(pair<int, int> pos);
	
	//returns true if this postion is an empty space or a bridge
	bool isAvailable(pair<int, int> pos);
	
	//return true if this position is a block or an enemy's tank
	bool isObstacle(pair<int, int> pos);

public:
	MY_IStage() {}
	MY_IStage(IPlayerInfo *playerInfo, IGameInfo *gameInfo);
	virtual ~MY_IStage() {}

	//loads playerInfo and gameInfo
	//only uses this method in the beginning of the stage
	void loadInfo(IPlayerInfo *playerInfo, IGameInfo *gameInfo);
	
	//check if the stage ends
	virtual bool isEndStage() = 0;

	//get the next move of this stage
	virtual Command nextMove() = 0;
};



