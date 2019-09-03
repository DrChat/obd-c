#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "libobd/fakeelm.h"
#include "libobd/faketransport.h"

using testing::_;
using testing::MockFunction;
using testing::Return;

TEST(FakeELM, BasicTest) {
  MockFunction<void(const uint8_t*, size_t)> mockReceive;
  FakeTransport t;
  FakeELM327 e;

  t.SetReceiveHandler(mockReceive.AsStdFunction());
  t.Connect(&e);

  EXPECT_CALL(mockReceive, Call(_, _));
  t.Send("ATI\r");
}