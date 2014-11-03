#ifndef __TANKGAME_BASEPLAYER__
#define __TANKGAME_BASEPLAYER__

#include "IPlayer.h"
#include "IPlayerInfo.h"
#include "IGameInfo.h"
#include "Command.h"
#include "MY_Strategy.h"
#include "MY_Stage1.h"

class BasePlayer : public IPlayer {
public:
  BasePlayer();
  ~BasePlayer();

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