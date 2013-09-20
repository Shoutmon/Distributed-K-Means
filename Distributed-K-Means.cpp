#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <algorithm>

using namespace std;

const double INF=1e20;
const int MAXD = 1000;//最高维度数
const int MAXN = 1000;//最大样本点数
const int MAXC = 50;//类的最大个数
const int NUM_THREADS = 4;//线程数

struct aCluster//类
{
    double Center[MAXD];//类的中心
    int Number;//类中包含的样本point数目
    int Member[MAXN];//类中包含的样本point的index
};

class D_K_Means
{
    private:
    double Point[MAXN][MAXD];//第i个样本点的第j个属性
    aCluster Cluster[MAXC];//所有类
    int Cluster_Num;//类的个数
    int Point_Num;//样本数
    int Point_Dimension;//样本属性维度
    aCluster TempCluster[MAXC];//临时存放类的中心
    double Distance(int,int);

    public:
    bool ReadData();//读取初始数据
    void Init();//初始化K类的中心
    void Mapper();
    void Combiner();
    void Reducer();
    bool TempWrit();//将一轮迭代结束后的结果写入临时文件
    void Write_Result();//输出结果
};

double D_K_Means::Distance(int p,int c)//编号为p的点与第c类的中心的距离
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
    FILE *fp;
    if((fp=fopen("Data.txt","r"))==NULL)
    {
        cout<<"Open Data.txt Error!"<<endl;
        exit(0);
    }
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
    fclose(fp);
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
            TempCluster[i].Center[j]=Point[r][j];
        }
    }
}

void D_K_Means::Mapper()//求解每个类下的样本点
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
        Cluster[index].Member[Cluster[index].Number++]=i;
    }
    fclose(fp);
}

void D_K_Means::Combiner()//用多线程处理进入Reducer前的数据，使进入Reducer的数据量减少
{
    int id;
    for(int i=0;i<Cluster_Num;i++)
    {
        memset(TempCluster[i].Center,0,sizeof(TempCluster[i].Center));
    }
    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel private(i)
    {
//        id=omp_get_thread_num();
//        #pragma omp for
        for(int i;i<Cluster_Num;i++)
        {
            for(int j=0;j<Cluster[i].Number;j++)
            {
                for(int k=0;k<Point_Dimension;k++)
                {
                    TempCluster[i].Center[j] += Point[Cluster[i].Member[j]][k];
                }
            }
        }
    }
}


void D_K_Means::Reducer()
{
    for(int i=0;i<Cluster_Num;i++)
    {
        for(int j=0;j<Point_Dimension;j++)
        {
            TempCluster[i].Center[j]/=Cluster[i].Number;
        }
    }
}

bool D_K_Means::TempWrit()//将所有类的中心写入临时文件
{
    double ERR=0.0;
    FILE *fp;
    if((fp=fopen("TempData.txt","w"))==NULL)
    {
        cout<<"Open TempData.txt Error!"<<endl;
        exit(0);
    }
    for(int i=0;i<Cluster_Num;i++)//将TempCluster的中心坐标复制到Cluster中，同时计算与上一次迭代的变化（取2范数的平方）
    {
        for(int j=0;j<Point_Dimension;j++)
        {
            ERR+=(TempCluster[i].Center[j]-Cluster[i].Center[j])*(TempCluster[i].Center[j]-Cluster[i].Center[j]);
            Cluster[i].Center[j]=TempCluster[i].Center[j];
        }
    }
    for(int i=0;i<Cluster_Num;i++)//将Cluster的中心坐标写入临时文件
    {
        for(int j=0;j<Point_Dimension;j++)
        {
            cout<<Cluster[i].Center[j];
            if(j!=Point_Dimension-1) cout<<" ";
            else cout<<endl;
        }
    }
    fclose(fp);
    if(ERR<0.1) return true;
    else return false;
}

void D_K_Means::Write_Result()//输出结果
{
    FILE *fp;
    if((fp=fopen("Result.txt","w"))==false)
    {
        cout<<"Open Result.txt Error!"<<endl;
        exit(0);
    }
    for(int i=0;i<Cluster_Num;i++)
    {
        cout<<"Cluster "<<i<<" : ";
        sort(Cluster[i].Member,(Cluster[i].Member)+Cluster[i].Number);//类内序号排序，方便输出
        for(int j=0;j<Cluster[i].Number;j++)
        {
            cout<<Cluster[i].Member[j];
            if(j!=Cluster[i].Number-1) cout<<" ";
            else cout<<endl;
        }
    }
    fclose(fp);
}

void Solve(D_K_Means kmeans)
{
    bool converged=false;
    int times=1;
    kmeans.ReadData();
    while(converged==false)
    {
        kmeans.Mapper();
        kmeans.Combiner();
        kmeans.Reducer();
        converged=kmeans.TempWrit();//返回true时结束循环
    }
    kmeans.Write_Result();//把结果写入文件
}


int main()
{
    D_K_Means kmeans;
    Solve(kmeans);//算法主体过程
    return 0;
}
