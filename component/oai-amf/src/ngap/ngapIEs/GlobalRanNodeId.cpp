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

#include "GlobalRanNodeId.hpp"
#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
GlobalRanNodeId::GlobalRanNodeId() {}

//------------------------------------------------------------------------------
GlobalRanNodeId::~GlobalRanNodeId() {}

//------------------------------------------------------------------------------
void GlobalRanNodeId::setChoiceOfRanNodeId(
    const Ngap_GlobalRANNodeID_PR& m_idPresent) {
  idPresent = m_idPresent;
}

//------------------------------------------------------------------------------
Ngap_GlobalRANNodeID_PR GlobalRanNodeId::getChoiceOfRanNodeId() {
  return idPresent;
}

//------------------------------------------------------------------------------
void GlobalRanNodeId::setGlobalgNBID(const GlobalgNBId& m_globalgNBId) {
  globalgNBId = m_globalgNBId;
}

//------------------------------------------------------------------------------
void GlobalRanNodeId::getGlobalgNBID(GlobalgNBId& id) {
  id = globalgNBId;
}

//------------------------------------------------------------------------------
bool GlobalRanNodeId::encode2GlobalRANNodeID(
    Ngap_GlobalRANNodeID_t* globalRANNodeID) {
  globalRANNodeID->present = idPresent;

  switch (idPresent) {
    case Ngap_GlobalRANNodeID_PR_globalGNB_ID: {
      Ngap_GlobalGNB_ID_t* globalGNB_ID =
          (Ngap_GlobalGNB_ID_t*) calloc(1, sizeof(struct Ngap_GlobalGNB_ID));
      if (!globalGNB_ID) return false;
      if (!globalgNBId.encode2GlobalgNBId(globalGNB_ID)) return false;
      globalRANNodeID->choice.globalGNB_ID = globalGNB_ID;
      break;
    }
    case Ngap_GlobalRANNodeID_PR_globalNgENB_ID: {
      break;
    }
    case Ngap_GlobalRANNodeID_PR_globalN3IWF_ID: {
      break;
    }
    default:
      Logger::ngap().warn("GlobalRanNodeId Present encode error!");
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool GlobalRanNodeId::decodefromGlobalRANNodeID(
    Ngap_GlobalRANNodeID_t* globalRANNodeID) {
  idPresent = globalRANNodeID->present;
  switch (idPresent) {
    case Ngap_GlobalRANNodeID_PR_globalGNB_ID: {
      if (!globalgNBId.decodefromGlobalgNBId(
              globalRANNodeID->choice.globalGNB_ID))
        return false;
      break;
    }
    case Ngap_GlobalRANNodeID_PR_globalNgENB_ID: {
      Logger::ngap().warn("GlobalRANNodeID Present is globalNgENB!");
      break;
    }
    case Ngap_GlobalRANNodeID_PR_globalN3IWF_ID: {
      Logger::ngap().warn("GlobalRANNodeID Present is globalN3IWF!");
      break;
    }
    default:
      Logger::ngap().warn("GlobalRanNodeId Present decode error!");
      return false;
  }
  return true;
}
}  // namespace ngap
