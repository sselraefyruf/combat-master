#pragma once
#include "def.h"
#include <math.h>
#include <vector>

inline uintptr_t SignatureScan(const char* module, const char* pattern)
{
    uintptr_t moduleAdress = 0;
    moduleAdress = (uintptr_t)GetModuleHandleA(module);

    static auto patternToByte = [](const char* pattern)
    {
        auto       bytes = std::vector<int>{};
        const auto start = const_cast<char*>(pattern);
        const auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current)
        {
            if (*current == '?')
            {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else { bytes.push_back(strtoul(current, &current, 16)); }
        }
        return bytes;
    };

    const auto dosHeader = (PIMAGE_DOS_HEADER)moduleAdress;
    const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)moduleAdress + dosHeader->e_lfanew);

    const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto       patternBytes = patternToByte(pattern);
    const auto scanBytes = reinterpret_cast<std::uint8_t*>(moduleAdress);

    const auto s = patternBytes.size();
    const auto d = patternBytes.data();

    for (auto i = 0ul; i < sizeOfImage - s; ++i)
    {
        bool found = true;
        for (auto j = 0ul; j < s; ++j)
        {
            if (scanBytes[i + j] != d[j] && d[j] != -1)
            {
                found = false;
                break;
            }
        }
        if (found) { return reinterpret_cast<uintptr_t>(&scanBytes[i]); }
    }
    return NULL;
}

inline uintptr_t GetAbsoluteAddress(uintptr_t address, int offset, int size) {
    if (address)
    {
        uintptr_t addrs = address + size + *(int*)(address + offset);
        return addrs;
    }
}


inline static Vec2 CalcAngle2(Vec3 startPOS, Vec3 endPOS)
{
    float deltaX = endPOS.x - startPOS.x;
    float deltaY = endPOS.y - startPOS.y;
    float deltaZ = endPOS.z - startPOS.z;
    float dist = sqrt(pow((endPOS.x - startPOS.x), 2.0) + pow((endPOS.y - startPOS.y), 2.0) + pow((endPOS.z - startPOS.z), 2.0));

    if (dist < 0)
    {
        dist = dist * -1;
    }

    float xzlength = sqrt((deltaX * deltaX) + (deltaZ * deltaZ));
    float angleX = atan2(deltaY, xzlength) * (-57.2957795);
    float angleY = atan2(deltaX, deltaZ) * (57.2957795);
    Vec2 angle = { angleY,angleX };
    return  angle;
}

inline static Vec2 ReturnAngle(Vec2 angle)
{
    if (angle.x + 360 > 360)
        return { angle.x,angle.y + 270 };
    return { angle.x + 360,angle.y + 270 };
}

template<typename T, typename... args> static T call(uint64_t address, args... _args) {
    T(*fn)(args...) = (T(*)(args...))address;
    return fn(_args...);
}
