#pragma once
#include "MY_IStage.h"
class MY_Special1 :
	public MY_IStage
{
public:
	MY_Special1(IPlayerInfo *playerInfo = NULL, IGameInfo *gameInfo = NULL);
	~MY_Special1();

	bool isEndStage();
	Command nextMove();
protected:
	bool endStage;
};

