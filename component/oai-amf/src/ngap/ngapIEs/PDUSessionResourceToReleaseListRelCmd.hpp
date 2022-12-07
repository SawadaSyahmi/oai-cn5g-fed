/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#ifndef _PDU_SESSION_RESOURCE_TO_RELEASE_LIST_REL_CMD_H_
#define _PDU_SESSION_RESOURCE_TO_RELEASE_LIST_REL_CMD_H_

#include "PDUSessionResourceToReleaseItemRelCmd.hpp"
#include <vector>

extern "C" {
#include "Ngap_PDUSessionResourceToReleaseItemRelCmd.h"
#include "Ngap_PDUSessionResourceToReleaseListRelCmd.h"
}

namespace ngap {

class PDUSessionResourceToReleaseListRelCmd {
 public:
  PDUSessionResourceToReleaseListRelCmd();
  virtual ~PDUSessionResourceToReleaseListRelCmd();

  void setPDUSessionResourceToReleaseListRelCmd(
      const std::vector<PDUSessionResourceToReleaseItemRelCmd>& list);
  void getPDUSessionResourceToReleaseListRelCmd(
      std::vector<PDUSessionResourceToReleaseItemRelCmd>& list);

  bool encode2PDUSessionResourceToReleaseListRelCmd(
      Ngap_PDUSessionResourceToReleaseListRelCmd_t*
          pduSessionResourceToReleaseListRelCmd);
  bool decodefromPDUSessionResourceToReleaseListRelCmd(
      Ngap_PDUSessionResourceToReleaseListRelCmd_t*
          pduSessionResourceToReleaseListRelCmd);

 private:
  std::vector<PDUSessionResourceToReleaseItemRelCmd> itemRelCmdList;
};

}  // namespace ngap

#endif
