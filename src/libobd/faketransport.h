/**
 ******************************************************************************
 * OBD-C : CAN BUS Research Project                                           *
 ******************************************************************************
 * Copyright 2019 Justin Moore. All rights reserved.                          *
 ******************************************************************************
 */
#pragma once

#include <cinttypes>
#include <functional>
#include <string>

class FakeTransport;

class FakeTransportDevice {
 public:
  virtual void OnReceive(const uint8_t* data, size_t len) = 0;
  void Send(const uint8_t* data, size_t len);

 private:
  friend class FakeTransport;
  FakeTransport* transport_ = nullptr;
};

class FakeTransport {
 public:
  typedef std::function<void(const uint8_t* data, size_t len)> recv_handler;

  FakeTransport() = default;

  void Connect(FakeTransportDevice* dev);

  void Send(const uint8_t* data, size_t len);
  void Send(const std::string& data);

  void SetReceiveHandler(recv_handler recv);

 private:
  friend class FakeTransportDevice;

  void OnReceive(const uint8_t* data, size_t len);

  recv_handler recv_ = nullptr;
  FakeTransportDevice* dev_ = nullptr;
};
