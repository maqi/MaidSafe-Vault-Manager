/***************************************************************************************************
 *  Copyright 2012 maidsafe.net limited                                                            *
 *                                                                                                 *
 *  The following source code is property of MaidSafe.net limited and is not meant for external    *
 *  use. The use of this code is governed by the licence file licence.txt found in the root of     *
 *  this directory and also on www.maidsafe.net.                                                   *
 *                                                                                                 *
 *  You are not free to copy, amend or otherwise use this source code without the explicit written *
 *  permission of the board of directors of MaidSafe.net.                                          *
 **************************************************************************************************/

#include "maidsafe/common/log.h"
#include "maidsafe/common/return_codes.h"
#include "maidsafe/common/test.h"
#include "maidsafe/common/utils.h"

#include "boost/date_time/posix_time/posix_time_duration.hpp"

#include "maidsafe/private/process_management/client_controller.h"
#include "maidsafe/private/process_management/vault_controller.h"
#include "maidsafe/private/process_management/invigilator.h"


namespace bptime = boost::posix_time;

namespace maidsafe {

namespace priv {

namespace process_management {

namespace test {

TEST(InvigilatorTest, BEH_StartStop) {
  // test case for startup (non-existent config file)
  LOG(kError) << "Test 1 starting";
  {
    if (fs::exists(fs::path(".") / Invigilator::kConfigFileName()))
      fs::remove(fs::path(".") / Invigilator::kConfigFileName());
    ASSERT_FALSE(fs::exists(fs::path(".") / Invigilator::kConfigFileName()));
    Invigilator invigilator;
    ClientController client_controller;
    int max_seconds = Invigilator::kMaxUpdateInterval().total_seconds();
    EXPECT_FALSE(client_controller.SetUpdateInterval(bptime::seconds(max_seconds + 1)));
    EXPECT_TRUE(client_controller.SetUpdateInterval(bptime::seconds(max_seconds)));
    int min_seconds = Invigilator::kMinUpdateInterval().total_seconds();
    EXPECT_TRUE(client_controller.SetUpdateInterval(bptime::seconds(min_seconds)));
    EXPECT_FALSE(client_controller.SetUpdateInterval(bptime::seconds(min_seconds - 1)));
    Sleep(boost::posix_time::seconds(2));
    EXPECT_TRUE(fs::exists(fs::path(".") / Invigilator::kConfigFileName()));
  }
  LOG(kError) << "Test 1 finished";
  // test case for existing config file with minimum content (generated in previous test case)
  {
    Invigilator invigilator;
    ClientController client_controller;
    asymm::Keys keys;
    ASSERT_EQ(kSuccess, rsa::GenerateKeyPair(&keys));
    EXPECT_TRUE(client_controller.StartVault(keys, "F"));
    Sleep(boost::posix_time::seconds(3));
    EXPECT_TRUE(fs::exists(fs::path(".") / Invigilator::kConfigFileName()));
  }
  LOG(kError) << "Test 2 finished";
}

}  // namespace test

}  // namespace process_management

}  // namespace priv

}  // namespace maidsafe