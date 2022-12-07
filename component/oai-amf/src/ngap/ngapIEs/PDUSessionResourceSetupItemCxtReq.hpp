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

#ifndef _PDU_SESSION_RESOURCE_SETUP_ITEM_CXT_REQ_H_
#define _PDU_SESSION_RESOURCE_SETUP_ITEM_CXT_REQ_H_

#include "NAS-PDU.hpp"
#include "PDUSessionID.hpp"
#include "S-NSSAI.hpp"
#include <vector>

extern "C" {
#include "Ngap_PDUSessionResourceSetupItemCxtReq.h"
}

namespace ngap {

class PDUSessionResourceSetupItemCxtReq {
 public:
  PDUSessionResourceSetupItemCxtReq();
  virtual ~PDUSessionResourceSetupItemCxtReq();

  void setPDUSessionResourceSetupItemCxtReq(
      const PDUSessionID& m_pDUSessionID, NAS_PDU*& m_nAS_PDU,
      const S_NSSAI& m_s_NSSAI,
      const OCTET_STRING_t& m_pDUSessionResourceSetupRequestTransfer);
  void getPDUSessionResourceSetupItemCxtReq(
      PDUSessionID& m_pDUSessionID, NAS_PDU*& m_nAS_PDU, S_NSSAI& m_s_NSSAI,
      OCTET_STRING_t& m_pDUSessionResourceSetupRequestTransfer);

  bool encode2PDUSessionResourceSetupItemCxtReq(
      Ngap_PDUSessionResourceSetupItemCxtReq_t*
          pduSessionResourceSetupItemCxtReq);
  bool decodefromPDUSessionResourceSetupItemCxtReq(
      Ngap_PDUSessionResourceSetupItemCxtReq_t*
          pduSessionResourceSetupItemCxtReq);

 private:
  PDUSessionID pDUSessionID;                              // Mandatory
  NAS_PDU* nAS_PDU;                                       // Optional
  S_NSSAI s_NSSAI;                                        // Mandatory
  OCTET_STRING_t pDUSessionResourceSetupRequestTransfer;  // Mandatory
};

}  // namespace ngap

#endif
