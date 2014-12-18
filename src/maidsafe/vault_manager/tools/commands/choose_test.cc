/*  Copyright 2014 MaidSafe.net limited

    This MaidSafe Software is licensed to you under (1) the MaidSafe.net Commercial License,
    version 1.0 or later, or (2) The General Public License (GPL), version 3, depending on which
    licence you accepted on initial access to the Software (the "Licences").

    By contributing code to the MaidSafe Software, or to this project generally, you agree to be
    bound by the terms of the MaidSafe Contributor Agreement, version 1.0, found in the root
    directory of this project at LICENSE, COPYING and CONTRIBUTOR respectively and also
    available at: http://www.maidsafe.net/licenses

    Unless required by applicable law or agreed to in writing, the MaidSafe Software distributed
    under the GPL Licence is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS
    OF ANY KIND, either express or implied.

    See the Licences for the specific language governing permissions and limitations relating to
    use of the MaidSafe Software.                                                                 */

#include "maidsafe/vault_manager/tools/commands/choose_test.h"

#include "maidsafe/common/log.h"
#include "maidsafe/common/make_unique.h"

#include "maidsafe/vault_manager/tools/local_network_controller.h"

namespace maidsafe {

namespace vault_manager {

namespace tools {

ChooseTest::ChooseTest(LocalNetworkController* local_network_controller)
    : Command(local_network_controller, "Test options.",
              "\nUnimplemented as yet. "
              "(type 100 for quit or 101 for tear down vaults with interval)\n",
              "Main Test Choices"),
      choice_(0) {}

void ChooseTest::GetChoice() {
  TLOG(kYellow) << kInstructions_;
  while (!DoGetChoice(choice_, static_cast<int*>(nullptr), 100, 101)) {
    TLOG(kDefaultColour) << '\n' << kInstructions_;
  }
}

void ChooseTest::HandleChoice() {
  // Throwing an error to indicate a quit request
  if (choice_ == 101)
    BOOST_THROW_EXCEPTION(MakeError(VaultManagerErrors::timed_out));
  if (choice_ == 100)
    BOOST_THROW_EXCEPTION(MakeError(CommonErrors::success));
}

}  // namespace tools

}  // namespace vault_manager

}  // namespace maidsafe
