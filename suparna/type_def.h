#ifndef TYPE_DEF_H
#define TYPE_DEF_H

// #ifndef size_type
// #define size_type unsigned long long
// #endif

using size_type = unsigned long long;

template<typename T, T v>
struct ReferenceBase
{ static constexpr T value = v; };

#if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
struct IsBigEndian : ReferenceBase<bool, __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__>
{ };
struct IsLittleEndian : ReferenceBase<bool, __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__>
{ };
#else
static constexpr unsigned char __EndianValues[2] = {0, 1};

struct IsBigEndian : ReferenceBase<bool, __EndianValues[0] == 0x01>
{ };
struct IsLittleEndian : ReferenceBase<bool, __EndianValues[0] == 0x00>
{ };
#endif

// 本机是大端机器有：  IsBigEndian::value为true，IsLittleEndian::value为false

#endif