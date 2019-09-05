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

  using dispatch_fn = std::function<void(FakeELM327*, const std::string&, Parameter)>;

  struct DispatchEntry {
    // Constructor for no-parameter commands.
    DispatchEntry(Parameter param, void (FakeELM327::*fn)(const std::string&, Parameter))
        : parm(param), fn(std::mem_fn(fn)), min_param(0), max_param(0) {}

	// Constructor with min_param and max_param (for commands that take parameters)
    DispatchEntry(Parameter param, uint32_t min_param, uint32_t max_param,
                  void (FakeELM327::*fn)(const std::string&, Parameter))
        : parm(param), fn(std::mem_fn(fn)), min_param(min_param), max_param(max_param) {}

    Parameter parm;  // Parameter, used for reusable functions (e.g. boolean)
    uint32_t min_param;
    uint32_t max_param;
    dispatch_fn fn;
  };
  static const std::map<std::string, DispatchEntry> dispatch_table;

 private:
  // Member variables
  std::string last_command_;

 private:
  // Member functions
  void SetDefaultParameters();
  bool GetParamValueBool(Parameter param);

  bool DispatchCommand(const std::string& cmd);  // Dispatch a command. Returns false if it was not found.

 private:
  void OnSetIgnoreOK(const std::string& arg, Parameter param);
  void OnSetBoolean(const std::string& arg, Parameter param);

  // Commands
  void OnDisplayDeviceDesc(const std::string& arg, Parameter param);
  void OnSetProtocol(const std::string& arg, Parameter param);
  void OnInfo(const std::string& arg, Parameter param);
  void OnReset(const std::string& arg, Parameter param);
};