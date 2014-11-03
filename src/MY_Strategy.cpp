#include "MY_Strategy.h"


MY_Strategy::MY_Strategy(IPlayerInfo *player, IGameInfo *game) : _playerInfo(player), _gameInfo(game)
{
	this->currentStage = this->currentSteps = 0;
}

MY_Strategy::~MY_Strategy()
{
	for (int i = 0; i < stage.size(); ++i)
		delete stage[i];
}

void MY_Strategy::push(MY_IStage* st)
{
	stage.push_back(st);
	st->loadInfo(this->_playerInfo, this->_gameInfo);
}

Command MY_Strategy::nextMove()
{
	//increases the currentStep
	++currentSteps;

	//if the current stage ends, takes the next stage
	while (currentStage < stage.size() && stage[currentStage]->isEndStage())
		++currentStage;

	//if there is no more stage, skips the this turn
	if (currentStage >= stage.size())
		return Command();

	return stage[currentStage]->nextMove();
}

