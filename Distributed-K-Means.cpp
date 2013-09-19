#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

const double INF=1e20;
const int MAXD = 1000;//最高维度数
const int MAXN = 1000;//最大样本点数
const int MAXC = 50;//类的最大个数

struct Cluster//类
{
    double Center[MAXD];//类的中心
    int Number;//类中包含的样本point数目
    int Member[MAXN];//类中包含的样本point的index
};

class D_K_Means
{
    private:
    double Point[MAXN][MAXD];//第i个样本点的第j个属性
    Cluster Cluster[MAXC];//所有类
    int Cluster_Num;//类的个数
    int Point_Num;//样本数
    int Point_Dimension;//样本属性维度


    public:
    bool ReadData();//读取初始数据
    void Init();//初始化K类的中心
    void Mapper();
    void Calc();//算法主运算程序
    void TempWrit();//将一轮迭代结束后的结果写入临时文件
};

double D_K_Means::Distance(int p, int c)//编号为p的点与第c类的中心的距离
{
    double dis=0;
    for(int j=0;j<Point_Dimension;j++)
    {
        dis+=(Point[p][j]-Cluster[c].Center[j])*(Point[p][j]-Cluster[c].Center[j]);
    }
    return sqrt(dis);
}

bool D_K_Means::ReadData()//读取数据
{
    cin>>Point_Num;
    cin>>Point_Dimension;
    cin>>Cluster_Num;

    for(int i=0;i<Point_Num;i++)
    {
        for(int j=0;j<Point_Dimension;j++)
        {
            cin>>Point[i][j];//读取第i个样本点的第j个属性
        }
    }
    Init();//初始化K个类的中心
    TempWrit();//将所有类的中心作为第一轮迭代前的数据写入临时文件
}

void D_K_Means::Init()//初始化K个类的中心
{
    srand(time(NULL));//抛随机种子
    for(int i=0;i<Cluster_Num;i++)
    {
        int r=rand()%Point_Num;//随机选择所有样本点中的一个作为第i类的中心
        Cluster[i].Member[0]=r;
        for(int j=0;j<Point_Dimension;j++)
        {
            Cluster[i].Center[j]=Point[r][j];
        }
    }
}

void D_K_Means::Mapper()
{
    FILE *fp;
    if((fp=fopen("TempData.txt","r"))==NULL)
    {
        cout<<"Open TempData.txt Error!"<<endl;
        exit(0);
    }
    double temp;
    for(int i=0;i<Cluster_Num;i++)//读取中心
    {
        Cluster[i].Number=0;
        for(int j=0;j<Point_Dimension;j++)
        {
            cin>>temp;
            Cluster[i].Center[j]=temp;
        }
    }
    for(int i=0;i<Point_Num;i++)//重新计算所有类中的样本点
    {
        int index;
        double dis=INF;
        for(int j=0;j<Cluster_Num;j++)
        {
            if(Distance(i,j)<dis)
            {
                dis=Distance(i,j);
                index=j;
            }
        }
        Cluster[index].Member[Number++]=i;
    }
}

void D_K_Means::Combiner()
{

}


void D_K_Means::Reducer()
{

}

void D_K_Means::TempWrit()//将所有类的中心写入临时文件
{
    FILE *fp;
    if((fp=fopen("TempData.txt","w"))==NULL)
    {
        cout<<"Open TempData.txt Error!"<<endl;
        exit(0);
    }
    for(int i=0;i<Cluster_Num;i++)
    {
        for(int j=0;j<Point_Dimension;j++)
        {
            cout<<Cluster[i].Center[j];
            if(j!=Point_Dimension-1) cout<<" ";
            else cout<<endl;
        }
    }
    fclose(fp);
}

void Solve(D_K_Means kmeans)
{
    kmeans.ReadData();
    kmeans.Mapper();
    kmeans.Conbiner();
    kmeans.Reducer();
}

int main()
{

    D_K_Means kmeans;
    Solve(kmeans);//算法主体过程

    return 0;
}
