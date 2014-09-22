//
//  Model.h
//  proGL
//
//  Created by el1ven on 19/9/14.
//  Copyright (c) 2014 el1ven. All rights reserved.
//

#ifndef __proGL__Model__
#define __proGL__Model__

#include <iostream>
#include <vector>
#include <list>

using namespace std;

//结构体详细信息
struct HE_vert{//顶点
    float x;//顶点坐标
    float y;
    float z;
    struct HE_edge *edge;//从这个顶点为起点发射出去的一条半边
    float nx,ny,nz;
};

struct HE_edge{
    struct HE_vert *vert;//在半边末尾的顶点
    struct HE_edge *pair;//相反方向的半边
    struct HE_face *face;//半边对应的面
    struct HE_edge *next;//指向下一条半边
};

struct HE_face{
    struct HE_edge *edge;//组成这个面的一条半边
};

struct HE_normal{
    float x;
    float y;
    float z;
};

class Model{
private:
    //信息存储
    vector<HE_vert> vertexList;
    vector<HE_edge *> edgeList;
    vector<HE_face> faceList;
    //vector<HE_normal> normalList;
    vector<vector<int>> tempFaceList;
public:
    Model(string filePath);//把读文件操作直接放在参数构造函数里面
    ~Model();
    void drawModel();
    HE_normal normCrossProd(HE_vert v1, HE_vert v2);
};

#endif /* defined(__proGL__Model__) */
