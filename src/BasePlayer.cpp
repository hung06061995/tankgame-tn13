#include "BasePlayer.h"
#include <list>
#include <cstdlib>

using namespace std;

BasePlayer::BasePlayer() 
  : _playerInfo(NULL), _gameInfo(NULL) {}

BasePlayer::~BasePlayer() {}

void BasePlayer::onBindPlayerInfo(IPlayerInfo* playerInfo) {
  _playerInfo = playerInfo;
}
void BasePlayer::onBindGameInfo(IGameInfo* gameInfo) {
  _gameInfo = gameInfo;
}

IPlayerInfo* BasePlayer::getPlayerInfo() const {
  return _playerInfo;
}

bool BasePlayer::onStart() {
	_strategy = new MY_Strategy(this->_playerInfo, this->_gameInfo);
	_strategy->push(new MY_Stage1());
	return true;
}

bool BasePlayer::onFinish(){
  return true;
}

Command BasePlayer::nextMove() {
	return _strategy->nextMove();
}