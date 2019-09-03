#include "gtest/gtest.h"

#include "libobd/fakeelm.h"
#include "libobd/faketransport.h"

TEST(FakeELM, BasicTest) {
  FakeTransport t;
  FakeELM327 e;

  t.SetReceiveHandler([](const uint8_t* buf, size_t len) {
	  printf("%.*s", (int)len, buf);
  });

  t.Connect(&e);
  t.Send("ATI\r");
}