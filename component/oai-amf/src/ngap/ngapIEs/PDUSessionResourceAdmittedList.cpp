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

#include "PDUSessionResourceAdmittedList.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceAdmittedList::PDUSessionResourceAdmittedList() {}

//------------------------------------------------------------------------------
PDUSessionResourceAdmittedList::~PDUSessionResourceAdmittedList() {}

//------------------------------------------------------------------------------
void PDUSessionResourceAdmittedList::setPDUSessionResourceAdmittedList(
    const std::vector<PDUSessionResourceAdmittedItem>& list) {
  admittedItemList = list;
}

//------------------------------------------------------------------------------
void PDUSessionResourceAdmittedList::getPDUSessionResourceAdmittedList(
    std::vector<PDUSessionResourceAdmittedItem>& item) {
  item = admittedItemList;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceAdmittedList::encode2PDUSessionResourceAdmittedList(
    Ngap_PDUSessionResourceAdmittedList_t* pduSessionResourceAdmittedList) {
  for (auto& item : admittedItemList) {
    Ngap_PDUSessionResourceAdmittedItem_t* response =
        (Ngap_PDUSessionResourceAdmittedItem_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceAdmittedItem_t));
    if (!response) return false;
    if (!item.encode2PDUSessionResourceAdmittedItem(response)) return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceAdmittedList->list, response) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceAdmittedList::decodefromPDUSessionResourceAdmittedList(
    Ngap_PDUSessionResourceAdmittedList_t* pduSessionResourceAdmittedList) {
  for (int i = 0; i < pduSessionResourceAdmittedList->list.count; i++) {
    PDUSessionResourceAdmittedItem item = {};
    if (!item.decodefromPDUSessionResourceAdmittedItem(
            pduSessionResourceAdmittedList->list.array[i]))
      return false;
    admittedItemList.push_back(item);
  }

  return true;
}

}  // namespace ngap
