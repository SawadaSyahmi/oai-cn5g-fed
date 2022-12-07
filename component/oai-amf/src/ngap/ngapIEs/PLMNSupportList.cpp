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

#include "PLMNSupportList.hpp"

using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PLMNSupportList::PLMNSupportList() {}

//------------------------------------------------------------------------------
PLMNSupportList::~PLMNSupportList() {}

//------------------------------------------------------------------------------
bool PLMNSupportList::encode2PLMNSupportList(
    Ngap_PLMNSupportList_t* plmnsupportList) {
  for (std::vector<PLMNSupportItem>::iterator it = std::begin(plmnSupportItems);
       it != std::end(plmnSupportItems); ++it) {
    Ngap_PLMNSupportItem_t* supportItem =
        (Ngap_PLMNSupportItem_t*) calloc(1, sizeof(Ngap_PLMNSupportItem_t));
    if (!supportItem) return false;
    if (!it->encode2PLMNSupportItem(supportItem)) return false;
    if (ASN_SEQUENCE_ADD(&plmnsupportList->list, supportItem) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool PLMNSupportList::decodefromPLMNSupportList(
    Ngap_PLMNSupportList_t* plmnsupportList) {
  for (int i = 0; i < plmnsupportList->list.count; i++) {
    PLMNSupportItem item = {};
    if (!item.decodefromPLMNSupportItem(plmnsupportList->list.array[i]))
      return false;
    plmnSupportItems.push_back(item);
  }

  return true;
}

//------------------------------------------------------------------------------
void PLMNSupportList::addPLMNSupportItems(
    const std::vector<PLMNSupportItem>& items) {
  plmnSupportItems = items;
}

//------------------------------------------------------------------------------
void PLMNSupportList::getPLMNSupportItems(std::vector<PLMNSupportItem>& items) {
  items = plmnSupportItems;
}

//------------------------------------------------------------------------------
void PLMNSupportList::addPLMNSupportItem(const PLMNSupportItem& item) {
  plmnSupportItems.push_back(item);
}

}  // namespace ngap
