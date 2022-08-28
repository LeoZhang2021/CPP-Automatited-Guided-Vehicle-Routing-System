#include"AGV.h"
#include"Map.h"

Map::Map()
{}

int Map::G1[N][N];	//临接矩阵

Map::~Map()
{}

void Map::addEdge(int from, int to, int cost, vector<Edge> G[N])	//为边的权值 双向赋值，1-2和2-1是一样的cost
{
	Edge e(to, cost);	//路径终点边对象初始化：在 to 节点的位置，从初始起点到 to 所需要的 cost
	G[from].push_back(e);

	Edge e1(from, cost);	//路径起点边对象初始化：在 from 节点的位置，从初始点到 from 所需要的 cost
	G[to].push_back(e1);
}

void Map::solve()
{
	int sta, finall;
	cout << "请输入起点:" << endl;
	cin >> sta;
	startpoint = sta;
	cout << "请输入终点:" << endl;
	cin >> finall;

	build();	//建立临接矩阵 G1
	dijkstra(sta, G);	//找到从起点到其他所有点的最短距离 存储在 G 中

	int unsigned i, j;

	//vector<Ans> paths;
	Ans ans;
	memset(vis, false, sizeof(vis)); // 初始化 是否访问过 为 否
	dfs(sta, finall, ans, paths, sta);	// paths：路径集

	cout << endl << sta << "到" << finall << "的所有最短路径：" << endl;
	int tsize;
	tsize = paths.size();
	int *t = new int[tsize];	//数组
	for (i = 0; i < paths.size(); i++)
	{
		*(t + i) = 0;
		cout << sta << " - ";
		for (j = 0; j < paths[i].path.size(); j++)
		// paths[i].path[j]：起点 -> i -> j 
		{
			cout << paths[i].path[j] << " - ";
			if (j > 1 && abs(paths[i].path[j] - paths[i].path[j - 2]) != 22 && abs(paths[i].path[j] - paths[i].path[j - 2]) != 2)
			{
				*(t + i) += 1;
			}
			else if (j == 1 && abs(paths[i].path[j] - sta) != 22 && abs(paths[i].path[j] - sta) != 2)
			{
				*(t + i) += 1;
			}
		}
		cout << "---cost：" << paths[i].cost << "   " << "转弯数：" << *(t + i) << endl;
	}
	cout << "     " << endl;
	cout << "     " << endl;
	turnMin = 1000;
	for (i = 0; i < paths.size(); i++) //寻找最小转弯数
	{
		if (turnMin > *(t + i))
		{
			turnMin = *(t + i);
		}
	}
	cout << "最优路径为：  " << endl;
	for (i = 0; i < paths.size(); i++) //输出最小转弯数所对应的路径
	{
		if (*(t + i) == turnMin)
		{
			cout << sta << " - ";
			for (j = 0; j < paths[i].path.size(); j++)
			{
				cout << paths[i].path[j] << " - ";
			}
			cout << "---cost：" << paths[i].cost << "   " << "转弯数：" << turnMin << endl;
		}
	}
	cout << "     " << endl;
	cout << "次优路径为：  " << endl;
	for (i = 0; i < paths.size(); i++) //输出次小转弯数所对应的路径
	{
		if (*(t + i) == turnMin + 1)
		{
			cout << sta << " -";
			for (j = 0; j < paths[i].path.size(); j++)
			{
				cout << paths[i].path[j] << " - ";
			}
			cout << "---cost：" << paths[i].cost << "   " << "转弯数：" << turnMin + 1 << endl;
		}
	}
}
void Map::build() //建立临接矩阵
{
	int i;
	ifstream fin;
	fin.open("data.txt"); //打开数据文件
	fin >> nV;//输入顶点数
	fin >> nE;//输入边数

	// 输入图，通过临接矩阵 初始化为所有两点距离为INF
	for (i = 0; i < nV; i++)
	{
		for (int j = i; j < nV; j++)
		{
			G1[i][j] = G1[j][i] = INF;
		}
	}
	int from, to, cost;//输入两点间距离
	for (i = 0; i < nE; i++)
	{
		fin >> from >> to >> cost;

		addEdge(from, to, cost, G);
		G1[from][to] = G1[to][from] = cost; //构建临接矩阵
	}
	fin.close();
}

void Map::dijkstra(int s, vector<Edge> G[N]) // 求起点到其他所有点的最短路径，存储在 dist 数组里
{
	fill(dist, dist + nV + 1, INF);		//把从源点出发 到 原点和终点之间任何点位 的最短距离 设为无限大
	priority_queue<P, vector<P>, greater<P> > q;	// 存储 pair 类型的队列, pair 中的 first是最短距离，second是顶点编号
	dist[s] = 0;		//起点到起点的路径 权为 0
	q.push(P(0, s));	//起点到起点的路径 入队
	while (!q.empty())
	{
		P p = q.top();   //从尚未使用的顶点中找到一个距离最小的顶点，访问并出队
		q.pop();
		int v = p.second;	//得到起点编号
		if (dist[v] < p.first) //如果 从起点到v的距离 比 从起点到队列中离起点权值最小的路径的距离 小，则跳出本次循环
			continue;
		for (int unsigned i = 0; i < G[v].size(); i++)
		{
			Edge &e = G[v][i];	//每次循环 检查从 V 到所有其他点的 距离
			int dis = dist[v] + e.cost;
			if (dist[e.to] > dis) //若通过 V 中转能得到权更小的路径
			{
				dist[e.to] = dist[v] + e.cost;	//更新路径
				q.push(P(dist[e.to], e.to));	//把新的路径入队
				G4[v].push_back(e);		//把 从 V 到其临界点的路径 加入 G4
			}
			else if (dist[e.to] == dis) //若通过 V 中转 最短路径长度无变化
			{
				G4[v].push_back(e);
			}
		}
	}
}

void Map::dfs(int s, int t, Ans &A, vector< Ans > &paths, int start)	
//深搜，遍历所有结点对之间的路径，最终目的为修改 paths 路径集
{ 
	if (s == t) // 返回条件：若起点等于终点
	{
		A.start = start;
		A.getCost();
		paths.push_back(A);
	}
	
	for (int unsigned i = 0; i < G4[s].size(); i++) //搜索 s 到其他所有点的路径和其权值
	{
		int u = G4[s][i].to;	// to ： 该段路径终端点
		if (!vis[u])
		{
			vis[u] = true;
			A.path.push_back(u);
			dfs(u, t, A, paths, start);
			A.path.pop_back();
			vis[u] = false;
		}
	}
}