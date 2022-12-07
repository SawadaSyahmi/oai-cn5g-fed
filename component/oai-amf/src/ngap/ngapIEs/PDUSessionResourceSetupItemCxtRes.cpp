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

#include "PDUSessionResourceSetupItemCxtRes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceSetupItemCxtRes::PDUSessionResourceSetupItemCxtRes() {}

//------------------------------------------------------------------------------
PDUSessionResourceSetupItemCxtRes::~PDUSessionResourceSetupItemCxtRes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupItemCxtRes::setPDUSessionResourceSetupItemCxtRes(
    const PDUSessionID& m_pDUSessionID,
    const OCTET_STRING_t& m_pDUSessionResourceSetupResponseTransfer) {
  pDUSessionID = m_pDUSessionID;
  pDUSessionResourceSetupResponseTransfer =
      m_pDUSessionResourceSetupResponseTransfer;
}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupItemCxtRes::getPDUSessionResourceSetupItemCxtRes(
    PDUSessionID& m_pDUSessionID,
    OCTET_STRING_t& m_pDUSessionResourceSetupResponseTransfer) {
  m_pDUSessionID = pDUSessionID;
  m_pDUSessionResourceSetupResponseTransfer =
      pDUSessionResourceSetupResponseTransfer;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupItemCxtRes::
    encode2PDUSessionResourceSetupItemCxtRes(
        Ngap_PDUSessionResourceSetupItemCxtRes_t*
            pduSessionResourceSetupItemCxtRes) {
  if (!pDUSessionID.encode2PDUSessionID(
          pduSessionResourceSetupItemCxtRes->pDUSessionID))
    return false;
  pduSessionResourceSetupItemCxtRes->pDUSessionResourceSetupResponseTransfer =
      pDUSessionResourceSetupResponseTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupItemCxtRes::
    decodefromPDUSessionResourceSetupItemCxtRes(
        Ngap_PDUSessionResourceSetupItemCxtRes_t*
            pduSessionResourceSetupItemCxtRes) {
  if (!pDUSessionID.decodefromPDUSessionID(
          pduSessionResourceSetupItemCxtRes->pDUSessionID))
    return false;
  pDUSessionResourceSetupResponseTransfer =
      pduSessionResourceSetupItemCxtRes
          ->pDUSessionResourceSetupResponseTransfer;

  return true;
}

}  // namespace ngap
