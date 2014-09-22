//
//  Model.cpp
//  proGL
//
//  Created by el1ven on 19/9/14.
//  Copyright (c) 2014 el1ven. All rights reserved.
//

#include "Model.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <GLUT/GLUT.h>
#include <math.h>

int j = 0;

Model::Model(string filePath){//读取文件的构造函数
    string *str1 = new string();//每行开头第一个无用单词
    string *str2 = new string();//每行开头第二个无用单词
    ifstream fin(filePath);
    string *everyLine = new string();//针对每一行数据
    
    if(!fin){//读取文件失败的情况
        cout<<"Cannot open this file!"<<endl;
        return;
    }
    
    while (getline(fin, *everyLine)) {//一行一行读文件
        
        if((*everyLine)[0] == 'V'){//读取顶点
            
            istringstream sin(*everyLine);
            string *s1 = new string();
            string *s2 = new string();
            string *s3 = new string();
            sin>>*str1>>*str2>>*s1>>*s2>>*s3;
            
            struct HE_vert *v = new HE_vert();
            
            istringstream *stream1 = new istringstream();
            (*stream1).str(*s1);
            float *index1 = new float();
            (*stream1) >> *index1;
            (*v).x = *index1;
            delete stream1;
            delete index1;
            
            istringstream *stream2 = new istringstream();
            (*stream2).str(*s2);
            float *index2 = new float();
            (*stream2) >> *index2;
            (*v).y = *index2;
            delete stream2;
            delete index2;
            
            istringstream *stream3 = new istringstream();
            (*stream3).str(*s3);
            float *index3 = new float();
            (*stream3) >> *index3;
            (*v).z = *index3;
            delete stream3;
            delete index3;
            
            //cout<<(*v).x<<"*"<<(*v).y<<"*"<<(*v).z<<endl;
            
            vertexList.push_back(*v);
            delete v;
            
            sin.clear();
            delete s1;
            delete s2;
            delete s3;
        }
        
        if((*everyLine)[0] == 'F'){//读取面
            
            istringstream sin(*everyLine);
            string *s1 = new string();
            string *s2 = new string();
            string *s3 = new string();
            sin>>*str1>>*str2>>*s1>>*s2>>*s3;
            
            vector<int> *tempFace = new vector<int>();
            
            
            istringstream *stream1 = new istringstream();
            (*stream1).str(*s1);
            int *index1 = new int();
            (*stream1) >> *index1;
            (*tempFace).push_back(*index1);
            delete stream1;
            delete index1;
            
            istringstream *stream2 = new istringstream();
            (*stream2).str(*s2);
            int *index2 = new int();
            (*stream2) >> *index2;
            (*tempFace).push_back(*index2);
            delete stream2;
            delete index2;
            
            istringstream *stream3 = new istringstream();
            (*stream3).str(*s3);
            int *index3 = new int();
            (*stream3) >> *index3;
            (*tempFace).push_back(*index3);
            delete stream3;
            delete index3;
            
            //cout<<(*tempFace)[0]<<"*"<<(*tempFace)[1]<<"*"<<(*tempFace)[2]<<endl;
            
            tempFaceList.push_back(*tempFace);//临时存储面上的点索引
            delete tempFace;
            
            sin.clear();
            delete s1;
            delete s2;
            delete s3;
        }
    }
    
    //根据上方读取得到的临时面数据构建半边结构的面集以及边集的部分数据，并同时补全点集中缺失的数据
    vector<vector<int>>::iterator it = tempFaceList.begin();
    for(; it != tempFaceList.end(); it++){
        
        HE_face *f = new HE_face();
        int num = 3;//每个面都有3个顶点的索引值
        
        HE_edge *edges = new HE_edge[num];
        
        /*for(int i = 0; i < num; i++){//先处理每个边拥有哪个起始点
            int index = 0;
            index = (*it)[i] - 1;
            edges[i].vert = &vertexList[index];//索引需要减1
        }*/
        
        for(int j = 0; j < num; j++){
            int index = 0;//临时变量用来存储下标
            index = (*it)[j] - 1;
            edges[j].vert = &vertexList[index];
            edges[j].next = &edges[(j+1)%num];
            //cout<<j<<"---"<<edges[j].next->vert->x<<endl;
            edges[j].face = f;
            
            if((vertexList[index]).edge == NULL) {
                (vertexList[index]).edge = &edges[j];
            }
            edgeList.push_back(&edges[j]);
            
            //cout<<edges[j].pair->vert->x<<endl;
        }
        
        (*f).edge = &edges[0];
        faceList.push_back(*f);
        
        //delete edges;
        //delete f;
    }
    
    //两层循环，根据上面构建得到的边集数据来补全边集自身缺失的数据
    //具体做法是嵌套两层遍历边集数据的循环，根据半边结构的特性来找出每条边对应的另一条边半边，并补全其缺失对边的数据
    vector<HE_edge *>::iterator it1 = edgeList.begin();
    vector<HE_edge *>::iterator it2;
    
    for(; it1 != edgeList.end(); it1++){
        //if(it1->pair == NULL)continue;
        for(it2 = it1; it2 != edgeList.end(); it2++){
            if((*it1)->next->vert->x == (*it2)->vert->x &&
               (*it1)->next->vert->y == (*it2)->vert->y &&
               (*it1)->next->vert->z == (*it2)->vert->z &&
               (*it2)->next->vert->x == (*it1)->vert->x &&
               (*it2)->next->vert->y == (*it1)->vert->y &&
               (*it2)->next->vert->z == (*it1)->vert->z){
                (*it1)->pair = &*(*it2);
                (*it2)->pair = &*(*it1);
                break;
            }
        }
        
        //cout<<it1->pair->vert->x<<endl;
    }
    
    
    /*vector<HE_edge>::iterator it3;
    for(it3 = edgeList.begin();it3 != edgeList.end(); it3++){
        if(it3->pair != NULL){
            cout<<it3->pair->vert->x<<"*"<<it3->pair->vert->y<<""<<it3->pair->vert->z<<endl;
            cout<<"Not NULL"<<endl;
        }else{
            cout<<"NULL"<<endl;
        }
    }*/
    
    //计算每个顶点的法线向量
    
    
    for(int i = 0; i < vertexList.size(); i++){

        HE_edge *current;
        current = vertexList[i].edge;
        //if(vertexList[i].edge->pair == NULL)
        //cout<<vertexList[i].edge->pair->vert->x<<endl;
        HE_normal sum;
        
        do{
            HE_vert v1,v2;
            //cout<<current->next->next->vert->x<<endl;
            v1.x = current->next->vert->x - vertexList[i].x;
            v1.y = current->next->vert->y - vertexList[i].y;
            v1.z = current->next->vert->z - vertexList[i].z;
            v2.x = current->next->next->vert->x - current->next->vert->x;
            v2.y = current->next->next->vert->y- current->next->vert->y;
            v2.z = current->next->next->vert->z - current->next->vert->z;
            HE_normal n;
            n = normCrossProd(v1, v2);
            sum.x += n.x;
            sum.y += n.y;
            sum.z += n.z;
            current = current->pair->next;
        }while(current != vertexList[i].edge);
    
        
        double d = sqrt(sum.x*sum.x +sum.y*sum.y +sum.z*sum.z);
        sum.x /=d ; sum.y /=d ; sum.z /= d;
        //cout<<sum.x<<"*"<<sum.y<<"*"<<sum.z<<endl;
        vertexList[i].nx = sum.x;
        vertexList[i].ny= sum.y;
        vertexList[i].nz = sum.z;
    }
    
}


void Model::drawModel(){//画出模型
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    //glEnable(GL_NORMALIZE);
    for(int i = 0; i < faceList.size(); i++){
        glBegin(GL_POLYGON);
        HE_edge *current;
        current  = faceList[i].edge;
        do{
            glNormal3f(current->vert->nx,current->vert->ny,current->vert->nz);
            //cout<<current->vert->nx<<"*"<<current->vert->ny<<"*"<<current->vert->nz<<endl;
            glVertex3f(current->vert->x, current->vert->y, current->vert->z);
            current = current->next;
        }while(current != faceList[i].edge);
        glEnd();
    }
}



HE_normal Model:: normCrossProd(HE_vert v1, HE_vert v2){
    
    HE_normal n;
    n.x = v1.y * v2.z - v1.z * v2.y;
    n.y = v1.z * v2.x - v1.x * v2.z;
    n.z = v1.x * v2.y - v1.y * v2.x;
    
    float d = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
    n.x /= d;//坐标归一化
    n.y /= d;
    n.z /= d;
    return n;
}




