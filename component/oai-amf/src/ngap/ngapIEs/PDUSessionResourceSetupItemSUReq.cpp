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

#include "PDUSessionResourceSetupItemSUReq.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceSetupItemSUReq::PDUSessionResourceSetupItemSUReq() {
  nAS_PDU = nullptr;
}

//------------------------------------------------------------------------------
PDUSessionResourceSetupItemSUReq::~PDUSessionResourceSetupItemSUReq() {}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupItemSUReq::setPDUSessionResourceSetupItemSUReq(
    const PDUSessionID& m_pDUSessionID, NAS_PDU* m_nAS_PDU,
    const S_NSSAI& m_s_NSSAI,
    const OCTET_STRING_t& m_pDUSessionResourceSetupRequestTransfer) {
  pDUSessionID = m_pDUSessionID;
  nAS_PDU      = m_nAS_PDU;
  s_NSSAI      = m_s_NSSAI;
  pDUSessionResourceSetupRequestTransfer =
      m_pDUSessionResourceSetupRequestTransfer;
}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupItemSUReq::getPDUSessionResourceSetupItemSUReq(
    PDUSessionID& m_pDUSessionID, NAS_PDU*& m_nAS_PDU, S_NSSAI& m_s_NSSAI,
    OCTET_STRING_t& m_pDUSessionResourceSetupRequestTransfer) {
  m_pDUSessionID = pDUSessionID;
  m_nAS_PDU      = nAS_PDU;
  m_s_NSSAI      = s_NSSAI;
  m_pDUSessionResourceSetupRequestTransfer =
      pDUSessionResourceSetupRequestTransfer;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupItemSUReq::encode2PDUSessionResourceSetupItemSUReq(
    Ngap_PDUSessionResourceSetupItemSUReq_t* pduSessionResourceSetupItemSUReq) {
  if (!pDUSessionID.encode2PDUSessionID(
          pduSessionResourceSetupItemSUReq->pDUSessionID))
    return false;
  if (nAS_PDU) {
    Ngap_NAS_PDU_t* naspdu =
        (Ngap_NAS_PDU_t*) calloc(1, sizeof(Ngap_NAS_PDU_t));
    if (!naspdu) return false;
    if (!nAS_PDU->encode2octetstring(*naspdu)) {
      free_wrapper((void**) &naspdu);
      return false;
    }
    pduSessionResourceSetupItemSUReq->pDUSessionNAS_PDU = naspdu;
  }
  if (!s_NSSAI.encode2S_NSSAI(&pduSessionResourceSetupItemSUReq->s_NSSAI))
    return false;
  pduSessionResourceSetupItemSUReq->pDUSessionResourceSetupRequestTransfer =
      pDUSessionResourceSetupRequestTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupItemSUReq::
    decodefromPDUSessionResourceSetupItemSUReq(
        Ngap_PDUSessionResourceSetupItemSUReq_t*
            pduSessionResourceSetupItemSUReq) {
  if (!pDUSessionID.decodefromPDUSessionID(
          pduSessionResourceSetupItemSUReq->pDUSessionID))
    return false;
  if (!s_NSSAI.decodefromS_NSSAI(&pduSessionResourceSetupItemSUReq->s_NSSAI))
    return false;

  if (pduSessionResourceSetupItemSUReq->pDUSessionNAS_PDU) {
    nAS_PDU = new NAS_PDU();
    if (!nAS_PDU->decodefromoctetstring(
            *pduSessionResourceSetupItemSUReq->pDUSessionNAS_PDU))
      return false;
  }

  pDUSessionResourceSetupRequestTransfer =
      pduSessionResourceSetupItemSUReq->pDUSessionResourceSetupRequestTransfer;

  return true;
}

}  // namespace ngap
