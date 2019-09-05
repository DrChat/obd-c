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
const std::map<std::string, FakeELM327::DispatchEntry> FakeELM327::dispatch_table = {
    // Ignored commands
    {"AL", {PAR_INVALID, &FakeELM327::OnSetIgnoreOK}},

    // Boolean parameters
    {"E", {PAR_ECHO, 1, 1, &FakeELM327::OnSetBoolean}},
    {"L", {PAR_LINEFEED, 1, 1, &FakeELM327::OnSetBoolean}},

    // Commands
    {"@1", {PAR_INVALID, &FakeELM327::OnDisplayDeviceDesc}},
    {"D", {PAR_INVALID, &FakeELM327::OnReset}},
    {"I", {PAR_INVALID, &FakeELM327::OnInfo}},
    {"SP", {PAR_INVALID, &FakeELM327::OnSetProtocol}},
    {"Z", {PAR_INVALID, &FakeELM327::OnReset}},
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

  // Repeat last command
  if (len >= 1 && (buf[0] == '\r' || buf[0] == '\n')) {
    buf = (const uint8_t*)last_command_.c_str();
    len = last_command_.length();
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
    if (!DispatchCommand(s)) {
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

void FakeELM327::SetDefaultParameters() { std::memcpy(params_, default_params, sizeof(default_params)); }

bool FakeELM327::GetParamValueBool(Parameter param) {
  assert(param < PAR_MAX);
  assert(params_[param].type == PARTYPE_BOOL);

  return params_[param].bval;
}

bool FakeELM327::DispatchCommand(const std::string& cmd) {
  std::string param;
  auto it = dispatch_table.begin();

  // Search for the command.
  do {
    // Exact match.
    it = dispatch_table.find(cmd);
    if (it != dispatch_table.end()) {
      param = "";
      break;
    }

    // 4 to 3-character commands.
    for (size_t i = 2; i >= 1; i--) {
      it = dispatch_table.find(cmd.substr(0, i));
      if (it != dispatch_table.end()) {
        param = cmd.substr(i);
        break;
      }
    }

    if (it != dispatch_table.end()) break;
  } while (0);

  if (it != dispatch_table.end()) {
    it->second.fn(this, param, it->second.parm);
    return true;
  }

  return false;
}

void FakeELM327::OnSetIgnoreOK(const std::string& arg, Parameter param) {
  (void)arg;
  (void)param;
  SendReply(MESSAGE_OK);
}

void FakeELM327::OnSetBoolean(const std::string& arg, Parameter param) {
  (void)arg;
  assert(param < PAR_MAX);
  assert(params_[param].type == PARTYPE_BOOL);

  if (arg == "1") {
    params_[param].bval = true;
    SendReply(MESSAGE_OK);
    return;
  } else if (arg == "0") {
    params_[param].bval = false;
    SendReply(MESSAGE_OK);
    return;
  }

  SendReply(MESSAGE_ERR);
}

void FakeELM327::OnDisplayDeviceDesc(const std::string& arg, Parameter param) {
  (void)arg;
  (void)param;
  SendReply("Fake OBDII ELM327 adapter");
}

void FakeELM327::OnSetProtocol(const std::string& arg, Parameter param) {
  (void)arg;
  (void)param;
  SendReply(MESSAGE_OK);
}

void FakeELM327::OnInfo(const std::string& arg, Parameter param) {
  (void)arg;
  (void)param;
  SendReply("ELM327 v2.1");
}

void FakeELM327::OnReset(const std::string& arg, Parameter param) {
  (void)arg;
  (void)param;
  SetDefaultParameters();
  SendReply(MESSAGE_OK);
}
