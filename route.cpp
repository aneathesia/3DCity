#include "route.h"

route::route()
{

}
void route::generatedata()
{
    data={};
    for(int i=0;i<pos.size();i++)
    {
        data.push_back(pos[i].x());
        data.push_back(pos[i].y());
        data.push_back(pos[i].z()-3);
    }
}
void route::init(QString shadername)
{

}
void route::init()
{

}

