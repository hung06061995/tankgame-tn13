#include "MY_Stage1.h"
#include <queue>

using namespace std;

MY_Stage1::MY_Stage1() : MY_IStage(NULL, NULL)
{
	endStage = false;
}

MY_Stage1::MY_Stage1(IPlayerInfo *playerInfo, IGameInfo *gameInfo) : MY_IStage(playerInfo, gameInfo)
{
	endStage = false;
}


MY_Stage1::~MY_Stage1()
{
}

bool MY_Stage1::isEndStage()
{

	return false;
}

template <class T>
void clearAll(queue<T>& q)
{
	while (!q.empty())
		q.pop();
}

template <class T>
void createVector2d(vector<vector<T> >& vt, int firstSize, int secondSize, const T& value)
{
	vt.clear();
	vt.resize(firstSize);

	for (int i = 0; i < firstSize; ++i)
		vt[i].resize(secondSize, value);
}

#define ammu first
#define distance second.first
#define position second.second
#define make_triple(a, b, c) make_pair((a), make_pair((b), (c)))

void MY_Stage1::getInfoByDijkstra(const vectorInt2d& mapHP, vectorPair2d& nextPos, vectorInt2d& dis, vectorInt2d& amm)
{
	
	typedef pair<int, pair<int, pair<int, int> > > triple;
	const int dx[] = { -1, 1, 0, 0 };
	const int dy[] = { 0, 0, -1, 1 };
	const int rate = 4000; //equal to the map size

	IMap *_map = this->_gameInfo->getMap();
	int _width = mapHP.size();
	int _height = mapHP[0].size();
	priority_queue<triple, vector<triple>, greater<triple> > q;
	vector<vector<bool> > isVisit;

	createVector2d(nextPos, _width, _height, make_pair(0, 0));
	createVector2d(dis, _width, _height, infinity);
	createVector2d(amm, _width, _height, infinity);
	createVector2d(isVisit, _width, _height, false);
	for (int i = 0; i < _width; ++i)
	for (int j = 0; j < _height; ++j)
	if (this->_gameInfo->getMap()->isWater(i, j))
		isVisit[i][j] = true;
	
	pair<int, int> headQuarter = this->_playerInfo->getHeadquarterPosition();

	dis[headQuarter.first][headQuarter.second] = 0;
	amm[headQuarter.first][headQuarter.second] = 0;

	q.push(make_triple(0, 0, headQuarter));

	while (!q.empty())
	{
		triple u = q.top();
		pair<int, int> upos = u.position;
		q.pop();
		
		if (u.ammu != amm[upos.first][upos.second] || u.distance != dis[upos.first][upos.second])
			continue;
		isVisit[upos.first][upos.second] = true;

		for (int i = 0; i < 4; ++i)
		{
			pair<int, int> vpos(upos.first + dx[i], upos.second + dy[i]);
			if (!_map->isOnMap(vpos))
				continue;

			if (isVisit[vpos.first][vpos.second])
				continue;

			
			int tempAmm = amm[upos.first][upos.second] + mapHP[vpos.first][vpos.second];
			int tempDis = dis[upos.first][upos.second] + 1;
			if (tempAmm < amm[vpos.first][vpos.second] || (tempAmm == amm[vpos.first][vpos.second] && tempDis < dis[vpos.first][vpos.second]))
			{
				amm[vpos.first][vpos.second] = tempAmm;
				dis[vpos.first][vpos.second] = tempDis;
				nextPos[vpos.first][vpos.second] = upos;
				q.push(make_triple(tempAmm, tempDis, vpos));
			}
		}
	}

	

}

#undef distance
#undef ammu
#undef position
#undef make_triple(a, b, c)



void MY_Stage1::getNextPosition(vectorPair2d& nextPos, vectorInt2d& dis, vectorInt2d& amm)
{
	const int numberOfDir = 4;
	const int dx[] = { -1, 1, 0, 0 };
	const int dy[] = { 0, 0, -1, 1 };
	
	IMap *map = this->_gameInfo->getMap();

	int width = map->getWidth();
	int height = map->getHeight();

	vectorInt2d mapHP;
	createVector2d(mapHP, width, height, infinity);
	//mapHP.resize(width);
	//for (int i = 0; i < width; ++i)
	//	mapHP[i].resize(height, infinity);

	list<IMapObject*> iob;
	list<IBlock*> lb = this->_gameInfo->getOnMapBlocks();
	list<ITank*> lt = this->_playerInfo->getAliveTanks();
	iob.insert(iob.end(), lt.begin(), lt.end());
	iob.insert(iob.end(), lb.begin(), lb.end());
	

	for (list<IMapObject*>::iterator it = iob.begin(); it != iob.end(); ++it)
	if ((*it)->isOnMap())
	{
		pair<int, int> pos = (*it)->getPosition();
		mapHP[pos.first][pos.second] = (*it)->getHP();
	}
	

	for (int i = 0; i < width; ++i)
	for (int j = 0; j < height; ++j)
	{
		char id = (*map)(i, j);

		//if this position is an empty land, bridge, or my tank, sets HP
		if (map->isEmptySpace(id) || map->isBridge(id))
		{
			mapHP[i][j] = 0;
		}
	}
	
	this->getInfoByDijkstra(mapHP, nextPos, dis, amm);
	/*
	

	nextPos.resize(width);
	dis.resize(width);
	for (int i = 0; i < width; ++i)
	{
		nextPos[i].resize(height);
		dis[i].resize(height);

		for (int j = 0; j < height; ++j)
		{
			nextPos[i][j].first = nextPos[i][j].second = -1;
			dis[i][j] = -1;
		}
			
	}
	*/
	/*
	queue<pair<int, queue<pair<int, int> > > > nextQ;
	queue<pair<int, int> > temp;
	clearAll(nextQ);
	clearAll(temp);

	pair<int, int> headQuarter = this->_playerInfo->getHeadquarterPosition();
	temp.push(headQuarter);
	nextQ.push(make_pair(0, temp));

	while (!nextQ.empty())
	{
		//current queue
		queue<pair<int, int> > q = nextQ.front().second;
		int CurrentDis = nextQ.front().first;
		nextQ.pop();
		//current next queue
		queue<pair<int, int> > nq; 
		clearAll(nq);
		
		while (!q.empty())
		{
			pair<int, int> u = q.front();
			q.pop();

			//if this distance of u is not the shortest distance, ignores this position
			if (CurrentDis > dis[u.first][u.second])
				continue;

			for (int i = 0; i < numberOfDir; ++i)
			{
				pair<int, int> v(u.first + dx[i], u.second + dy[i]);
				if (!map->isOnMap(v))
					continue;

				char id = (*map)(v.first, v.second);
				
				if (map->isWater(id))
					continue;


				//if 
			}
		}
		
		if (nq.size() > 0)
			nextQ.push(make_pair(CurrentDis, nq));
	}

	*/
//	return ret;
	
}

Command MY_Stage1::nextMove()
{
	vectorPair2d _nextPos;
	vectorInt2d _distance, _ammu;

	this->getNextPosition(_nextPos, _distance, _ammu);

	list<ITank*> alives = this->_playerInfo->getAliveTanks();
	ITank *chosenOne = NULL;

	for (list<ITank*>::iterator it = alives.begin(); it != alives.end(); ++it)
	{
		pair<int, int> pos = (*it)->getPosition();
		if (_distance[pos.first][pos.second] >= infinity)
			continue;
		if (isMyTank(_nextPos[pos.first][pos.second]))
			continue;
		if (_gameInfo->getMap()->isHeadquarter(_nextPos[pos.first][pos.second].first, _nextPos[pos.first][pos.second].second))
			continue;

		if (chosenOne == NULL)
			chosenOne = *it;
		else
		{
			pair<int, int> tPos = chosenOne->getPosition();
			if (_ammu[pos.first][pos.second] < _ammu[tPos.first][tPos.second]
				|| (_ammu[pos.first][pos.second] == _ammu[tPos.first][tPos.second]
				&& _distance[pos.first][pos.second] < _distance[tPos.first][tPos.second]))
				chosenOne = *it;
		}
	}

	if (chosenOne == NULL)
	{
		endStage = true;
		return Command();
	}
		

	pair<int, int> cPos = chosenOne->getPosition();
	pair<int, int> next = _nextPos[cPos.first][cPos.second];

	//if next position is an empty space, moves
	if (isAvailable(next))
		return Command(chosenOne, Command::MOVE, next.first, next.second);

	//otherwise, shoots that position
	return Command(chosenOne, Command::FIRE, next.first, next.second);
}