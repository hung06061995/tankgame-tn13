#include "RandomPlayer.h"

#include <list>
#include <cstdlib>
#include <iostream>

using namespace std;

RandomPlayer::RandomPlayer()
  : _playerInfo(NULL), _gameInfo(NULL)
{
	//adds stage to the strategy here.
}

RandomPlayer::~RandomPlayer() {}

void RandomPlayer::onBindPlayerInfo(IPlayerInfo* playerInfo) {
  _playerInfo = playerInfo;
}
void RandomPlayer::onBindGameInfo(IGameInfo* gameInfo) {
  _gameInfo = gameInfo;
}

IPlayerInfo* RandomPlayer::getPlayerInfo() const {
  return _playerInfo;
}

bool RandomPlayer::onStart() {
	
	return true;
}

bool RandomPlayer::onFinish(){
  return true;
}



Command RandomPlayer::nextMove() {
  list<ITank*> alives = _playerInfo->getAliveTanks();
  int numOfTank = alives.size();
  //cout << this->_gameInfo->getMap()->getHeight();
  if (numOfTank > 0) {
    int next = rand() % numOfTank;

    list<ITank*>::iterator it;
    int j;
    for (it = alives.begin(), j = 0; it != alives.end() && j < next; ++it, ++j)
      ;

    ITank* nextTank = *it;

    int choice = rand() % 1; // 0: move, 1: fire
    int direction = rand() % 4; //0: north, 1: south, 2: east, 3: west


    Command::Action actchoices[2] = {Command::MOVE, Command::FIRE};
    pair<int,int> delta[4] = {
      pair<int,int>(0,-1), // north
      pair<int,int>(0,1), // south
      pair<int,int>(1,0), // east
      pair<int,int>(-1,0) // west
    };

    pair<int,int> curPos = nextTank->getPosition();

    int range = rand() % nextTank->getRange() + 1;

    switch (actchoices[choice]) {
      case Command::MOVE:
        return Command(nextTank, actchoices[choice],
          curPos.first + delta[direction].first,
          curPos.second + delta[direction].second);
      case Command::FIRE:
        return Command(nextTank, actchoices[choice],
          curPos.first + range * delta[direction].first,
          curPos.second + range * delta[direction].second);
    }
  }

  return Command();
}
