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

#ifndef _TAI_H_
#define _TAI_H_

#include "PlmnId.hpp"
#include "Tac.hpp"
#include "NgapIEsStruct.hpp"

extern "C" {
#include "Ngap_TAI.h"
}

namespace ngap {

class TAI {
 public:
  TAI();
  virtual ~TAI();

  void setTAI(const PlmnId&, const TAC&);
  void setTAI(
      const std::string& mcc, const std::string& mnc, const uint32_t& tac);
  void getTAI(std::string& mcc, std::string& mnc, uint32_t& tac);

  void setTAI(const Tai_t& tai);
  void getTAI(Tai_t& tai);

  bool encode2TAI(Ngap_TAI_t*);
  bool decodefromTAI(Ngap_TAI_t*);
  void getTAI(PlmnId&, TAC&);

 private:
  PlmnId plmnId;  // Mandatory
  TAC tac;        // Mandatory
};
}  // namespace ngap

#endif
