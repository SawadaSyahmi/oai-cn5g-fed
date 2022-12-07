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

/*! \file
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "PDUSessionResourceFailedToSetupListCxtRes.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceFailedToSetupListCxtRes::
    PDUSessionResourceFailedToSetupListCxtRes() {}

//------------------------------------------------------------------------------
PDUSessionResourceFailedToSetupListCxtRes::
    ~PDUSessionResourceFailedToSetupListCxtRes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceFailedToSetupListCxtRes::
    setPDUSessionResourceFailedToSetupListCxtRes(
        const std::vector<PDUSessionResourceFailedToSetupItemCxtRes>& list) {
  itemList = list;
}

//------------------------------------------------------------------------------
void PDUSessionResourceFailedToSetupListCxtRes::
    getPDUSessionResourceFailedToSetupListCxtRes(
        std::vector<PDUSessionResourceFailedToSetupItemCxtRes>& list) {
  list = itemList;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceFailedToSetupListCxtRes::
    encode2PDUSessionResourceFailedToSetupListCxtRes(
        Ngap_PDUSessionResourceFailedToSetupListCxtRes_t*
            pduSessionResourceFailedToSetupListCxtRes) {
  for (std::vector<PDUSessionResourceFailedToSetupItemCxtRes>::iterator it =
           std::begin(itemList);
       it < std::end(itemList); ++it) {
    Ngap_PDUSessionResourceFailedToSetupItemCxtRes_t* failedToResponse =
        (Ngap_PDUSessionResourceFailedToSetupItemCxtRes_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceFailedToSetupItemCxtRes_t));
    if (!failedToResponse) return false;

    if (!it->encode2PDUSessionResourceFailedToSetupItemCxtRes(failedToResponse))
      return false;
    if (ASN_SEQUENCE_ADD(
            &pduSessionResourceFailedToSetupListCxtRes->list,
            failedToResponse) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceFailedToSetupListCxtRes::
    decodefromPDUSessionResourceFailedToSetupListCxtRes(
        Ngap_PDUSessionResourceFailedToSetupListCxtRes_t*
            pduSessionResourceFailedToSetupListCxtRes) {
  itemList.reserve(pduSessionResourceFailedToSetupListCxtRes->list.count);
  for (int i = 0; i < pduSessionResourceFailedToSetupListCxtRes->list.count;
       i++) {
    PDUSessionResourceFailedToSetupItemCxtRes item = {};
    if (!item.decodefromPDUSessionResourceFailedToSetupItemCxtRes(
            pduSessionResourceFailedToSetupListCxtRes->list.array[i]))
      return false;
    itemList.push_back(item);
  }

  return true;
}

}  // namespace ngap
