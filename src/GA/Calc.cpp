#include "define.hpp"
#include <cstring>

bool Calc()
{
	bool vis[MAX_TARGET_NUM+1];
	memset(vis,false,sizeof(vis));
	for(int i=1;i<=truck_num;i++)
	{
		if(!next[i])
		{
			if(vis[i]) return false;
			vis[i]=true;
		}
		else
		{
			if(vis[next[i]]) return false;
			vis[next[i]]=true;
		}
	}
	return true;
}
