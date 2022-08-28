#include"AGV.h"
#include "Map.h"
#include<iostream>

using namespace std;

class AGV{

	public:
		~AGV();
		Map *agvMap;	// 仓库图 Map对象
		int next_point; // 下一个点位
		int this_point; // 当前
		bool checkFree = true; // 是否有任务给AGV
		queue<int> agv_path;	//当前 AGV 有效路径，从 起点到第一个点 到 起点到最后一个点 依次入队
		int i = 0;

		void setStart(bool (&isFull)[N][N] ,int t); // 将路径输入AGV
		void move(); //移动
};

AGV::~AGV()
{
	delete agvMap;
	agvMap=NULL;
}

void AGV::move() //移动
{
	if (checkFree)
		next_point = this_point; //更新点位
	else 
	{
		this_point = next_point;
		if (i != 1)
		{
			next_point = agv_path.front();	//移动到 agv_path 中存储的队列顶部路径的终端点
			agv_path.pop(); 
		}
	}

	if (agv_path.empty())	//有效路径集为空说明该 AGV 已经到达终点
	{
		if (i == 1)
		{
			checkFree = true;
			i = 0;
			delete agvMap;
			agvMap = NULL;
		}
		i++;
	}	    
	cout << this_point << endl;	//输出当前所在点 （通过循环调用可输出整条路径）
}

void AGV::setStart(bool (&isFull)[N][N],int t) 
{
	//初始化
	cout << "setStart" << endl;
	checkFree = false;
	next_point = agvMap->startpoint;
	int time = t;

	for (int k = 0; i < agvMap->paths.size(); i++) //遍历所有路径组合
	{
		for (int j = 0; j < agvMap->paths[k].path.size(); j++)
		{
			if (isFull[agvMap->paths[k].path[j]][time])	// 路径上的结点是否在 time 时间 被占用
			{
				// 通过换路径防止碰撞
				cout << "已被占用的节点" << agvMap->paths[k].path[j] << "---" << "第" << time << "s" << endl; // 通过换路径防止碰撞
				while (!agv_path.empty())	//删除该路径上的所有结点
				{
					agv_path.pop();
					cout << "删除尾节点" << endl;
				}
				for (j=j-1;j >=0; j--)	//把删除的结点的占用标记改为否
				{
					cout << "释放占用的节点" << endl;
					isFull[agvMap->paths[k].path[j]][time] = false;
					time--;
				}
				if (j == 0)
					time = t;
				break;	//跳出循环
			}
			// 若结点未被占用，则占用该节点
			agv_path.push(agvMap->paths[k].path[j]);
			cout << "成功占用节点：" << agvMap->paths[k].path[j] << "---" << "第" << time << "s" << endl;
			isFull[agvMap->paths[k].path[j]][time] = true;
			time++;
		}
		if (!agv_path.empty())
			break;	//衔接上一个循环：若路径冲突，则跳出循环并重新规划路径
	}

	if (agv_path.empty()) // 若路径需要重新规划
	{					  
		for (int j = 0; j < agvMap->paths[0].path.size(); j++)
		{
			if (isFull[agvMap->paths[0].path[j]][time])	// 若检测到 从起点到j点路径 的 j点被占用
			{
				j--;	// 通过碰撞前停止防止碰撞
				agv_path.push(agvMap->paths[0].path[j]);	// 把碰撞点之前的路径加入 agv_path （有效路径）
				isFull[agvMap->paths[0].path[j]][time] = true;	//占点
				time++;
				continue;
			}
			agv_path.push(agvMap->paths[0].path[j]);	// 把当前路径加入有效路径
			isFull[agvMap->paths[0].path[j]][time] = true;	//占点
			time++;
		}
	}
	i = 0;	
}