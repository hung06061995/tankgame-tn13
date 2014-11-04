#include "NHOM12Player.h"
#include <list>
#include <cstdlib>

using namespace std;

NHOM12Player::NHOM12Player() 
  : _playerInfo(NULL), _gameInfo(NULL) {}

NHOM12Player::~NHOM12Player() {}

void NHOM12Player::onBindPlayerInfo(IPlayerInfo* playerInfo) {
  _playerInfo = playerInfo;
}
void NHOM12Player::onBindGameInfo(IGameInfo* gameInfo) {
  _gameInfo = gameInfo;
}

IPlayerInfo* NHOM12Player::getPlayerInfo() const {
  return _playerInfo;
}

bool NHOM12Player::onStart() {
	_strategy = new MY_Strategy(this->_playerInfo, this->_gameInfo);
	_strategy->push(new MY_Stage1());
	_strategy->loadSpecial(new MY_Special1());
	return true;
}

bool NHOM12Player::onFinish(){
  return true;
}

Command NHOM12Player::nextMove() {
	return _strategy->nextMove();
}