#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;

class D_K_Means
{
    private:

    public:
    bool Init();//初始化K个类的中心
    void Calc();//算法主程序
    void Writ();//把结果写入文件
}

bool D_K_Means::Init()
{

}

void D_K_Means::Calc()
{

}

void D_K_Means::Writ()
{

}

void Solve(D_K_Means kmeans)
{
    kmeans.Init();
    kmeans.Calc();
    kmeans.Writ();
}

int main()
{
    FILE *fp;
    char fname[20] = "data.txt";
    D_K_Means kmeans;

    if(kmeans.ReadData(fname) == false)
    {
        cout<<"Read Data Error!"<<endl;
        exit(0);
    }
    freopen("result.txt","w",stdout);

    Solve(kmeans);//算法主过程

    return 0;
}
