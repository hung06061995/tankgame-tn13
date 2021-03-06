#include "BasePlayerInfo.h"

#include <algorithm>

using namespace std;

#pragma region IPlayerInfoImplementation
char BasePlayerInfo::getPlayerMapID() const {
  return _mapID;
}

list<ITank*> BasePlayerInfo::getAliveTanks() const {
  return _aliveTanks;
}

list<ITank*> BasePlayerInfo::getDeadTanks() const {
  return _deadTanks;
}

pair<int, int> BasePlayerInfo::getHeadquarterPosition() const {
  return _headquarterPosition;
}

// Command getLastMove() const {
//   return _lastMove;
// }

bool BasePlayerInfo::isPlayable() const {
  return !_aliveTanks.empty();
}

#pragma endregion

#pragma region ModelPreservedInterface
bool BasePlayerInfo::isOwnerOf(const BaseTank* tank) const {
  return this == tank->getOwner();
}

bool BasePlayerInfo::removeTank(BaseTank* tank) {
  list<ITank*>::iterator it 
    = find(_aliveTanks.begin(), _aliveTanks.end(), tank);
  if (it != _aliveTanks.end()) {
    tank->kill();
    _deadTanks.push_back(tank);
    _aliveTanks.erase(it);
    return true;
  }
  return false;
}

bool BasePlayerInfo::getHit(BaseTank* tank) {
  list<ITank*>::iterator it 
    = find(_aliveTanks.begin(), _aliveTanks.end(), tank);

  if (it != _aliveTanks.end()) {
    tank->decreaseHP();
    if (!tank->isAlive()) {
      _deadTanks.push_back(tank);
      _aliveTanks.erase(it);
      return true;
    }
  }
  return false;
}

bool BasePlayerInfo::getHit(const pair<int,int>& pos, BaseTank*& tank) {
  list<ITank*>::iterator it;

  for (it = _aliveTanks.begin(); it != _aliveTanks.end(); ++it) {
    if ((*it)->getPosition() == pos)
      break;
  }

  if (it != _aliveTanks.end()) {
    tank = (BaseTank*)(*it);
    tank->decreaseHP();
    if (!tank->isAlive()) {
      _deadTanks.push_back(tank);
      _aliveTanks.erase(it);
      return true;
    }
  }
  return false;
}

// void updateLastMove(const Command& cmd) {
//   _lastMove = cmd;
// }

void BasePlayerInfo::addTank(int hp, int ammo, int range, pair<int,int> pos) {
  BaseTank* newTank = new BaseTank(hp, ammo, range, pos, this);

  _baseTanks.push_back(newTank);
  _aliveTanks.push_back(newTank);
}

BasePlayerInfo::BasePlayerInfo(char id, const pair<int,int>& head) :
  _mapID(id), _headquarterPosition(head) {}

BasePlayerInfo::~BasePlayerInfo() {
  for (int i = 0; i < _baseTanks.size(); i++)
    delete _baseTanks[i];
}

#pragma endregion