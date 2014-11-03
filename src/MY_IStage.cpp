#include "MY_IStage.h"

MY_IStage::MY_IStage(IPlayerInfo *playerInfo, IGameInfo *gameInfo) : _playerInfo(playerInfo), _gameInfo(gameInfo) {}

//
void MY_IStage::loadInfo(IPlayerInfo *playerInfo, IGameInfo *gameInfo)
{
	_playerInfo = playerInfo;
	_gameInfo = gameInfo;
}

bool MY_IStage::isMyTank(pair<int, int> pos)
{
	char id = (*_gameInfo->getMap())(pos.first, pos.second);
	if (_gameInfo->getMap()->isTank(id) && _gameInfo->getPlayerByID(id) == _playerInfo)
		return true;

	return false;
}

bool MY_IStage::isAvailable(pair<int, int> pos)
{
	char id = (*this->_gameInfo->getMap())(pos.first, pos.second);
	if (this->_gameInfo->getMap()->isEmptySpace(id) || this->_gameInfo->getMap()->isBridge(id))
		return true;

	return false;
}

bool MY_IStage::isObstacle(pair<int, int> pos)
{
	char id = (*this->_gameInfo->getMap())(pos.first, pos.second);

	bool condition1 = this->_gameInfo->getMap()->isBlock(id);
	bool condition2 = (this->_gameInfo->getMap()->isTank(id)) && (this->_gameInfo->getPlayerByID(id) != this->_playerInfo);
	
	return condition1 || condition2;

}