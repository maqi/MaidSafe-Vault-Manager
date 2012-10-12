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

#include <future>
#include <thread>

#include "maidsafe/common/test.h"
#include "maidsafe/common/types.h"

#include "maidsafe/private/utils/utilities.h"
#include "maidsafe/private/utils/fob.h"


namespace maidsafe {

namespace priv {

namespace utils {

namespace test {

class FobTest : public testing::Test {
 protected:
  void RunInParallel(std::function<void()> f, int num_threads = 6) {
    std::vector<std::future<void> > vec;
    for (int i = 0; i < num_threads; ++i)
      vec.push_back(std::async(std::launch::async, f));
    // wait for all threads to finish
    for (auto &i : vec)
      i.get();
  }
};


TEST_F(FobTest, BEH_FobSerialisationAndParsing) {
  auto f([=] {
           Fob ring(GenerateFob(nullptr));
           NonEmptyString serialised_ring1(SerialiseFob(ring));
           NonEmptyString serialised_ring2(SerialiseFob(ring));
           ASSERT_EQ(serialised_ring1.string(), serialised_ring2.string());
           Fob re_ring1(ParseFob(serialised_ring2));
           Fob re_ring2(ParseFob(serialised_ring1));
           ASSERT_EQ(re_ring1.identity.string(), re_ring2.identity.string());
           ASSERT_EQ(re_ring1.validation_token.string(), re_ring2.validation_token.string());
           ASSERT_TRUE(asymm::MatchingKeys(re_ring1.keys.public_key, re_ring2.keys.public_key));
           ASSERT_TRUE(asymm::MatchingKeys(re_ring1.keys.private_key, re_ring2.keys.private_key));
         });
  RunInParallel(f);
}

}  // namespace test

}  // namespace utils

}  // namespace priv

}  // namespace maidsafe