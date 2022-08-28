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
#include "AGV.h"
#include "AGV.cpp"
#include"Map.h"
#include"Map.cpp"

using namespace std;
/*
在为编号AGV发布任务后。计算任务所需时间（秒）
在该编号AGV执行任务期间，只能给其他小车发布任务
若所有小车都处于任务中，则自动推进/更新时间，知道有小车空出来
10 秒之后，自动退出（注：此处的秒为程序内部时间，并非钟表时间）
*/

int main()
{
	bool isFull[N][N]; // 前一个参数是点，后一个参数是时间，表示什么时间什么点被占据
	AGV agv1;
	AGV agv2;
	int time = 1; //初始化时间
	Map map;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			isFull[i][j] = false; 
		}
	}
	
	while (true)
	{
		cout << "第" << time << "秒：" << endl;
		if (agv1.checkFree) // 规划任务路径
		{
			int n;
			cout << "是有否任务给1号车（0.是，1.否）" << endl;
			cin >> n;
			if (n == 0)
			{
				agv1.agvMap = new Map;
				agv1.agvMap->solve();	// 规划矩阵最短路径，存储到 paths ( Map ) 里
				agv1.setStart(isFull,time);	// 找出不与其他路径冲突的最优路径，存储到 AGV_path ( AGV ) 里
			}
		}
		if (agv2.checkFree)
		{
			int n;
			cout << "是否有任务给2号车（0.是，1.否）" << endl;
			cin >> n;
			if (n == 0)
			{
				agv2.agvMap = new Map;
				agv2.agvMap->solve();
				agv2.setStart(isFull,time);
			}
		}
		// 执行任务路径，每次移动一个点位
		cout << "A:";
		agv1.move();
		cout << "B:";
		agv2.move();
		
		time++;	//每移动一次，时间增加一秒
		if (time == 10)	// 10秒退出
			exit(0);
	}
	return 0;
}
