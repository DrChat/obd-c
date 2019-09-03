/**
 ******************************************************************************
 * OBD-C : CAN BUS Research Project                                           *
 ******************************************************************************
 * Copyright 2019 Justin Moore. All rights reserved.                          *
 ******************************************************************************
 */
#include "transport_bt_win32.h"

#include <WinSock2.h>
#include <Windows.h>

std::vector<std::string> TransportBTWin32::LookupDevices()
{
	return {};
}

void TransportBTWin32::Send(const uint8_t* data, size_t len)
{
	(void)data;
	(void)len;
}
