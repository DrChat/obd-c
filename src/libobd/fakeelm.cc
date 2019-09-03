/**
 ******************************************************************************
 * OBD-C : CAN BUS Research Project                                           *
 ******************************************************************************
 * Copyright 2019 Justin Moore. All rights reserved.                          *
 ******************************************************************************
 */
#include "fakeelm.h"

#include <cassert>
#include <functional>

const static std::string MESSAGE_OK = "OK";
const static std::string MESSAGE_ERR = "?";

// This is the dispatch table for commands prefixed with "AT".
const std::map<std::string, FakeELM327::DispatchEntry>
    FakeELM327::dispatch_table = {
        // Boolean parameters
        {"E0", {PAR_ECHO, std::mem_fn(&FakeELM327::OnSetValueFalse)}},
        {"E1", {PAR_ECHO, std::mem_fn(&FakeELM327::OnSetValueTrue)}},
        {"L0", {PAR_LINEFEED, std::mem_fn(&FakeELM327::OnSetValueFalse)}},
        {"L1", {PAR_LINEFEED, std::mem_fn(&FakeELM327::OnSetValueTrue)}},

        // Commands
        {"SP", {PAR_INVALID, std::mem_fn(&FakeELM327::OnSetProtocol)}},
        {"I", {PAR_INVALID, std::mem_fn(&FakeELM327::OnInfo)}},
        {"Z", {PAR_INVALID, std::mem_fn(&FakeELM327::OnReset)}},
};

const FakeELM327::ParameterValue FakeELM327::default_params[PAR_MAX] = {
    /* PAR_INVALID  */ {PARTYPE_INVALID, false},
    /* PAR_LINEFEED */ {PARTYPE_BOOL, true},
    /* PAR_ECHO     */ {PARTYPE_BOOL, true},
};

FakeELM327::FakeELM327() {
  // Initialize with default parameters.
  SetDefaultParameters();
}

void FakeELM327::OnConnected() { Send(">"); }

void FakeELM327::OnReceive(const uint8_t* buf, size_t len) {
  if (GetParamValueBool(PAR_ECHO)) {
    Send(buf, len);
  }

  // Check for an AT command.
  if (len > 2 && buf[0] == 'A' && buf[1] == 'T') {
    // Parse the buffer into plaintext. Fail if it is not ASCII.
    std::string s;
    s.reserve(len - 2);
    for (size_t i = 2; i < len; i++) {
      if (std::isprint(buf[i]))
        s.append(1, buf[i]);
      else if (buf[i] == '\r' || buf[i] == '\n')
        break;
      else {
        // Invalid string.
        SendReply(MESSAGE_ERR);
        return;
      }
    }

    // Dispatch the command.
    auto it = dispatch_table.find(s);
    if (it != dispatch_table.end()) {
      it->second.fn(this, s, it->second.parm);
    } else {
      SendReply(MESSAGE_ERR);
    }
  }

  Send(">");
}

void FakeELM327::SendReply(const std::string& reply) {
  std::string data;
  data.reserve(reply.length() + 2);
  data = reply;

  if (GetParamValueBool(PAR_LINEFEED)) {
    // Send string with '\r\n'
    data += "\r\n";
  } else {
    // Send string with '\r'
    data += "\r";
  }

  Send((const uint8_t*)data.c_str(), data.length());
}

void FakeELM327::SetDefaultParameters() {
  std::memcpy(params_, default_params, sizeof(default_params));
}

bool FakeELM327::GetParamValueBool(Parameter param) {
  assert(param < PAR_MAX);
  assert(params_[param].type == PARTYPE_BOOL);

  return params_[param].bval;
}

void FakeELM327::OnSetValueTrue(const std::string& cmd, Parameter param) {
  (void)cmd;
  assert(param < PAR_MAX);
  assert(params_[param].type == PARTYPE_BOOL);

  params_[param].bval = true;
}

void FakeELM327::OnSetValueFalse(const std::string& cmd, Parameter param) {
  (void)cmd;
  assert(param < PAR_MAX);
  assert(params_[param].type == PARTYPE_BOOL);

  params_[param].bval = false;
}

void FakeELM327::OnSetProtocol(const std::string& cmd, Parameter param) {
  (void)cmd;
  (void)param;
}

void FakeELM327::OnInfo(const std::string& cmd, Parameter param) {
  (void)cmd;
  (void)param;
  SendReply("ELM327 v2.1");
}

void FakeELM327::OnReset(const std::string& cmd, Parameter param) {
  (void)cmd;
  (void)param;
  SetDefaultParameters();
  SendReply(MESSAGE_OK);
}
