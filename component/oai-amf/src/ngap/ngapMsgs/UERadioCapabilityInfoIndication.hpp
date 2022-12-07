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

#ifndef _UE_RADIO_CAPABILITY_INFO_INDICATION_H_
#define _UE_RADIO_CAPABILITY_INFO_INDICATION_H_

#include "UERadioCapabilityForPaging.hpp"
#include "NgapUEMessage.hpp"

extern "C" {
#include "Ngap_UERadioCapabilityInfoIndication.h"
}

namespace ngap {

class UeRadioCapabilityInfoIndicationMsg : public NgapUEMessage {
 public:
  UeRadioCapabilityInfoIndicationMsg();
  virtual ~UeRadioCapabilityInfoIndicationMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setUERadioCapability(uint8_t* buf, size_t size);
  bool getUERadioCapability(uint8_t*& buf, size_t& size);

  void setUERadioCapability(const OCTET_STRING_t& capability);
  void getUERadioCapability(OCTET_STRING_t& capability);

  void setUERadioCapabilityForPaging(
      uint8_t* nr, size_t sizeofnr, uint8_t* eutra, size_t sizeofeutra);
  bool getUERadioCapabilityForPaging(
      uint8_t*& nr, size_t& sizeofnr, uint8_t*& eutra, size_t& sizeofeutra);

 private:
  Ngap_UERadioCapabilityInfoIndication_t* ueRadioCapabilityInfoIndicationIEs;
  // AMF_UE_NGAP_ID //Mandatory
  // RAN_UE_NGAP_ID //Mandatory
  OCTET_STRING_t ueRadioCapability;                        // Mandatory
  UERadioCapabilityForPaging* ueRadioCapabilityForPaging;  // Optional
};

}  // namespace ngap
#endif
