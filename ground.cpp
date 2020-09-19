#include "ground.h"

ground::ground()
{

}
ground::ground(QVector3D nw,QVector3D ne,QVector3D se,QVector3D sw)
{
    pos.push_back(nw);
    pos.push_back(ne);
    pos.push_back(se);
    pos.push_back(sw);
}

void ground::generatedata()
{
    for(int i=0;i<pos.size();i++)
    {
        data.push_back(pos[i].x());
        data.push_back(pos[i].y());
        data.push_back(pos[i].z());
    }
}
