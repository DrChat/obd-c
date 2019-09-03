/**
 ******************************************************************************
 * OBD-C : CAN BUS Research Project                                           *
 ******************************************************************************
 * Copyright 2019 Justin Moore. All rights reserved.                          *
 ******************************************************************************
 */
#pragma once

#include <cinttypes>
#include <string>
#include <vector>

/**
 * \brief Bluetooth transport (for Win32)
 *
 * \warning This class assumes that \ref WSAStartup has been called.
 */
class TransportBTWin32 {
 public:
  std::vector<std::string> LookupDevices();
  void Send(const uint8_t* data, size_t len);

 private:
};