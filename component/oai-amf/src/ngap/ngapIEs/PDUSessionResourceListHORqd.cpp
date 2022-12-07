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

#include "PDUSessionResourceListHORqd.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceListHORqd::PDUSessionResourceListHORqd() {}

//------------------------------------------------------------------------------
PDUSessionResourceListHORqd::~PDUSessionResourceListHORqd() {}

//------------------------------------------------------------------------------
void PDUSessionResourceListHORqd::setPDUSessionResourceListHORqd(
    const std::vector<PDUSessionResourceItemHORqd>& list) {
  itemHORqdList = list;
}

//------------------------------------------------------------------------------
void PDUSessionResourceListHORqd::getPDUSessionResourceListHORqd(
    std::vector<PDUSessionResourceItemHORqd>& list) {
  list = itemHORqdList;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceListHORqd::encode2PDUSessionResourceListHORqd(
    Ngap_PDUSessionResourceListHORqd_t* pduSessionResourceListHORQqd) {
  for (auto& item : itemHORqdList) {
    Ngap_PDUSessionResourceItemHORqd_t* itemHORqd =
        (Ngap_PDUSessionResourceItemHORqd_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceItemHORqd_t));
    if (!itemHORqd) return false;
    if (!item.encode2PDUSessionResourceItemHORqd(itemHORqd)) return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceListHORQqd->list, itemHORqd) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceListHORqd::decodefromPDUSessionResourceListHORqd(
    Ngap_PDUSessionResourceListHORqd_t* pduSessionResourceListHORQqd) {
  for (int i = 0; i < pduSessionResourceListHORQqd->list.count; i++) {
    PDUSessionResourceItemHORqd item = {};
    if (!item.decodefromPDUSessionResourceItemHORqd(
            pduSessionResourceListHORQqd->list.array[i]))
      return false;
    itemHORqdList.push_back(item);
  }

  return true;
}
}  // namespace ngap
