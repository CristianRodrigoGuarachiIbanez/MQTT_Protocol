/*
**************************************************************************************
** Project: TBM/VCM Platform
**
** Copyright (C) MAN Truck & Bus AG
**
** This document contains proprietary information belonging to MAN Truck & Bus AG.
** Passing on and copying of this document, and communication of its contents
** is not permitted without prior written authorization.
**
**************************************************************************************
*/

/*
**====================================================================================
** Provided include files
**====================================================================================
*/
#include "util/ToHex.h"

/*
**====================================================================================
** Required include files
**====================================================================================
*/
#include <cassert>

/*
**====================================================================================
** Unnamed Namespaces
**====================================================================================
*/
namespace {

constexpr auto hex_lookup = "0123456789ABCDEF";

}  // namespace

/*
**====================================================================================
** Named Namespaces
**====================================================================================
*/
namespace util {

std::string PayloadToHex(const void* data, std::size_t size) {
  assert(data);

  if (!size) {
    return {};
  }

  auto result = std::string(3 * size - 1, ' ');
  auto output_ptr = &result[0];

  for (decltype(size) i = 0; i < size; i++) {
    const auto data_byte = *(static_cast<const std::uint8_t*>(data) + i);
    *(output_ptr++) = hex_lookup[(data_byte & 0xF0) >> 4];
    *(output_ptr++) = hex_lookup[data_byte & 0x0F];
    output_ptr++;
  }

  return result;
}

std::string NumericToHex(const void* data, std::size_t size) {
  assert(data);
  assert(size);

  auto result = std::string(2 * size, ' ');
  auto output_ptr = &result[0];

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  for (auto i = size; i > 0; i--) {
    const auto data_byte = *(static_cast<const std::uint8_t*>(data) + i - 1);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  for (decltype(size) i = 0; i < size; i++) {
    const auto data_byte = *(static_cast<const std::uint8_t*>(data) + i);
#else
#error "Unsupported machine byte order"
#endif
    *(output_ptr++) = hex_lookup[(data_byte & 0xF0) >> 4];
    *(output_ptr++) = hex_lookup[data_byte & 0x0F];
  }

  return result;
}

}  // namespace util

/************************************************************************************/
