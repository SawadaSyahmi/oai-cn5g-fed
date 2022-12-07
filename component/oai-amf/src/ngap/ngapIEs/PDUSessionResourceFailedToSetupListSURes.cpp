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

#include "PDUSessionResourceFailedToSetupListSURes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceFailedToSetupListSURes::
    PDUSessionResourceFailedToSetupListSURes() {}

//------------------------------------------------------------------------------
PDUSessionResourceFailedToSetupListSURes::
    ~PDUSessionResourceFailedToSetupListSURes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceFailedToSetupListSURes::
    setPDUSessionResourceFailedToSetupListSURes(
        const std::vector<PDUSessionResourceFailedToSetupItemSURes>& list) {
  itemSUResList = list;
}

//------------------------------------------------------------------------------
void PDUSessionResourceFailedToSetupListSURes::
    getPDUSessionResourceFailedToSetupListSURes(
        std::vector<PDUSessionResourceFailedToSetupItemSURes>& list) {
  list = itemSUResList;
}
//------------------------------------------------------------------------------
bool PDUSessionResourceFailedToSetupListSURes::
    encode2PDUSessionResourceFailedToSetupListSURes(
        Ngap_PDUSessionResourceFailedToSetupListSURes_t*
            pduSessionResourceFailedToSetupListSURes) {
  for (auto& item : itemSUResList) {
    Ngap_PDUSessionResourceFailedToSetupItemSURes_t* failedToResponse =
        (Ngap_PDUSessionResourceFailedToSetupItemSURes_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceFailedToSetupItemSURes_t));
    if (!failedToResponse) return false;
    if (!item.encode2PDUSessionResourceFailedToSetupItemSURes(failedToResponse))
      return false;
    if (ASN_SEQUENCE_ADD(
            &pduSessionResourceFailedToSetupListSURes->list,
            failedToResponse) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceFailedToSetupListSURes::
    decodefromPDUSessionResourceFailedToSetupListSURes(
        Ngap_PDUSessionResourceFailedToSetupListSURes_t*
            pduSessionResourceFailedToSetupListSURes) {
  for (int i = 0; i < pduSessionResourceFailedToSetupListSURes->list.count;
       i++) {
    PDUSessionResourceFailedToSetupItemSURes item = {};
    if (!item.decodefromPDUSessionResourceFailedToSetupItemSURes(
            pduSessionResourceFailedToSetupListSURes->list.array[i]))
      return false;
    itemSUResList.push_back(item);
  }

  return true;
}

}  // namespace ngap
