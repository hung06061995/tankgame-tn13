#pragma once
#include "MY_IStage.h"
#include "IPlayerInfo.h"
#include "IGameInfo.h"
#include "Command.h"
#include <vector>
#include <list>

using namespace std;

class MY_Strategy
{
protected:
	IPlayerInfo *_playerInfo;
	IGameInfo *_gameInfo;

	//currentSteps < gameInfo->getMaximumNumberOfTurn()
	int currentSteps;
	
	int currentStage;
	vector<MY_IStage*> stage;

public:
	//loads new Strategy
	template<class InputIterator>
	void reloadStage(InputIterator first, InputIterator last);
	
	//pushes new stage to the Strategy & update the playerInfo and gameInfo for that stage
	void push(MY_IStage* st);

	//gets the next move of this strategy
	Command nextMove();

	MY_Strategy(IPlayerInfo *player = NULL, IGameInfo *game = NULL);
	~MY_Strategy();
};



template<class InputIterator>
void MY_Strategy::reloadStage(InputIterator first, InputIterator last)
{
	stage = vector<MY_IStage*>(first, last);
	for (int i = 0; i < stage.size(); ++i)
		stage[i]->loadInfo(this->playerInfo, this->gameInfo);
}


