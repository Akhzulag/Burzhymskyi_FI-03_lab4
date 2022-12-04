#ifndef BURZHYMSKYI_FI_03_LAB4_GF2_H
#define BURZHYMSKYI_FI_03_LAB4_GF2_H
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>


#define u64 u_int64_t

namespace Field
{
    const int mulMatrix[] = { 0, 49, -1, -1, 0,50, 1, 45, 0, 12, 1, 45, 1, 53, 2, 40, 0, 0, 1, 7, 1, 31, 2, 53, 0, 37, 0, 30, 2, 11, 2, 5, 2, 58, 2, 21, 1, 21, 2, 60, 0, 21, 2, 22, 0, 28, 2, 10, 0, 31, 0, 17, 0, 44, 2, 52, 2, 17, 2, 5, 1, 36, 2, 55, 0, 7, 1, 16, 1, 23, 2, 38, 0, 18, 2, 33, 0, 38, 2, 9, 0, 44, 1, 30, 1, 0, 2, 11, 0, 39, 0, 20, 0, 17, 2, 36, 1, 62, 1, 14, 1, 27, 1, 18, 1, 33, 2, 46, 1, 39, 2, 15, 1, 51, 2, 23, 0, 40, 1, 20, 0, 28, 2, 63, 2, 36, 2, 34, 0, 32, 2, 37, 0, 38, 0, 27, 1, 62, 2, 52, 1, 48, 2, 27, 2, 42, 2, 1, 1, 54, 1, 46, 0, 48, 1, 44, 1, 58, 2, 40, 1, 24, 2, 48, 0, 6, 1, 34, 1, 17, 1, 12, 0, 34, 1, 35, 0, 9, 1, 23, 1, 12, 2, 61, 2, 57, 2, 3, 1, 41, 2, 43, 2, 50, 2, 26, 1, 25, 1, 8, 0, 46, 2, 39, 1, 15, 2, 53, 0, 26, 0, 16, 1, 48, 1, 27, 1, 55, 1, 9, 2, 41, 2, 25, 0, 11, 1, 43, 2, 48, 2, 44, 1, 10, 2, 2, 1, 60, 1, 47, 0, 13, 2, 41, 0, 47, 1, 44, 1, 7, 2, 24, 0, 22, 2, 14, 1, 20, 2, 19, 1, 28, 2, 28, 0, 15, 1, 61, 1, 55, 2, 1, 0, 13, 2, 0, 0, 49, 0, 48, 0, 12, 1, 53, 1, 58, 2, 24, 1, 38, 2, 44, 0, 3, 2, 56, 2, 50, 2, 16, 0, 23, 2, 45, 1, 42, 2, 23, 2, 59, 2, 56, 0, 35, 2, 4, 0, 7, 1, 11, 0, 9, 2, 47, 0, 24, 1, 17, 2, 54, 2, 15, 0, 45, 1, 6, 0, 30, 2, 8, 1, 0, 2, 29, 1, 49, 1, 19, 0, 25, 1, 61, 1, 9, 2, 46, 0, 1, 2, 49, 0, 10, 2, 39, 0, 33, 0, 6, 2, 61, 2, 33, 0, 41, 2, 20, 0, 21, 1, 50, 1, 28, 2, 63, 0, 25, 1, 13, 0, 8, 1, 33, 0, 34, 2, 54, 1, 63, 2, 38, 0, 26, 2, 35, 1, 18, 2, 62, 0, 8, 0, 5, 1, 35, 2, 3, 1, 56, 2, 47, 1, 60, 1, 26, 0, 1, 2, 25, 0, 46, 1, 52, 1, 31, 2, 14, 2, 31, 2, 8, 1, 3, 1, 2, 1, 4, 2, 30, 1, 4, 2, 7, 2, 30, 2, 12, 0, 29, 1, 29, 0, 20, 1, 19, 1, 13, 2, 34, 0, 5, 1, 22, 0, 41, 2, 57, 1, 37, 2, 22, 0, 42, 2, 4, 0, 4, 2, 60, 1, 41, 1, 37, 0, 35, 2, 16, 1, 32, 1, 16, 0, 45, 1, 63, 0, 37, 0, 16, 2, 27, 2, 17, 0, 2, 1, 40, 1, 25, 2, 45, 0, 10, 1, 57, 1, 34, 1, 10, 0, 24, 1, 26, 1, 39, 2, 49, 1, 57, 1, 42, 0, 3, 2, 2, 0, 14, 2, 26, 1, 59, 1, 54, 0, 47, 0, 11, 0, 0, 1, 24, 0, 33, 1, 15, 0, 18, 2, 35, 0, 27, 0, 19, 1, 14, 2, 37, 0, 19, 2, 62, 0, 32, 1, 22, 2, 20, 2, 9, 1, 5, 2, 13, 1, 3, 1, 1, 1, 29, 2, 7, 1, 49, 2, 18, 0, 15, 2, 51, 0, 2, 2, 42, 1, 59, 1, 8, 1, 52, 1, 43, 0, 22, 1, 38, 0, 40, 2, 59, 0, 42, 2, 10, 1, 21, 2, 32, 1, 50, 2, 13, 2, 28, 2, 6, 0, 36, 2, 51, 1, 40, 2, 55, 0, 23, 1, 32, 1, 51, 1, 6, 2, 31, 2, 19, 1, 1, 2, 6, 0, 43, 0, 29, 0, 39, 2, 21, 0, 31, 2, 32, 1, 30, 1, 5, 1, 2, 2, 29, 2, 18, 2, 12, 0, 43, 0, 36, 1, 36, 2, 58, 0, 4, 1, 11, 1, 56, 2, 43, 0, 14, 1, 47, 1, 46, 2, 0 };

}

class GF2
{
    mutable std::string hex;

    mutable int size = 0;
    int bitSizeGen = 179;
public:
    mutable u64 elementGF[3]{};
    explicit GF2(std::string);
    GF2(const u64 *a);
    ~GF2();

    void allocGF2();

    u64 hexCharToNumber(char&);
    void convertHexTo64Bit();
    std::string convert64bitToHex() const;

    friend GF2& operator * (const GF2 &, const GF2 &);
    friend GF2& operator ^ (const GF2 &, const GF2 &);
    friend GF2& operator + (const GF2 &, const GF2 &);

    //GF2& cycleShiftLeft();
    //GF2& cycleShiftRight();
    void oneBitShiftLeft() const;
    GF2& cycleShiftRight();

    void checkResize();

    GF2& power2(const GF2&);
    void power2();
    void power2k(int k);
    GF2& power2k(const GF2&, int k);
    u64 Trace();
    void buildMulmatrix(std::pair<int,int >);
    GF2& inverseGF();
    GF2& operator += (const GF2 &);

    GF2& operator *= (GF2 &);

};


#endif //BURZHYMSKYI_FI_03_LAB4_GF2_H
