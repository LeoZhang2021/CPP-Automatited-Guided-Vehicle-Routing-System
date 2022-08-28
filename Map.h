#pragma once
//预处理指令，保证头文件只被编译一次

#include <iostream>
#include "string"
#include "fstream"
#include "vector"
#include "queue"
#include "sstream"
#include "set"
#include "string.h"
#include "math.h"
#include <functional>

using namespace std;

#define INF 200000000
#define N 100

class Map
{
public:
	struct  Edge //边定义
	{
		int to;          // 边终止节点
		int cost;        // 权值

		Edge(int to1, int cost1)
		{
			to = to1;
			cost = cost1;
		}
	};
	struct Ans
	{
		vector<int> path;	//路径集，存储从起点到终点所需要经过的所有路径
		int cost; 			//路径的权值
		int start;

		void getCost()
		{
			cost = G1[start][path[0]]; 	// 第一条路径权值：起点到第一个路径节点
			for (int unsigned i = 0; i < path.size() - 1; i++)
			{
				cost += G1[path[i]][path[i + 1]]; 	//把后续所有路径权值加到cost里，cost最终为整条路径的权值
			}
		}
	};
	int startpoint;
	int turnMin;
	vector<Ans> paths;
	static int G1[N][N];         // 图的邻接矩阵形式
	int dist[N];                 // 从源点出发的最短距离
	int nV;                      // 顶点数
	int nE;
	
	vector<Edge> G[N];
	vector<Edge> G4[N];			 // 存储图中所有两点之间最短路径，通过 Dijkstra 算法更新
								 // 注意： G4 是二维动态数组
	bool vis[N];				 // 是否访问过
	typedef pair<int, int> P;    // first是最短距离，second是顶点编号
public:
	Map();
	~Map();
	void solve();
	void build();		//建立临接矩阵
	void dijkstra(int s, vector<Edge> G[N]);            // dijkstra算法 求最短路径
	void addEdge(int from, int to, int cost, vector<Edge> G[N]);//双向赋值，1-2和2-1是一样的cost
	void dfs(int s, int t, Ans &A, vector< Ans > &paths, int start); //深搜算法

};