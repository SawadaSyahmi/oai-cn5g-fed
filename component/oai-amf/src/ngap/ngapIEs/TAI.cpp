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

#include "TAI.hpp"

namespace ngap {

//------------------------------------------------------------------------------
TAI::TAI() {}

//------------------------------------------------------------------------------
TAI::~TAI() {}

//------------------------------------------------------------------------------
void TAI::setTAI(const PlmnId& m_plmnId, const TAC& m_tac) {
  plmnId = m_plmnId;
  tac    = m_tac;
}

//------------------------------------------------------------------------------
void TAI::setTAI(
    const std::string& mcc, const std::string& mnc, const uint32_t& m_tac) {
  plmnId.setMccMnc(mcc, mnc);
  tac.setTac(m_tac);
}

//------------------------------------------------------------------------------
void TAI::getTAI(std::string& mcc, std::string& mnc, uint32_t& m_tac) {
  plmnId.getMcc(mcc);
  plmnId.getMnc(mnc);
  m_tac = tac.getTac();
}

//------------------------------------------------------------------------------
void TAI::setTAI(const Tai_t& tai) {
  plmnId.setMccMnc(tai.mcc, tai.mnc);
  tac.setTac(tai.tac);
}
//------------------------------------------------------------------------------
bool TAI::encode2TAI(Ngap_TAI_t* tai) {
  if (!plmnId.encode2octetstring(tai->pLMNIdentity)) return false;
  if (!tac.encode2octetstring(tai->tAC)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool TAI::decodefromTAI(Ngap_TAI_t* tai) {
  if (!plmnId.decodefromoctetstring(tai->pLMNIdentity)) return false;
  if (!tac.decodefromoctetstring(tai->tAC)) return false;

  return true;
}

//------------------------------------------------------------------------------
void TAI::getTAI(PlmnId& m_plmnId, TAC& m_tac) {
  m_plmnId = plmnId;
  m_tac    = tac;
}

//------------------------------------------------------------------------------
void TAI::getTAI(Tai_t& tai) {
  plmnId.getMcc(tai.mcc);
  plmnId.getMnc(tai.mnc);
  tai.tac = tac.getTac();
}
}  // namespace ngap
