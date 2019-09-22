#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../../includes/user.h"

using namespace testing;

TEST(user, User)
{
  QString name = "Ivan";
  QString surname = "Ivanov";
  QString email = "iva@nov.com";
  QString password = "qwerty";
  position post = position::USER;

  EXPECT_NO_FATAL_FAILURE (User(name, surname, email, password, post));
  User ivan(name, surname, email, password, post);
  EXPECT_EQ("Ivan", ivan.getName() );
  EXPECT_EQ ("Ivanov", ivan.getSurname() );
  EXPECT_NO_FATAL_FAILURE (ivan.setName ("oleg"));
  EXPECT_NO_FATAL_FAILURE (ivan.setPost (position::BOSS));
}

int test();
