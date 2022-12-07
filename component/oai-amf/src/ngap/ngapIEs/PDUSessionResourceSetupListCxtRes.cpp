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

#include "PDUSessionResourceSetupListCxtRes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceSetupListCxtRes::PDUSessionResourceSetupListCxtRes() {}

//------------------------------------------------------------------------------
PDUSessionResourceSetupListCxtRes::~PDUSessionResourceSetupListCxtRes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListCxtRes::setPDUSessionResourceSetupListCxtRes(
    const std::vector<PDUSessionResourceSetupItemCxtRes>& list) {
  itemList = list;
}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListCxtRes::getPDUSessionResourceSetupListCxtRes(
    std::vector<PDUSessionResourceSetupItemCxtRes>& list) {
  list = itemList;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListCxtRes::
    encode2PDUSessionResourceSetupListCxtRes(
        Ngap_PDUSessionResourceSetupListCxtRes_t*
            pduSessionResourceSetupListCxtRes) {
  for (std::vector<PDUSessionResourceSetupItemCxtRes>::iterator it =
           std::begin(itemList);
       it < std::end(itemList); ++it) {
    Ngap_PDUSessionResourceSetupItemCxtRes_t* response =
        (Ngap_PDUSessionResourceSetupItemCxtRes_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceSetupItemCxtRes_t));
    if (!response) return false;
    if (!it->encode2PDUSessionResourceSetupItemCxtRes(response)) return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceSetupListCxtRes->list, response) !=
        0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListCxtRes::
    decodefromPDUSessionResourceSetupListCxtRes(
        Ngap_PDUSessionResourceSetupListCxtRes_t*
            pduSessionResourceSetupListCxtRes) {
  itemList.reserve(pduSessionResourceSetupListCxtRes->list.count);
  for (int i = 0; i < pduSessionResourceSetupListCxtRes->list.count; i++) {
    PDUSessionResourceSetupItemCxtRes item = {};
    if (!item.decodefromPDUSessionResourceSetupItemCxtRes(
            pduSessionResourceSetupListCxtRes->list.array[i]))
      return false;
    itemList.push_back(item);
  }

  return true;
}

}  // namespace ngap
