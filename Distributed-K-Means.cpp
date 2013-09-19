#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

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
    bool ReadData();//读取数据
    void Init();//初始化K类的中心
    void Calc();//算法主运算程序
    void Writ();//结果写入文件
};

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

void D_K_Means::Calc()
{

}

void D_K_Means::Writ()
{

}

void Solve(D_K_Means kmeans)
{
    kmeans.ReadData();
    kmeans.Init();
    kmeans.Calc();
    kmeans.Writ();
}

int main()
{
    if(freopen("data.txt","r",stdin)==false)
    {
        cout<<"Open data.txt Error!"<<endl;
        exit(0);
    }
    freopen("result.txt","w",stdout);

    D_K_Means kmeans;
    Solve(kmeans);//算法主体过程

    fclose(stdin);
    fclose(stdout);
    return 0;
}
