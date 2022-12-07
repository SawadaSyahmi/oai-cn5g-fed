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

#ifndef _PDU_SESSION_RESOURCE_RELEASE_COMMAND_H_
#define _PDU_SESSION_RESOURCE_RELEASE_COMMAND_H_

#include "PDUSessionResourceToReleaseListRelCmd.hpp"

#include "RANPagingPriority.hpp"
#include "NAS-PDU.hpp"
#include "NgapUEMessage.hpp"

namespace ngap {

class PduSessionResourceReleaseCommandMsg : public NgapUEMessage {
 public:
  PduSessionResourceReleaseCommandMsg();
  virtual ~PduSessionResourceReleaseCommandMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setRanPagingPriority(const uint8_t& priority);
  int getRanPagingPriority();

  void setNasPdu(uint8_t* nas, size_t sizeofnas);
  bool getNasPdu(uint8_t*& nas, size_t& sizeofnas);

  void setPduSessionResourceToReleaseList(
      const std::vector<PDUSessionResourceToReleaseItem_t>& list);
  bool getPduSessionResourceToReleaseList(
      std::vector<PDUSessionResourceToReleaseItem_t>& list);

 private:
  Ngap_PDUSessionResourceReleaseCommand_t* pduSessionResourceReleaseCommandIEs;

  RANPagingPriority* ranPagingPriority;  // Optional
  NAS_PDU* nasPdu;                       // Optional
  PDUSessionResourceToReleaseListRelCmd
      pduSessionResourceToReleaseList;  // Mandatory
};

}  // namespace ngap
#endif
