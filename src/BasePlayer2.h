#ifndef __TANKGAME_BASEPLAYER2__
#define __TANKGAME_BASEPLAYER2__

#include "IPlayer.h"
#include "IPlayerInfo.h"
#include "IGameInfo.h"
#include "Command.h"
#include <list>
#include <vector>
#include <queue>
using namespace std;

class BasePlayer2 : public IPlayer {
public:
	BasePlayer2();
	~BasePlayer2();

	void onBindPlayerInfo(IPlayerInfo* playerInfo);
	void onBindGameInfo(IGameInfo* gameInfo);

	IPlayerInfo* getPlayerInfo() const;

	bool onStart();
	bool onFinish();
	Command nextMove();

	//Nếu other = true là lấy thông số kết quả của đội bạn, ngược lại lấy của mình
	void getResult(int& availITankCnt, int& HPRemain, int& sumDistance, bool other = false);

protected:
	IPlayerInfo* _playerInfo;
	IGameInfo* _gameInfo;

	int _currentTank;
};
#endif