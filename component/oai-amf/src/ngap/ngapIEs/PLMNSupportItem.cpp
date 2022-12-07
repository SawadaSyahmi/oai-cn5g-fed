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

#include "PLMNSupportItem.hpp"

extern "C" {
#include "Ngap_SliceSupportItem.h"
}

using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PLMNSupportItem::PLMNSupportItem() {}

//------------------------------------------------------------------------------
PLMNSupportItem::~PLMNSupportItem() {}

//------------------------------------------------------------------------------
void PLMNSupportItem::setPlmnSliceSupportList(
    const PlmnId& m_plmn, const std::vector<S_NSSAI>& m_snssais) {
  plmn    = m_plmn;
  snssais = m_snssais;
}

//------------------------------------------------------------------------------
bool PLMNSupportItem::encode2PLMNSupportItem(
    Ngap_PLMNSupportItem_t* plmnsupportItem) {
  if (!plmn.encode2octetstring(plmnsupportItem->pLMNIdentity)) return false;
  for (std::vector<S_NSSAI>::iterator it = std::begin(snssais);
       it < std::end(snssais); ++it) {
    Ngap_SliceSupportItem_t* slice =
        (Ngap_SliceSupportItem_t*) calloc(1, sizeof(Ngap_SliceSupportItem_t));
    if (!it->encode2S_NSSAI(&slice->s_NSSAI)) return false;
    ASN_SEQUENCE_ADD(&plmnsupportItem->sliceSupportList.list, slice);
  }
  return true;
}

//------------------------------------------------------------------------------
bool PLMNSupportItem::decodefromPLMNSupportItem(
    Ngap_PLMNSupportItem_t* plmnsupportItem) {
  if (!plmn.decodefromoctetstring(plmnsupportItem->pLMNIdentity)) return false;

  for (int i = 0; i < plmnsupportItem->sliceSupportList.list.count; i++) {
    S_NSSAI snssai = {};
    if (!snssai.decodefromS_NSSAI(
            &plmnsupportItem->sliceSupportList.list.array[i]->s_NSSAI))
      return false;
    snssais.push_back(snssai);
  }
  return true;
}

//------------------------------------------------------------------------------
void PLMNSupportItem::getPlmnSliceSupportList(
    PlmnId& m_plmn, std::vector<S_NSSAI>& m_snssais) {
  m_plmn    = plmn;
  m_snssais = snssais;
}
}  // namespace ngap
