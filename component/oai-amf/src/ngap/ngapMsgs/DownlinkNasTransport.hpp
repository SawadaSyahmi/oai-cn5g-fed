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

#ifndef _DOWNLINK_NAS_TRANSPORT_H_
#define _DOWNLINK_NAS_TRANSPORT_H_

#include "AMFName.hpp"
#include "IndexToRFSP.hpp"
#include "NAS-PDU.hpp"
#include "RANPagingPriority.hpp"
#include "NgapUEMessage.hpp"

namespace ngap {

class DownLinkNasTransportMsg : public NgapUEMessage {
 public:
  DownLinkNasTransportMsg();
  virtual ~DownLinkNasTransportMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setOldAmfName(const std::string name);
  bool getOldAmfName(std::string& name);

  void setRanPagingPriority(uint8_t pagingPriority);  // 1~256
  uint8_t getRanPagingPriority();

  void setNasPdu(uint8_t* nas, size_t sizeofnas);
  bool getNasPdu(uint8_t*& nas, size_t& sizeofnas);

  void setIndex2Rat_Frequency_SelectionPriority(uint8_t value);  // 1~256
  uint8_t getIndex2Rat_Frequency_SelectionPriority();

 private:
  Ngap_DownlinkNASTransport_t* downLinkNasTransportIEs;

  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  AmfName* oldAmfName;                   // Optional
  RANPagingPriority* ranPagingPriority;  // Optional
  NAS_PDU nasPdu;                        // Mandatory
  // TODO: Mobility Restriction List (Optional)
  IndexToRFSP*
      indexToRFSP;  // Index to RAT/Frequency Selection Priority (Optional)
  // TODO: UE Aggregate Maximum Bit Rate (Optional)
  // TODO: Allowed NSSAI (Optional)
};

}  // namespace ngap
#endif
