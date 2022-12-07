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

#ifndef _GLOBAL_RAN_NODE_ID_H_
#define _GLOBAL_RAN_NODE_ID_H_

extern "C" {
#include "Ngap_GlobalRANNodeID.h"
}

#include "GlobalgNBId.hpp"

namespace ngap {

class GlobalRanNodeId {
 public:
  GlobalRanNodeId();
  virtual ~GlobalRanNodeId();

  void setChoiceOfRanNodeId(const Ngap_GlobalRANNodeID_PR& m_idPresent);
  Ngap_GlobalRANNodeID_PR getChoiceOfRanNodeId();

  void setGlobalgNBID(const GlobalgNBId&);
  void getGlobalgNBID(GlobalgNBId&);

  bool encode2GlobalRANNodeID(Ngap_GlobalRANNodeID_t*);
  bool decodefromGlobalRANNodeID(Ngap_GlobalRANNodeID_t*);

 private:
  GlobalgNBId globalgNBId;  // Mandatory
  Ngap_GlobalRANNodeID_PR idPresent;
};

}  // namespace ngap

#endif
