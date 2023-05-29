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

#ifndef UTIL_INCLUDE_UTIL_TOHEX_H_
#define UTIL_INCLUDE_UTIL_TOHEX_H_

#include <string>

namespace util {

/**
** \brief Dumps payload-like data to human-readable hex string.
**
** \details Each payload byte (2 digit hex value) is separate by a space.
**
** For example:
**
** \code{.cpp}
** char data[] = {0x01, 0x02, 0xFF, 0x00};
** auto result = PayloadToHex(data, sizeof(data));
** \endcode
**
** \c result will be set to "01 02 FF 00".
**
** \param[in] data Pointer to data.
** \param[in] size Size of data.
**
** \return String with space-separated hex value.
*/
std::string PayloadToHex(const void* data, std::size_t size);

/**
** \brief Dumps numeric (integral or floating-point type) data to human-readable hex
**        string.
**
** \details This function takes endianness (machine byte order) into the
**          consideration and outputs bytes in natural (big-endian) order.
**
** For example:
**
** \code{.cpp}
** int data = 0x12345;
** auto result = NumericToHex(&data, sizeof(data));
** \endcode
**
** \c result will be set to "00012345".
**
** \param[in] data Pointer to data.
** \param[in] size Size of data.
**
** \return String with hex value.
*/
std::string NumericToHex(const void* data, std::size_t size);

}  // namespace util

#endif  // UTIL_INCLUDE_UTIL_TOHEX_H_

/************************************************************************************/
