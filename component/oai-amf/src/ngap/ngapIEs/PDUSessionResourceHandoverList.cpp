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

#include "PDUSessionResourceHandoverList.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceHandoverList::PDUSessionResourceHandoverList() {}

//------------------------------------------------------------------------------
PDUSessionResourceHandoverList::~PDUSessionResourceHandoverList() {}

//------------------------------------------------------------------------------
void PDUSessionResourceHandoverList::setPDUSessionResourceHandoverList(
    const std::vector<PDUSessionResourceHandoverItem>& list) {
  handoverItemList = list;
}

//------------------------------------------------------------------------------
void PDUSessionResourceHandoverList::getPDUSessionResourceHandoverList(
    std::vector<PDUSessionResourceHandoverItem>& list) {
  list = handoverItemList;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceHandoverList::encode(
    Ngap_PDUSessionResourceHandoverList_t& pduSessionResourceHandoverList) {
  for (auto& item : handoverItemList) {
    Ngap_PDUSessionResourceHandoverItem_t* handoverItem =
        (Ngap_PDUSessionResourceHandoverItem_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceHandoverItem_t));

    if (!handoverItem) return false;
    if (!item.encode(*handoverItem)) return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceHandoverList.list, handoverItem) !=
        0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceHandoverList::decode(
    const Ngap_PDUSessionResourceHandoverList_t&
        pduSessionResourceHandoverList) {
  for (int i = 0; i < pduSessionResourceHandoverList.list.count; i++) {
    PDUSessionResourceHandoverItem item = {};

    if (!item.decode(*pduSessionResourceHandoverList.list.array[i]))
      return false;
    handoverItemList.push_back(item);
  }
  return true;
}

}  // namespace ngap
