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

#ifndef _HANDOVERCOMMAND_H_
#define _HANDOVERCOMMAND_H_

#include "NgapUEMessage.hpp"
#include "PDUSessionResourceHandoverList.hpp"

extern "C" {
#include "Ngap_NGAP-PDU.h"
#include "Ngap_NGSetupRequest.h"
#include "Ngap_HandoverCommand.h"
}

namespace ngap {

typedef struct {
  uint8_t pduSessionId;
  OCTET_STRING_t HandoverCommandTransfer;
} PDUSessionResourceHandoverItem_t;

class HandoverCommandMsg : public NgapUEMessage {
 public:
  HandoverCommandMsg();
  virtual ~HandoverCommandMsg();

  void initialize();
  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setHandoverType(const long& type);
  // void getHandoverType(Ngap_HandoverType_t &type);

  void setPduSessionResourceHandoverList(
      const PDUSessionResourceHandoverList& list);
  void getPduSessionResourceHandoverList(PDUSessionResourceHandoverList& list);

  void setTargetToSource_TransparentContainer(
      const OCTET_STRING_t& targetTosource);
  void getTargetToSource_TransparentContainer(OCTET_STRING_t& targetTosource);

 private:
  Ngap_HandoverCommand_t* handoverCommandIEs;

  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  Ngap_HandoverType_t handoverType;  // Mandatory
  Ngap_NASSecurityParametersFromNGRAN_t*
      nASSecurityParametersFromNGRAN;  // TODO: Conditional
  PDUSessionResourceHandoverList pDUSessionResourceHandoverList;  // Optional
  bool pDUSessionResourceHandoverListIsSet;
  Ngap_PDUSessionResourceToReleaseListHOCmd_t*
      pDUSessionResourceToReleaseListHOCmd;  // TODO: Optional
  Ngap_TargetToSource_TransparentContainer_t
      targetToSource_TransparentContainer;                // Mandatory
  Ngap_CriticalityDiagnostics_t* criticalityDiagnostics;  // TODO: Optional
};

}  // namespace ngap

#endif
