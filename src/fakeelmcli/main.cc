/**
 ******************************************************************************
 * OBD-C : CAN BUS Research Project                                           *
 ******************************************************************************
 * Copyright 2019 Justin Moore. All rights reserved.                          *
 ******************************************************************************
 */
#include <cstdio>
#include <string>

#include "libobd/fakeelm.h"
#include "libobd/faketransport.h"

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  FakeTransport t;
  FakeELM327 e;

  t.SetReceiveHandler(
      [](const uint8_t* buf, size_t len) { printf("%.*s", (int)len, buf); });

  t.Connect(&e);

  while (true) {
    int c;
    std::string s;
    while ((c = fgetc(stdin)) != '\n') {
      s += (char)c;
    }

    t.Send(s);
  }

  return 0;
}
