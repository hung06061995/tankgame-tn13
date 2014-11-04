#include "MY_Special1.h"
#include <iostream>
using namespace std;


MY_Special1::MY_Special1(IPlayerInfo *playerInfo, IGameInfo *gameInfo) : MY_IStage(playerInfo, gameInfo)
{
	endStage = false;
}


MY_Special1::~MY_Special1()
{
}

bool MY_Special1::isEndStage()
{
	return false;
}

Command MY_Special1::nextMove()
{
	list<ITank*> alives = _playerInfo->getAliveTanks();
	ITank* chosenOne = NULL;
	pair<int, int> pEnemy;

	for (list<ITank*>::iterator it = alives.begin(); it != alives.end(); ++it)
	{
		//the tank has no more ammo to shoot
		if (!(*it)->hasAmmo())
			continue;

		pair<int, int> pos = (*it)->getPosition();
		int range = (*it)->getRange();

		for (int i = -range; i <= range; ++i)
		for (int j = -range; j <= range; ++j)
		if (abs(i) + abs(j) <= range)
		{
			pair<int, int> posE(pos.first + i, pos.second + j);
		
			if (!_gameInfo->getMap()->isOnMap(posE))
				continue;
			if (isEnemyTank(posE))
			{
				pEnemy = posE;
				chosenOne = *it;
			}
		}
	}

	if (chosenOne == NULL)
		return Command();

	return Command(chosenOne, Command::FIRE, pEnemy.first, pEnemy.second);
}