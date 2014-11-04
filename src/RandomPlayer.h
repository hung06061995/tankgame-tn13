#ifndef __RandomPlayer__
#define __RandomPlayer__

#include "IPlayer.h"
#include "IPlayerInfo.h"
#include "IGameInfo.h"
#include "Command.h"
//#include "MY_Strategy.h"

class RandomPlayer : public IPlayer {
public:
  RandomPlayer();
  ~RandomPlayer();

  void onBindPlayerInfo(IPlayerInfo* playerInfo);
  void onBindGameInfo(IGameInfo* gameInfo);

  IPlayerInfo* getPlayerInfo() const;

  bool onStart();
  bool onFinish();
  Command nextMove();

protected:
  IPlayerInfo* _playerInfo;
  IGameInfo* _gameInfo;

  int _currentTank;

 // MY_Strategy *mySt;
};
#endif