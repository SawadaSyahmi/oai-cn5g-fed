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

#include "PDUSessionResourceSetupListSUReq.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceSetupListSUReq::PDUSessionResourceSetupListSUReq() {}

//------------------------------------------------------------------------------
PDUSessionResourceSetupListSUReq::~PDUSessionResourceSetupListSUReq() {}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListSUReq::setPDUSessionResourceSetupListSUReq(
    const std::vector<PDUSessionResourceSetupItemSUReq>& list) {
  itemSUReqList = list;
}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListSUReq::getPDUSessionResourceSetupListSUReq(
    std::vector<PDUSessionResourceSetupItemSUReq>& list) {
  list = itemSUReqList;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListSUReq::encode2PDUSessionResourceSetupListSUReq(
    Ngap_PDUSessionResourceSetupListSUReq_t* pduSessionResourceSetupListSUReq) {
  for (auto& item : itemSUReqList) {
    Ngap_PDUSessionResourceSetupItemSUReq_t* request =
        (Ngap_PDUSessionResourceSetupItemSUReq_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceSetupItemSUReq_t));
    if (!request) return false;
    if (!item.encode2PDUSessionResourceSetupItemSUReq(request)) return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceSetupListSUReq->list, request) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListSUReq::
    decodefromPDUSessionResourceSetupListSUReq(
        Ngap_PDUSessionResourceSetupListSUReq_t*
            pduSessionResourceSetupListSUReq) {
  for (int i = 0; i < pduSessionResourceSetupListSUReq->list.count; i++) {
    PDUSessionResourceSetupItemSUReq item = {};
    if (!item.decodefromPDUSessionResourceSetupItemSUReq(
            pduSessionResourceSetupListSUReq->list.array[i]))
      return false;
    itemSUReqList.push_back(item);
  }

  return true;
}

}  // namespace ngap
