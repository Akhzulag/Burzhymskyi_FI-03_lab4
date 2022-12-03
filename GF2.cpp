//
// Created by Ростислав Буржимський on 18/11/2022.
//

#include "GF2.h"

GF2::~GF2()
{

}

void GF2::allocGF2()
{
    for(int i = 0; i < 3; ++i)
    {
        elementGF[i] = 0;
    }
}

GF2::GF2(std::string hex)
{
    this->size = ceil(double(hex.size())/16);

    this->allocGF2();


    std::reverse(hex.begin(),hex.end());
    std::transform(hex.begin(), hex.end(), hex.begin(), ::toupper);

    this->hex = hex;
    convertHexTo64Bit();
    convert64bitToHex();
}


u64 GF2::hexCharToNumber(char& hex)
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
void GF2::convertHexTo64Bit()
{
    for(int i = 2; i >= 3-size; --i)
        for(int k = 0; k < 16; ++k)
            elementGF[i] |= (hexCharToNumber(hex[16 * (2 - i) + k]) << k*4);

}

std::string GF2::convert64bitToHex() const
{
    char numHex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    std::ostringstream stream ;
    for(int i = 0; i < 3 ; ++i)
        stream << std::hex << elementGF[i];

    hex = stream.str();

    std::transform(this->hex.begin(), this->hex.end(), this->hex.begin(), ::tolower);
    return hex;
}

GF2& GF2::operator += (const GF2 &right)
{
    for(int i = 0; i < 2; ++i)
    {
        this->elementGF[i] = this->elementGF[i] ^ right.elementGF[i];
    }


    return *this;
}

u64 GF2::Trace()//needed check;
{
    int size = 2;
    u64 mask = 1;
    u64 el = this->elementGF[size];
    u64 trace = 0;
    while (size >= 0)
    {
        trace = trace ^ (el & mask);

        el >>= 1;
        if(el == 0)
        {
            --size;
            el = this->elementGF[size];
        }
    }
    return trace;
}

void GF2::power2() //needed test
{
   u64 carry = 0;
   for(int i = 0; i < 3; ++i)
   {
       u64 tmp = this->elementGF[i];
       this->elementGF[i] = (tmp >> 1) | (carry << 63);
       carry = (tmp & 1);
   }
   this->elementGF[0] |= carry << 50;

}

void GF2::oneBitShiftLeft() const
{
    u64 carry = 0;
    for(int i = 2; i >= 1; --i)
    {
        u64 tmp = this->elementGF[i];
        this->elementGF[i] = (tmp << 1) | carry;
        carry = (tmp >> 63) & 1;
    }
    u64 tmp = this->elementGF[0];

    if(tmp >> 50 == 1)
    {
        this->elementGF[0] = (((tmp << 1) ^ ((u64)1 << 51))) | carry;
        carry = (tmp >> 50) & 1;
    }else
    {
        this->elementGF[0] = (tmp << 1) | carry;
        carry = 0;
    }

    this->elementGF[2] |= carry;

}

u64 xorBites(u64 el)
{
    u64 a = 0;
    while(el != 0)
    {
        a ^= (el & 1);
        el >>= 1;
    }
    return a;
}

GF2::GF2(const u64* a)
{
    for(int i = 0; i < 3; ++i)
    {
        this->elementGF[i] = a[i];
    }
}



GF2& operator * (const GF2& right, const GF2& left)
{
    //i must copy right and left and then shift copy of them

    u64 result[3] = {0,0,0};
    for(int k = 0; k < 179; ++k)
    {
        u64 resultK[3] = {0,0,0};
        u64 tmpk = (right.elementGF[Field::mulMatrix[0]] >> Field::mulMatrix[1]) & (u64)1 ;
        std::cout<<((right.elementGF[Field::mulMatrix[0]] >> Field::mulMatrix[1]) & (u64)1);
        resultK[0] |= tmpk << 50;

        for(int i = 1; i < 179; ++i)
        {

            tmpk = (u64)(((right.elementGF[Field::mulMatrix[i*4]] >> Field::mulMatrix[i*4 + 1]) & (u64)1)
                    ^(((right.elementGF[Field::mulMatrix[i*4 + 2]] >> Field::mulMatrix[i*4 + 3]) & (u64)1)));

            std::cout<<tmpk;
            if(i < 51)
                resultK[0] |= tmpk << (50 - i);
            else
                resultK[(i+13)/64] |= tmpk << ((64 - (i - 50)%64 )%64);


        }
        std::cout<<'\n';
        std::cout<<std::hex<<resultK[0]<<' '<<resultK[1]<<' '<<resultK[2]<<'\n';
        std::cout<<"elementGF= "<<std::hex<<left.elementGF[0]<<' '<<left.elementGF[1]<<' '<<left.elementGF[2]<<'\n';
        //result of multiplying * left element
        u64 vectorJ = (resultK[0] & left.elementGF[0])^(resultK[1] & left.elementGF[1])^(resultK[2] & left.elementGF[2]);
        vectorJ = xorBites(vectorJ);
        std::cout<<vectorJ<<'\n';
        //put bit on position in result element
        if(k < 51)
            result[0] |= vectorJ << (50-k);
        else
            result[(k+13)/64] |= vectorJ << ((64-(k-50)%64)%64);

        right.oneBitShiftLeft();
        left.oneBitShiftLeft();
    }

    return *new GF2(result);
}
