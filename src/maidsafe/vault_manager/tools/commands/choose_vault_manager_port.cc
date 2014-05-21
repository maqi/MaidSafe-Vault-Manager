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

#include "maidsafe/vault_manager/tools/commands/choose_vault_manager_port.h"

#include <string>

#include "maidsafe/common/log.h"
#include "maidsafe/common/make_unique.h"

#include "maidsafe/vault_manager/tools/local_network_controller.h"
#include "maidsafe/vault_manager/tools/commands/choose_test.h"
#include "maidsafe/vault_manager/tools/commands/choose_vault_count.h"

namespace maidsafe {

namespace vault_manager {

namespace tools {

ChooseVaultManagerPort::ChooseVaultManagerPort(LocalNetworkController* local_network_controller,
                                               bool connect_to_running)
    : Command(local_network_controller, "VaultManager listening port.",
              "  This should be between\n1025 and 65536 inclusive. ('Enter' to use default " +
              std::to_string(GetDefault().kVaultManagerPort) + ")\n" + kPrompt_,
              connect_to_running ? "Connect to Running VaultManager" : ""),
      connect_to_running_(connect_to_running) {}

void ChooseVaultManagerPort::GetChoice() {
  TLOG(kDefaultColour) << kInstructions_;
  while (!DoGetChoice(local_network_controller_->vault_manager_port,
                      &GetDefault().kVaultManagerPort, 1025, 65536)) {
    TLOG(kDefaultColour) << '\n' << kInstructions_;
  }
}

void ChooseVaultManagerPort::HandleChoice() {
  if (connect_to_running_) {
    ClientInterface::SetTestEnvironment(
        static_cast<Port>(local_network_controller_->vault_manager_port),
        GetDefault().kTestEnvRootDir, GetDefault().kPathToVault, routing::BootstrapContact{}, 0);
    passport::MaidAndSigner maid_and_signer{ passport::CreateMaidAndSigner() };
    local_network_controller_->client_interface =
        maidsafe::make_unique<ClientInterface>(maid_and_signer.first);
    local_network_controller_->current_command =
        maidsafe::make_unique<ChooseTest>(local_network_controller_);
    TLOG(kGreen) << "Successfully connected to VaultManager.\n";
    TLOG(kDefaultColour) << kSeparator_;
  }
  else {
    local_network_controller_->current_command =
        maidsafe::make_unique<ChooseVaultCount>(local_network_controller_);
  }
}

}  // namespace tools

}  // namespace vault_manager

}  // namespace maidsafe
