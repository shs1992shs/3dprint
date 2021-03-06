#include "Voxelization.h"
using namespace std;

void Voxelization::GetExtremum(float x_max,float x_min,float y_max,float y_min,float z_max,float z_min)
{
    xmax=x_max;
    xmin=x_min;
    ymax=y_max;
    ymin=y_min;
    zmax=z_max;
    zmin=z_min;
}

void Voxelization::GetSurfacePointNum(void)
{
    unsigned int num;
    for(unsigned int i=0;i<(1<<(max_height-1));i++)
    {
        num = Octree::TraverseZ(i);
        surface_point_num.push_back(i);
        model_impact_factor.push_back(Octree::layer_impactfactor);
        /*
        cout<<num<<" voxels on the model in layer "<<i<<endl;
        cout<<"the impact factor is "<<Octree::layer_impactfactor<<endl;
        cout<<"the calculate factor is "<<1.0*Octree::layer_impactfactor/num<<endl;
        */
    }
}

void Voxelization::SurfacePoint(unsigned int x, unsigned int y, unsigned int z)
{
    x=CheckNum(x);
    y=CheckNum(y);
    z=CheckNum(z);
    OctreeNode* node = root;
    unsigned int nummax=1<<(max_height-1);
    if(x>nummax||y>nummax||z>nummax)
    {
        cout<<"the num is bigger than the max"<<endl;
        return;
    }
    int x_direction; 
    int y_direction; 
    int z_direction; 
    int position;
    for(int j=0;j<max_height-1;j++)
    {
        //use bit manipulation to change to binary
        x_direction = x>>(max_height-2-j)&1;
        y_direction = y>>(max_height-2-j)&1;
        z_direction = z>>(max_height-2-j)&1;
        position=x_direction*4+y_direction*2+z_direction;
        //            cout<<position<<endl;
        switch(position)
        {
            case 0:
                node=node->Back_Left_Bottom;
                break;
            case 1:
                node=node->Back_Left_Top;
                break;
            case 2:
                node=node->Back_Right_Bottom;
                break;
            case 3:
                node=node->Back_Right_Top;
                break;
            case 4:
                node=node->Front_Left_Bottom;
                break;
            case 5:
                node=node->Front_Left_Top;
                break;
            case 6:
                node=node->Front_Right_Bottom;
                break;
            case 7:
                node=node->Front_Right_Top;
                break;
            default:
                cerr<<"error"<<endl;
                break;
        }
    }
    node->flag=0;
    //            cout<<node->orderstr<<endl;
}

unsigned int Voxelization::ChangeCoordinate(float coordinate,float max,float min)
{
    int nummax=1<<(max_height-1);
    unsigned int data = (coordinate-min)*nummax/(max-min);
    //    cout<<data<<endl;
    return data;
}

unsigned int Voxelization::CheckNum(unsigned int num)
{
    if(num==(1<<(max_height-1)))
        num--;
    return num;
}

void Voxelization::PointToVoxel(vector<CVertex> VectorPoint)
{
    for(int i=0;i<int(VectorPoint.size());i++)
    {
        unsigned int voxelx = ChangeCoordinate(VectorPoint[i].m_Point[0],xmax,xmin);
        unsigned int voxely = ChangeCoordinate(VectorPoint[i].m_Point[1],ymax,ymin);
        unsigned int voxelz = ChangeCoordinate(VectorPoint[i].m_Point[2],zmax,zmin);
        //        cout<<binaryx<<binaryy<<binaryz<<endl;
        SurfacePoint(voxelx,voxely,voxelz);
    }
}

void Voxelization::EdgeToVoxel(vector<CEdge> VectorEdge, vector<CVertex> VectorPoint)
{
    for(int i=0;i<int(VectorEdge.size());i++)
    {
        //change the coordinate system
        unsigned int strx1 = ChangeCoordinate(VectorPoint[VectorEdge[i].PointIndex[0]].m_Point[0],xmax,xmin);
        unsigned int strx2 = ChangeCoordinate(VectorPoint[VectorEdge[i].PointIndex[1]].m_Point[0],xmax,xmin);
        unsigned int stry1 = ChangeCoordinate(VectorPoint[VectorEdge[i].PointIndex[0]].m_Point[1],ymax,ymin);
        unsigned int stry2 = ChangeCoordinate(VectorPoint[VectorEdge[i].PointIndex[1]].m_Point[1],ymax,ymin);
        unsigned int strz1 = ChangeCoordinate(VectorPoint[VectorEdge[i].PointIndex[0]].m_Point[2],zmax,zmin);
        unsigned int strz2 = ChangeCoordinate(VectorPoint[VectorEdge[i].PointIndex[1]].m_Point[2],zmax,zmin);
        OctreePoint point1(strx1,stry1,strz1);
        OctreePoint point2(strx2,stry2,strz2);
        EdgeChange_Bresenham(point1,point2);
    }
}

void Voxelization::FacetToVoxel(vector<CFacet> VectorFacet, vector<CVertex> VectorPoint)
{
    for(int i=0;i<int(VectorFacet.size());i++)
    {
        //change the coordinate system
        unsigned int strx1 = ChangeCoordinate(VectorPoint[VectorFacet[i].PointIndex[0]].m_Point[0],xmax,xmin);
        unsigned int strx2 = ChangeCoordinate(VectorPoint[VectorFacet[i].PointIndex[1]].m_Point[0],xmax,xmin);
        unsigned int strx3 = ChangeCoordinate(VectorPoint[VectorFacet[i].PointIndex[2]].m_Point[0],xmax,xmin);
        unsigned int stry1 = ChangeCoordinate(VectorPoint[VectorFacet[i].PointIndex[0]].m_Point[1],ymax,ymin);
        unsigned int stry2 = ChangeCoordinate(VectorPoint[VectorFacet[i].PointIndex[1]].m_Point[1],ymax,ymin);
        unsigned int stry3 = ChangeCoordinate(VectorPoint[VectorFacet[i].PointIndex[2]].m_Point[1],ymax,ymin);
        unsigned int strz1 = ChangeCoordinate(VectorPoint[VectorFacet[i].PointIndex[0]].m_Point[2],zmax,zmin);
        unsigned int strz2 = ChangeCoordinate(VectorPoint[VectorFacet[i].PointIndex[1]].m_Point[2],zmax,zmin);
        unsigned int strz3 = ChangeCoordinate(VectorPoint[VectorFacet[i].PointIndex[2]].m_Point[2],zmax,zmin);
        OctreePoint point1(strx1,stry1,strz1);
        OctreePoint point2(strx2,stry2,strz2);
        OctreePoint point3(strx3,stry3,strz3);
        if((point1==point2)&&(point1==point3))
            continue;
        else if(point1==point2)
        {
            EdgeChange_Bresenham(point1,point3);
            continue;
        }
        else if(point1==point3)
        {
            EdgeChange_Bresenham(point2,point3);
            continue;
        }
        else if(point3==point2)
        {
            EdgeChange_Bresenham(point1,point3);
            continue;
        }
//        FacetChange_FloodFill(point1,point2,point3);
        FacetChange_Scanning(point1,point2,point3);
    }
}

void Voxelization::Bresenham2D(Point2D pt2d1, Point2D pt2d2)
{
    opoint2D.insert(pt2d1);
    opoint2D.insert(pt2d2);
    int diffx = pt2d1.x-pt2d2.x;
    int diffy = pt2d1.y-pt2d2.y;
    int sign_x = diffx>0?1:-1;
    int sign_y = diffy>0?1:-1;
    Point2D str2d(pt2d2.x,pt2d2.y);
    int eps = 0; //eps is cumulative error
    diffx = abs(diffx);
    diffy = abs(diffy);//can use sign to judge positive and negative
    if(diffx>=diffy)
    {
        for(;str2d.x!=pt2d1.x-sign_x;)
        {
            str2d.x+=sign_x;
            eps+=diffy;
            if((eps<<1)>=diffx)
            {
                str2d.y+=sign_y;
                eps-=diffx;
            }
            opoint2D.insert(str2d);
        }
    }
    else
    {
        for(;str2d.y!=pt2d1.y-sign_y;)
        {
            str2d.y+=sign_y;
            eps+=diffx;
            if((eps<<1)>=diffy)
            {
                str2d.x+=sign_x;
                eps-=diffy;
            }
            opoint2D.insert(str2d);
        }
    }
}

void Voxelization::SuperCoverLine2D(Point2D pt2d1, Point2D pt2d2)
{
    opoint2D.insert(pt2d1);
    opoint2D.insert(pt2d2);
    int diffx = pt2d1.x-pt2d2.x;
    int diffy = pt2d1.y-pt2d2.y;
    if(diffx==0&&diffy==0)
        return;
    int sign_x = diffx>0?1:-1;
    if(diffx==0)
        sign_x=0;
    int sign_y = diffy>0?1:-1;
    if(diffy==0)
        sign_y=0;
    Point2D str2d(pt2d2.x,pt2d2.y);
    int eps = 0; //eps is cumulative error
    int preeps = 0; //preeps is the previous value of the error variable
    diffx = abs(diffx);
    diffy = abs(diffy);//can use sign to judge positive and negative
    int ddx = (diffx<<1);
    int ddy = (diffy<<1);
    try
    {
        if(diffx>=diffy)
        {
            preeps=eps=diffx;
            for(;str2d.x!=pt2d1.x-sign_x;)
            {
                if(int(str2d.x)==-1||int(str2d.y)==-1) throw str2d.x;
                str2d.x+=sign_x;
                eps+=ddy;
                if(eps>ddx)
                {
                    eps-=ddx;
                    if(eps+preeps<ddx)
                    {
                        opoint2D.insert(str2d);
                        str2d.y+=sign_y;
                    }
                    else if(eps+preeps>ddx)
                    {
                        str2d.y+=sign_y;
                        str2d.x-=sign_x;
                        opoint2D.insert(str2d);
                        str2d.x+=sign_x;
                    }
                    else
                    {
                        //   opoint2D.insert(str2d);
                        str2d.y+=sign_y;
                        str2d.x-=sign_x;
                        //   opoint2D.insert(str2d);
                        str2d.x+=sign_x;
                    }
                }
                opoint2D.insert(str2d);
                preeps = eps;
            }
        }
        else
        {
            preeps=eps=diffy;
            for(;str2d.y!=pt2d1.y-sign_y;)
            {
                if(int(str2d.x)==-1||int(str2d.y)==-1) throw str2d.x;
                str2d.y+=sign_y;
                eps+=ddx;
                if(eps>ddy)
                {
                    eps-=ddy;
                    if(eps+preeps<ddy)
                    {
                        opoint2D.insert(str2d);
                        str2d.x+=sign_x;
                    }
                    else if(eps+preeps>ddx)
                    {
                        str2d.x+=sign_x;
                        str2d.y-=sign_y;
                        opoint2D.insert(str2d);
                        str2d.y+=sign_y;
                    }
                    else
                    {
                        //    opoint2D.insert(str2d);
                        str2d.x+=sign_x;
                        str2d.y-=sign_y;
                        //    opoint2D.insert(str2d);
                        str2d.y+=sign_y;
                    }
                }
                opoint2D.insert(str2d);
                preeps = eps;
            }
        }
    }
    catch(unsigned int)
    {
        cout<<"str2d.x="<<str2d.x<<",str2d.y="<<str2d.y<<endl;
        cout<<"pt2d1.x="<<pt2d1.x<<",pt2d1.y="<<pt2d1.y<<endl;
        cout<<"pt2d2.x="<<pt2d2.x<<",pt2d2.y="<<pt2d2.y<<endl;
        cout<<"exsit -1"<<endl;
    }
}

float Voxelization::Xmax(void)
{
    return xmax;
}

float Voxelization::Xmin(void)
{
    return xmin;
}

float Voxelization::Ymax(void)
{
    return ymax;
}

float Voxelization::Ymin(void)
{
    return ymin;
}

float Voxelization::Zmax(void)
{
    return zmax;
}

float Voxelization::Zmin(void)
{
    return zmin;
}

void Voxelization::setperspective(double eyex,double eyey,double eyez,double centrex,double centrey,double centrez,double upx,double upy,double upz)
{
    float maxlength = max(max(xmax-xmin,ymax-ymin),zmax-zmin);
    gluLookAt( 2 * eyex * maxlength, 2 * eyey * maxlength, eyez * maxlength, centrex, centrey, centrez, upx, upy, upz);
}
