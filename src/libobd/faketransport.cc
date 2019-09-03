/**
 ******************************************************************************
 * OBD-C : CAN BUS Research Project                                           *
 ******************************************************************************
 * Copyright 2019 Justin Moore. All rights reserved.                          *
 ******************************************************************************
 */
#include "faketransport.h"

void FakeTransportDevice::Send(const uint8_t* data, size_t len) {
  transport_->OnReceive(data, len);
}

void FakeTransportDevice::Send(const std::string& data) {
  transport_->OnReceive((const uint8_t*)data.c_str(), data.length());
}

void FakeTransport::Connect(FakeTransportDevice* dev) {
  dev_ = dev;
  dev->transport_ = this;

  dev->OnConnected();
}

void FakeTransport::Send(const uint8_t* data, size_t len) {
  dev_->OnReceive(data, len);
}

void FakeTransport::Send(const std::string& data) {
  Send((const uint8_t*)data.c_str(), data.length());
}

void FakeTransport::SetReceiveHandler(recv_handler recv) { recv_ = recv; }

void FakeTransport::OnReceive(const uint8_t* data, size_t len) {
  recv_(data, len);
}
