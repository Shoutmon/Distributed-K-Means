#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;

class D_K_Means
{
    private:

    public:
    bool Init();//��ʼ��K���������
    void Calc();//�㷨������
    void Writ();//�ѽ��д���ļ�
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

    Solve(kmeans);//�㷨������

    return 0;
}
