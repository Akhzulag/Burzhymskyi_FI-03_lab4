#include "GF2.h"

using namespace Field;

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


u64 GF2::hexCharToNumber(char& hex) const
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
            if(16 * (2 - i) + k < hex.size())
                elementGF[i] |= (hexCharToNumber(hex[16 * (2 - i) + k]) << k*4);

}

std::string GF2::convert64bitToHex() const
{
    char numHex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    std::ostringstream stream ;
    for(int i = 0; i < 3 ; ++i)
        for(int k = 15; k >=0; --k)
            stream << numHex[(elementGF[i] >> k*4) & 15];

    std::string tmpHex = stream.str();

    //std::reverse(tmpHex.begin(), tmpHex.end());
    hex = '0';
    for(int i=0; i<tmpHex.size(); ++i)
    {
        if(tmpHex[i] != '0')
        {
            hex = tmpHex.substr(i);
            break;
        }
    }

    std::transform(this->hex.begin(), this->hex.end(), this->hex.begin(), ::tolower);
    return hex;
}



GF2& GF2::operator += (const GF2 &right)
{
    for(int i = 0; i < 3; ++i)
    {
        this->elementGF[i] = this->elementGF[i] ^ right.elementGF[i];
    }

    return *this;
}

u64 GF2::Trace()
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

void GF2::power2()
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


GF2& operator * (const GF2& r, const GF2& l)
{

    GF2 right(r.elementGF);
    GF2 left(l.elementGF);
    u64 result[3] = {0,0,0};
    for(int k = 0; k < 179; ++k)
    {

        u64 resultK[3] = {0,0,0};
        u64 tmpk = (right.elementGF[Field::mulMatrix[0]] >> Field::mulMatrix[1]) & (u64)1 ;
        resultK[0] |= tmpk << 50;

        for(int i = 1; i < 179; ++i)
        {

            tmpk = (u64)(((right.elementGF[Field::mulMatrix[i*4]] >> Field::mulMatrix[i*4 + 1]) & (u64)1)
                    ^(((right.elementGF[Field::mulMatrix[i*4 + 2]] >> Field::mulMatrix[i*4 + 3]) & (u64)1)));

            if(i < 51)
                resultK[0] |= tmpk << (50 - i);
            else
                resultK[(i+13)/64] |= tmpk << ((64 - (i - 50)%64 )%64);


        }
        u64 vectorJ = (resultK[0] & left.elementGF[0])^(resultK[1] & left.elementGF[1])^(resultK[2] & left.elementGF[2]);
        vectorJ = xorBites(vectorJ);

        if(k < 51)
            result[0] |= vectorJ << (50-k);
        else
            result[(k+13)/64] |= vectorJ << ((64-(k-50)%64)%64);

        right.oneBitShiftLeft();
        left.oneBitShiftLeft();
    }

    return *new GF2(result);
}

void GF2::power2k(int k)
{
    u64 carry = 0;
    u64 mask = 0xFFFFFFFFFFFFFFFF;
    for(int i = 0; i < 3; ++i)
    {
        u64 tmp = this->elementGF[i];
        this->elementGF[i] = (tmp >> k) | (carry << (64 - k));
        carry = (tmp & (mask >> (64-k)));
    }
    this->elementGF[0] |= (carry << (51-k));
}
GF2& GF2::power2k(const GF2&, int k)
{
    u64 carry = 0;
    u64 mask = 0xFFFFFFFFFFFFFFFF;
    u64 res[3] = {0,0,0};
    for(int i = 0; i < 3; ++i)
    {
        res[i] = (this->elementGF[i]>>k) | (carry << (64 - k));
        carry = (this->elementGF[i] & (mask >> (64-k)));
    }
    res[0] |= (carry << (51-k));
    return *new GF2(res);
}

GF2& GF2::inverseGF()
{

    GF2 b(this->elementGF);
    int k = 1;
    uint8_t m = 178;// (1011 0010)2
    for(int i = 6; i >= 0; --i)
    {
       GF2 tmp(b.elementGF);
       for(int j = 0; j < k; ++j)
           tmp.power2();
       b = tmp * b;
       k *= 2;

       if(((m>>i) & 1) == 1)
       {
           b.power2();
           GF2 tmp = b;
           b = b * *this;
           ++k;
       }
    }

    b.power2();
    return *new GF2(b.elementGF);
}

GF2& operator ^ (const GF2& left, std::string right)
{
    int sizeBinary = 0;
    bool firstOne = false;
    int binary[right.size()*4];
    int k = 0;
    for(int i = 0; i < right.size(); ++i)
    {
        for(int w = 0; w < 4; ++w)
        {
            if(((left.hexCharToNumber(right[i]) >> (3-w)) & 1))
                firstOne = true;

            if(firstOne == true)
                sizeBinary++;

            binary[i*4 + w] = ((left.hexCharToNumber(right[i])>>(3-w)) & 1);
        }
    }
    int begin = right.size()*4 - sizeBinary;
    int end = right.size()*4;

    GF2 *C = new GF2(one.elementGF);
    for(int i = begin; i < end; ++i)
    {
        if(binary[i] == 1)
        {
            GF2 tmp = *C;
            *C = tmp * left;
        }
        if(i != end-1)
        {
            C->power2();
        }
    }

    return *C;
}

bool operator == (const GF2 &left, const GF2 &right)
{
    if(&left == &right)
        return true;
    for(int i = 0; i < 3; ++i)
        if(left.elementGF[i] != right.elementGF[i])
            return false;
    return true;
}

int gcd(int a, int b)
{
    if(b == 0)
        return a;
    return gcd(b,a%b);
}

bool isPrime(int a)
{
    for(int i = 2; i <= sqrt(a); ++i)
    {
        if(a % i == 0)
            return 0;
    }
    return 1;
}

void GF2::ONB(const int& m,int T)
{
    if(T > 2 || m%8 == 0)
    {
        return;
    }
    int p = T*m + 1;
    if(isPrime(T*m + 1))
    {
        int k = 1;
        int tmp = 2;
        while(tmp != 1)
        {
            tmp = (tmp*2)%p;
            ++k;
        }
        if(gcd((T*m)/k,m) == 1)
            std::cout<<"m = "<<m<<";\nexist ONB type "<<T<<'\n';
    }
    ONB(m,T+1);


}