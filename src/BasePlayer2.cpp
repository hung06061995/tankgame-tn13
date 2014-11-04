#include "BasePlayer2.h"
#include <list>
#include <cstdlib>

using namespace std;

const pair<int, int> delta[4] = {
	pair<int, int>(0, -1), // north
	pair<int, int>(0, 1), // south
	pair<int, int>(1, 0), // east
	pair<int, int>(-1, 0) // west
};

const int MAX = 1e6;

BasePlayer2::BasePlayer2()
: _playerInfo(NULL), _gameInfo(NULL) {}

BasePlayer2::~BasePlayer2() {}

void BasePlayer2::onBindPlayerInfo(IPlayerInfo* playerInfo) {
	_playerInfo = playerInfo;
}
void BasePlayer2::onBindGameInfo(IGameInfo* gameInfo) {
	_gameInfo = gameInfo;
}

IPlayerInfo* BasePlayer2::getPlayerInfo() const {
	return _playerInfo;
}

bool BasePlayer2::onStart() {
	return true;
}

bool BasePlayer2::onFinish(){
	return true;
}

//Dùng thuật dijkstra tìm đường đi ngắn nhất từ tank mình đến trụ sở
//Trả về hướng đi, kèm tham chiếu dis mô tả độ dài đường đi
int move(IPlayerInfo* player, IGameInfo* gameInfo, ITank* tank, int& dis){
	IMap* map = gameInfo->getMap();
	int width = map->getWidth();
	int height = map->getHeight();

	bool **avail = new bool*[width];
	int **distance = new int*[width];
	int **edge = new int*[width];
	int **trace = new int*[width];

	char myID = player->getPlayerMapID();
	for (int i = 0; i < width; ++i){
		avail[i] = new bool[height];
		distance[i] = new int[height];
		edge[i] = new int[height];
		trace[i] = new int[height];
		for (int j = 0; j < height; ++j){
			avail[i][j] = true;
			distance[i][j] = MAX;
			if (map->isWater(i, j) || (map->isHeadquarter(i, j) && !map->isHeadquarter(i, j, myID)))
				edge[i][j] = MAX;
			else
				edge[i][j] = 1;
			trace[i][j] = -1;
		}
	}
	
	list<IBlock*> blocks = gameInfo->getOnMapBlocks();
	for (list<IBlock*>::iterator it = blocks.begin(); it != blocks.end(); ++it){
		pair<int, int> pos = (*it)->getPosition();
		edge[pos.first][pos.second] = (*it)->getHP() + 1;
	}

	pair<int, int> root = player->getHeadquarterPosition();
	pair<int, int> posTank = tank->getPosition();
	distance[posTank.first][posTank.second] = 0;

	list<ITank*> tanks = player->getAliveTanks();
	for (list<ITank*>::iterator it = tanks.begin(); it != tanks.end(); ++it)
	if ((*it) != tank){
		pair<int, int> pos = (*it)->getPosition();
		edge[pos.first][pos.second] = MAX >> 1;
	}

	while (1){
		int minval = MAX;
		pair<int, int> u = pair<int, int>(-1, -1);
		for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		if (avail[i][j] && minval > distance[i][j]){
			minval = distance[i][j];
			u = pair<int, int>(i, j);
		}

		if (u == pair<int, int>(-1, -1) || u == root)
			break;
		avail[u.first][u.second] = false;

		pair<int, int> v;
		for (int i = 0; i < 4; ++i){
			v.first = u.first + delta[i].first;
			v.second = u.second + delta[i].second;
			if (map->isOnMap(v) && avail[v.first][v.second] && 
				distance[v.first][v.second] > distance[u.first][u.second] + edge[v.first][v.second]){
				distance[v.first][v.second] = distance[u.first][u.second] + edge[v.first][v.second];
				trace[v.first][v.second] = i;
			}
		}
	};

	int res;
	dis = distance[root.first][root.second];

	while (1){
		res = trace[root.first][root.second];
		root.first -= delta[res].first;
		root.second -= delta[res].second;
		if (root == posTank)
			break;
	}

	for (int i = 0; i < width; ++i){
		delete[] avail[i];
		delete[] distance[i];
		delete[] edge[i];
		delete[] trace[i];
	}
	delete[] avail;
	delete[] distance;
	delete[] edge;
	delete[] trace;

	return res;
}

//Tìm tank đối thủ nằm trong range của tank mình
ITank* fire(IPlayerInfo *player, IGameInfo* gameInfo, ITank* tank){
	vector <IPlayerInfo*> players = gameInfo->getPlayersInfo();
	IPlayerInfo *playerFriend;

	for (int i = 0; i < players.size(); ++i)
	if (players[i]->getPlayerMapID() != player->getPlayerMapID()){
		playerFriend = players[i];
		break;
	}

	list<ITank*> alives = playerFriend->getAliveTanks();
	list<ITank*>::iterator it = alives.begin();
	pair<int, int> mytank = tank->getPosition();
	for (it = alives.begin(); it != alives.end(); ++it){
		pair<int, int> enemytank = (*it)->getPosition();
		int disA = abs(mytank.first - enemytank.first);
		int disB = abs(mytank.second - enemytank.second);
		if (disA + disB <= tank->getRange())
			return *it;
	}
	return NULL;
}

Command BasePlayer2::nextMove() {
	list<ITank*> alives = _playerInfo->getAliveTanks();
	int numOfTank = alives.size();

	if (numOfTank > 0) {
		Command::Action actchoices[2] = { Command::MOVE, Command::FIRE };

		list<ITank*>::iterator it = alives.begin();
		ITank* nextTank = NULL;

		int choice = 0; // 0: move, 1: fire
		int direction = rand() % 4; //0: north, 1: south, 2: east, 3: west
		int dis;
		
		for (it = alives.begin(); it != alives.end(); ++it){
			//Dùng thuật dijkstra tìm đường đi ngắn nhất với mỗi map được update
			int tmp = move(_playerInfo, _gameInfo, *it, dis);
			if (dis != 1 && dis != (MAX >> 1) + 1){
				nextTank = *it;
				direction = tmp;
				break;
			}
			else{
				//Nếu tank đã về đích thì đứng yên hoặc nã vào đối thủ trong range
				ITank *enemy = fire(_playerInfo, _gameInfo, *it);
				if (enemy != NULL){
					choice = 1;
					return Command(*it, actchoices[choice],
						enemy->getPosition().first,
						enemy->getPosition().second);
				}
			}
		}

		if (nextTank == NULL)
			return Command();

		pair<int, int> curPos = nextTank->getPosition();

		//Nếu o tiếp theo cần đi tới là vật cản thì chuyển hướng sang bắn
		pair<int, int> nextPos;
		nextPos.first = curPos.first + delta[direction].first;
		nextPos.second = curPos.second + delta[direction].second;
		if (_gameInfo->getMap()->isBlock(nextPos.first, nextPos.second))
			choice = 1;

		//Phá vật cản với range = 1, vì đứng sát
		int range = 1;

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

/*
void BasePlayer2::getResult(int& availITankCnt, int& HPRemain, int& sumDistance, bool other){
	availITankCnt = HPRemain = sumDistance = 0;
	vector <IPlayerInfo*> players = _gameInfo->getPlayersInfo();
	IPlayerInfo *player = _playerInfo;
	if (other)
	for (int i = 0; i < players.size(); ++i)
	if (players[i]->getPlayerMapID() != player->getPlayerMapID()){
		player = players[i];
		break;
	}


	IMap *map = _gameInfo->getMap();
	int width = map->getWidth();
	int height = map->getHeight();
	list <ITank*> alives = player->getAliveTanks();
	char playerID = player->getPlayerMapID();

	//Khởi tạo mảng đánh dấu và độ dài đường đi
	bool **avail = new bool*[height];
	int **distance = new int*[height];
	for (int i = 0; i < height; ++i){
		avail[i] = new bool[width];
		distance[i] = new int[width];
		for (int j = 0; j < width; ++j){
			//Kiểm tra (i, j) có đang trống không?
			avail[i][j] = map->isEmptySpace(j, i);
			//Kiểm tra thêm (i, j) có phải là cầu hay đang chứa xe tăng của đội đang xét
			if (map->isTank(j, i, playerID) || map->isBridge(j, i))
				avail[i][j] = true;
			distance[i][j] = -1;
		}
	}

	pair<int, int> direction[4] = {
		pair<int, int>(0, -1), // bắc
		pair<int, int>(0, 1), // nam
		pair<int, int>(1, 0), // đông
		pair<int, int>(-1, 0) // tây
	};

	//Dùng bfs loang từ headquarter ra tất cả các ô khác trên bản đồ
	queue <pair<int, int> > que;
	pair<int, int> root = player->getHeadquarterPosition();
	que.push(root);
	distance[root.second][root.first] = 0;

	while (!que.empty()){
		pair <int, int> u = que.front();
		que.pop();
		for (int i = 0; i < 4; ++i){
			pair <int, int> v = u;
			v.first += direction[i].first;
			v.second += direction[i].second;

			if (v.first >= 0 && v.first < width && v.second >= 0 && v.second < height)
			if (avail[v.second][v.first]){
				avail[v.second][v.first] = false;
				que.push(v);
				distance[v.second][v.first] = distance[u.second][u.first] + 1;
			}
		}
	}


	std::list<ITank*>::iterator it;
	int i;
	//Duyệt qua tất cả xe tăng còn sống và tính thông số xe tăng đó
	for (it = alives.begin(), i = 0; it != alives.end() && i < alives.size(); ++it, ++i){
		pair<int, int> u = (*it)->getPosition();
		//Kiểm tra từ headquarter có đến được xe tăng ở tọa độ u không?
		if (distance[u.second][u.first] != -1){
			++availITankCnt;
			HPRemain += (*it)->getHP();
			sumDistance += distance[u.second][u.first];
		}
	}

	for (int i = 0; i < height; ++i){
		delete[] avail[i];
		delete[] distance[i];
	}
	delete[] avail;
	delete[] distance;
}
*/