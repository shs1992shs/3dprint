/*********************************************************************************
  *Copyright(C),2017-2017,conceptclear
  *FileName:  Patchmodel.h
  *Author:  conceptclear
  *Version:  1.0
  *Date:  2017.12.5
  *Description:  use this class to read the patches graph
**********************************************************************************/
#pragma once

#include "../basic components/CVertex.h"
#include "../basic components/CEdge.h"
#include "../basic components/CFacet.h"
#include "../slice/point.h"
#include <string>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>

class Patchmodel
{
    public:
        bool ReadASCII(const char *cfilename);//��ȡASCII���ʽ
        bool ReadBinary(const char *cfilename);//��ȡ��������ʽ
        bool ReadSTLFile(const char *cfilename);//��ȡSTL�ļ�����
        void drawPatch(void);
        bool slice(float z); //��Ƭ����
        void drawslice(float z); //������Ƭ
        void setperspective(double eyex,double eyey,double eyez,double centrex,double centrey,double centrez,double upx,double upy,double upz); //�����ӽ�
        bool sliceequalllayers(int layernumber); //�Ȳ����Ƭ
        void drawsliceequalllayers(int layernumber); //���ƵȲ����Ƭ
        bool slicefacet(unsigned long layernumber); //����ĳһ��ı�����Ƭ
        void drawslicefacet(void); //���Ʊ�����
        void drawAABB(void); //draw AABB box
        float xmax(void); //return x_max
        float ymax(void); //return y_max
        float zmax(void); //return z_max
        float xmin(void); //return x_min
        float ymin(void); //return y_min
        float zmin(void); //return z_min
        vector<CVertex> m_VectorPoint; //���������vector����
        vector<CEdge> m_VectorEdge; //�����ߵ�vector����
        vector<CFacet> m_VectorFacet; //�������vector����
    private:
        set<CVertex> m_SetPoint; //���������set����
        set<CEdge> m_SetEdge; //�����ߵ�set����
        CFacet m_Facet; //����CFacet����
        vector<vector<point> > z_point; //���������������
        vector<vector<vector<point> > > slicingdata; //���������������
        vector<set<int> > slicefacetnumber; //����������Ƭ�ŵ�����

        //���귶Χ
        float x_max;
        float x_min;
        float y_max;
        float y_min;
        float z_max;
        float z_min;
};
