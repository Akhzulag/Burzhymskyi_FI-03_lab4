#include <iostream>
#include "GF2.h"
#include <fstream>

void matrix()
{
    int a = 1;
    int res[179];
    int mod =2*179+1;
    for(int i = 0; i < 179;++i)
    {
        res[i] = a;
        a = (a<<1)%(mod);
    }

    int one[179];
    int k;
    uint64_t tmp;
    uint64_t arr[179][3];

    std::cout<<"{ ";
    for(int i = 0; i < 179; ++i)
    {
        one[i] = 0;

        for(int j = 0; j < 179; ++j)
        {
            arr[i][j]=0;

            if((res[i]+res[j])%mod == 1 || (-res[i]+res[j])%mod == 1
            || (res[i]-res[j]+mod)%mod == 1 || ((-res[i]-res[j])+mod)%mod == 1)
            {
                if(j < 51)
                {
                    std::cout<<0<<", "<<50-j<<", ";
                    if(i == 0)
                        std::cout<<-1<<", "<<-1<<", ";
                    arr[i][0] |= (uint64_t)1<<(50-j);
                }
                else
                {
                    arr[i][(j+13)/64] |= (uint64_t)1 <<(64-(j-50)%64);
                    std::cout<<((j+13)/64)<<", "<<((64-(j-50)%64)%64)<<", ";
                }


                //std::cout<<1;
                one[i]++;
            }
            else
            {
                arr[i][j/64] |= 0;
               // std::cout<<0;
            }

        }
        //std::cout<<'\n';

        //for(int j = 0; j<=i;++j)
         //   std::cout<<"  ";
    }
    std::cout<<"}";
    std::cout<<'\n';
//    std::ofstream file("/Users/akhzulag/Desktop/KPI/3grade/СРОМ/Burzhymskyi_FI-03_lab4/out");
//    if(file)
//        for(int i = 0; i<179;++i)
//        {
//            file<<arr[i][0]<<' '<<arr[i][1]<<' '<<arr[i][2]<<'\n';
//        }
//    else
//        std::cout<<"file dont open";
//    file.close();
    std::cout<<'\n';
    for(int i = 0; i < 179; ++i)
    {
       // std::cout<<one[i]<<'\n';
    }

}


int main()
{

    while(true)
    {
        std::string a;
        std::cin>>a;
        GF2 b(a);
        //std::cout<<(b*b).convert64bitToHex()<<' ';
        b.power2();
        std::cout<<b.convert64bitToHex();
    }


    return 0;
}
