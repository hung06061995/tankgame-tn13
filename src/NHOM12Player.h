#ifndef __TANKGAME_NHOM12Player__
#define __TANKGAME_NHOM12Player__

#include "IPlayer.h"
#include "IPlayerInfo.h"
#include "IGameInfo.h"
#include "Command.h"
#include "MY_AllStage.h"
#include "MY_Strategy.h"


class NHOM12Player : public IPlayer {
public:
  NHOM12Player();
  ~NHOM12Player();

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
  MY_Strategy *_strategy;
};
#endif