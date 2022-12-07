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

#include "PDUSessionResourceReleasedListRelRes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceReleasedListRelRes::PDUSessionResourceReleasedListRelRes() {}

//------------------------------------------------------------------------------
PDUSessionResourceReleasedListRelRes::~PDUSessionResourceReleasedListRelRes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceReleasedListRelRes::
    setPDUSessionResourceReleasedListRelRes(
        const std::vector<PDUSessionResourceReleasedItemRelRes>& list) {
  itemRelResList = list;
}

//------------------------------------------------------------------------------
void PDUSessionResourceReleasedListRelRes::
    getPDUSessionResourceReleasedListRelRes(
        std::vector<PDUSessionResourceReleasedItemRelRes>& list) {
  list = itemRelResList;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceReleasedListRelRes::
    encode2PDUSessionResourceReleasedListRelRes(
        Ngap_PDUSessionResourceReleasedListRelRes_t*
            pduSessionResourceReleasedListRelRes) {
  for (auto& item : itemRelResList) {
    Ngap_PDUSessionResourceReleasedItemRelRes_t* rel =
        (Ngap_PDUSessionResourceReleasedItemRelRes_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceReleasedItemRelRes_t));
    if (!rel) return false;
    if (!item.encode2PDUSessionResourceReleasedItemRelRes(rel)) return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceReleasedListRelRes->list, rel) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceReleasedListRelRes::
    decodefromPDUSessionResourceReleasedListRelRes(
        Ngap_PDUSessionResourceReleasedListRelRes_t*
            pduSessionResourceReleasedListRelRes) {
  for (int i = 0; i < pduSessionResourceReleasedListRelRes->list.count; i++) {
    PDUSessionResourceReleasedItemRelRes item = {};
    if (!item.decodefromPDUSessionResourceReleasedItemRelRes(
            pduSessionResourceReleasedListRelRes->list.array[i]))
      return false;
    itemRelResList.push_back(item);
  }
  return true;
}

}  // namespace ngap
