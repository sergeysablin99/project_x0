#include "../../includes/calendar.h"
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

TEST(calendar, calendar_1)
{
  EXPECT_NO_FATAL_FAILURE (Calendar());
}

