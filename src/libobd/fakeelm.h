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
#include <map>
#include <string>
#include <vector>

#include "faketransport.h"

/**
 * \brief Fake ELM327 device. This connects to the end of a fake transport.
 */
class FakeELM327 : public FakeTransportDevice {
 public:
  FakeELM327();

  void OnConnected() override;
  void OnReceive(const uint8_t* buf, size_t len) override;
  void SendReply(const std::string& reply);

 private:
  // ELM327 parameters.
  // N.B: If you intend to add more parameters, you will need to modify
  //      default_params.
  enum Parameter {
    PAR_INVALID,
    PAR_LINEFEED,  // '\n' following '\r'
    PAR_ECHO,      // Echo sent command
    PAR_MAX,
  };

  enum ParameterType {
    PARTYPE_INVALID,
    PARTYPE_BOOL,
    PARTYPE_INT,
  };

  struct ParameterValue {
    ParameterType type;
    union {
      bool bval;
      int ival;
    };
  };

  static const ParameterValue default_params[PAR_MAX];
  ParameterValue params_[PAR_MAX];

  struct DispatchEntry {
    Parameter parm;  // Parameter, used for reusable functions (e.g. boolean)
    std::function<void(FakeELM327* elm, const std::string& cmd, Parameter par)> fn;
  };
  static const std::map<std::string, DispatchEntry> dispatch_table;

 private:
  // Member variables
  std::string last_command_;

 private:
  // Member functions
  void SetDefaultParameters();
  bool GetParamValueBool(Parameter param);

 private:
  void OnSetIgnoreOK(const std::string& cmd, Parameter param);
  void OnSetValueTrue(const std::string& cmd, Parameter param);
  void OnSetValueFalse(const std::string& cmd, Parameter param);

  // Commands
  void OnSetProtocol(const std::string& cmd, Parameter param);
  void OnInfo(const std::string& cmd, Parameter param);
  void OnReset(const std::string& cmd, Parameter param);
};