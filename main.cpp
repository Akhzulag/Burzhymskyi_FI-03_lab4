#include <iostream>
#include "GF2.h"
#include <fstream>

void matrix()
{
    int a = 1;
    int m = 179;
    int res[m];
    int mod =2*m+1;
    for(int i = 0; i < m;++i)
    {
        res[i] = a;
        a = (a<<1)%(mod);
    }

    int one[m];
    int k;
    uint64_t tmp;
    uint64_t arr[m][3];

    std::cout<<"{ ";
    for(int i = 0; i < m; ++i)
    {
        one[i] = 0;
        for(int j = 0; j < m; ++j)
        {
            arr[i][j]=0;

            if((res[i]+res[j])%mod == 1 || (-res[i]+res[j])%mod == 1
            || (res[i]-res[j]+mod)%mod == 1 || ((-res[i]-res[j])+mod)%mod == 1)
            {
                if(j < 51)
                {
                    std::cout<<0<<", "<<50-j<<", ";
                    if(i == 0)
                       // std::cout<<-1<<", "<<-1<<", ";
                    arr[i][0] |= (uint64_t)1<<(50-j);
                }
                else
                {
                    arr[i][(j+13)/64] |= (uint64_t)1 <<(64-(j-50)%64);
                    std::cout<<((j+13)/64)<<", "<<((64-(j-50)%64)%64)<<", ";
                }


                std::cout<<1<<' ';
                one[i]++;
            }
            else
            {
                arr[i][j/64] |= 0;
                std::cout<<0<<' ';
            }

        }
        std::cout<<'\n';
    }
    std::cout<<"}";
    std::cout<<'\n';

}

u64 hexCharToNumber(char& hex)
{
    switch (hex)
    {
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
        default:
            if(hex > '0' && hex <= '9')
                return hex - '0';
            return 0;
    }
}

int main()
{
//    std::bitset<179> a = 0x4000000000009;
//    std::bitset<179> b = 0x8000000000000009;
//    std::bitset<179> c = 0x8000000000000009;
//    std::bitset<179> d =(a<<128)|(b<<64)|c;
//    std::cout<<d;

   // matrix();
    //return 0;
    //0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    //while(true)
//    {
//        int h;
//        std::string right,d;
//        std::cin>>right;
//        int sizeBinary = 0;
//        bool firstOne = false;
//        int binary[right.size()*4];
//        int k = 0;
//        for(int i = 0; i < right.size(); ++i)
//        {
//            for(int w = 0; w < 4; ++w)
//            {
//                if(((hexCharToNumber(right[i]) >> (3-w)) & 1))
//                    firstOne = true;
//
//                if(firstOne == true)
//                    sizeBinary++;
//
//                binary[i*4 + w] = ((hexCharToNumber(right[i])>>(3-w)) & 1);
//            }
//        }
//        int begin = right.size()*4 - sizeBinary;
//        int end = right.size()*4;
//        for(int i = end-1; i >=begin; --i)
//            std::cout<<binary[i];
//        std::cout<<'\n'<<sizeBinary;
//       // std::cout<<log2(0);
//        GF2 b(a),c(a);
//        //b.oneBitShiftLeft();
//        //std::cout<<b.convert64bitToHex();
//        //std::cout<<(b*b).convert64bitToHex()<<'\n';
//
//        b = b.inverseGF();
//        GF2 tmp = b * c;
//            //std::cout<<std::dec<<i<<' ';
//            std::cout<<std::hex<<tmp.elementGF[0]<<' '<<std::hex<<tmp.elementGF[1]<<' '<<std::hex<<tmp.elementGF[2]<<'\n';
//            std::cout<<std::hex<<b.elementGF[0]<<' '<<std::hex<<b.elementGF[1]<<' '<<std::hex<<b.elementGF[2]<<'\n';
//
////
//        //std::cout<<b.convert64bitToHex();
    //}
    std::string a,c="5";
    std::cin>>a>>c;
    GF2 b(a),f(c),f1(a);
    b += f;
    std::cout<<std::hex<<b.elementGF[0]<<' '<<std::hex<<b.elementGF[1]<<' '<<std::hex<<b.elementGF[2]<<'\n';

    for(int i = 0; i < 4; ++i)
    {
        f = f * f1;
    }
    std::cout<<std::hex<<f.elementGF[0]<<' '<<std::hex<<f.elementGF[1]<<' '<<std::hex<<f.elementGF[2]<<'\n';

    return 0;
}
