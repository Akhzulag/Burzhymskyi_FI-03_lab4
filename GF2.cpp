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

std::string GF2::convert64bitToHex()
{
    char numHex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    std::ostringstream stream ;
    for(int i = 3 - size; i < 3 ; ++i)//треба перевірити чи нормально переводить бо я не докінця в певнений чи нормально буде при зміні порядку запису
        stream << std::hex << elementGF[i];

    hex = stream.str();

    std::transform(this->hex.begin(), this->hex.end(), this->hex.begin(), ::tolower);
    return hex;
}

GF2& GF2::operator += (const GF2 &right)
{
    this->size = std::max(right.size, this->size);

    for(int i = 0; i < this->size - 1; ++i)
    {
        this->elementGF[i] = this->elementGF[i] ^ right.elementGF[i];
    }

    for(int i = 3 - this->size; i < 3; ++i)
    {
        if(this->elementGF[i] == 0)
            --this->size;
        else
            break;
    }

    return *this;
}

u64 GF2::Trace()//needed check;
{
    int size = 3 - this->size;
    u64 mask = 1;
    u64 el = this->elementGF[size];
    u64 trace = 0;
    while (size >= 0)
    {
        trace = trace ^ (el & mask);

        el >>=1;
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
    for(int i = 3 - this->size; i < 3; ++i)
    {
        u64 tmp = this->elementGF[i];
        this->elementGF[i] = (tmp >> 1) | carry;
        carry = (tmp & 1)<<63;
    }

    this->elementGF[0] |= carry;
    for(int i = 0; i < 3; ++i)
    {
        if(this->elementGF[i] != 0)
            this->size = 3 - i;
        else
            break;
    }

}

void GF2::oneBitShiftLeft() const
{
    u64 carry = 0;
    for(int i = this->size - 1; i >= 0; --i)
    {
        u64 tmp = this->elementGF[i];
        this->elementGF[i] = (tmp << 1) | carry;
        carry = (tmp >> 63) & 1;
    }

    this->elementGF[this->size - 1] |= carry;

    for(int i = 0; i < 3; ++i)
    {
        if(this->elementGF[i] != 0)
            this->size = 3 - i;
        else
            break;
    }
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

GF2& operator * (const GF2& right, const GF2& left)
{
    //i must copy right and left and then shift copy of them
    u64 result[3] = {0,0,0};
    for(int j = 0; j < 179; ++j)
    {
        u64 resultJ[3] = {0,0,0};
        //multiply element with matrix
        for(int k = 0; k < 179; ++k)
        {
            u64 tmpk = (right.elementGF[Field::mulMatrix[0]] << Field::mulMatrix[1]) & (u64)1 ;
            for(int i = 1; i < 179; ++i)
            {
                if(i < 51)//51 because elementGF[0] has max 51 bit
                {
                    tmpk ^= ((right.elementGF[Field::mulMatrix[i*4]] >> Field::mulMatrix[i*4 + 1]) & (u64)1)
                            ^(((right.elementGF[Field::mulMatrix[i*4 + 2]] >> Field::mulMatrix[i*4 + 3]) & (u64)1));
                }
                else
                {
                    tmpk ^= ((right.elementGF[Field::mulMatrix[i*4]] >> Field::mulMatrix[i*4 + 1]) & (u64)1)
                            ^(((right.elementGF[Field::mulMatrix[i*4 + 2]] >> Field::mulMatrix[i*4 + 3]) & (u64)1));
                }
            }
            //Sum up all bites
            if(k<51)
                resultJ[0] |= tmpk << (50 - k);
            else
                resultJ[(k+13)/64] |= tmpk << ((64 - (k - 50)%64 )%64);
        }
        //result of multiplying * left element
        u64 vectorJ = (resultJ[0] & left.elementGF[0])^(resultJ[1] & left.elementGF[1])^(resultJ[2] & left.elementGF[2]);
        vectorJ = xorBites(vectorJ);

        //put bit on position in result element
        if(j<51)
            result[0] |= vectorJ << (50-j);
        else
            result[(j+13)/64] |= vectorJ << ((64-(j-50)%64)%64);

        right.oneBitShiftLeft();
        left.oneBitShiftLeft();
    }
    //right.oneBitShiftLeft();
    //left.oneBitShiftLeft();

}
