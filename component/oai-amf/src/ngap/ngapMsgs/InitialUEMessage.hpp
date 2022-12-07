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

#ifndef _INITIAL_UE_MESSAGE_H_
#define _INITIAL_UE_MESSAGE_H_

#include "FiveGSTmsi.hpp"
#include "NAS-PDU.hpp"
#include "NgapIEsStruct.hpp"
#include "RRCEstablishmentCause.hpp"
#include "UEContextRequest.hpp"
#include "UserLocationInformation.hpp"
#include "NgapMessage.hpp"

namespace ngap {

class InitialUEMessageMsg : public NgapMessage {
 public:
  InitialUEMessageMsg();
  virtual ~InitialUEMessageMsg();

  void initialize();
  bool decodeFromPdu(Ngap_NGAP_PDU_t* ngap_msg_pdu) override;

  void setRanUENgapID(const uint32_t& value);
  bool getRanUENgapID(uint32_t& value);

  void setNasPdu(uint8_t* nas, size_t size);
  bool getNasPdu(uint8_t*& nas, size_t& size);

  void setUserLocationInfoNR(
      const struct NrCgi_s& cig, const struct Tai_s& tai);
  bool getUserLocationInfoNR(struct NrCgi_s& cig, struct Tai_s& tai);

  void setRRCEstablishmentCause(const e_Ngap_RRCEstablishmentCause& cause);
  int getRRCEstablishmentCause();

  // TODO: void set5GS_TMSI(string amfSetId, string amfPointer, string
  // _5g_tmsi);
  bool get5GS_TMSI(std::string& _5GsTmsi);
  bool get5GS_TMSI(
      std ::string& setid, std ::string& pointer, std ::string& tmsi);

  void setUeContextRequest(const e_Ngap_UEContextRequest& ueCtxReq);
  int getUeContextRequest();

 private:
  Ngap_InitialUEMessage_t* initialUEMessageIEs;

  RAN_UE_NGAP_ID ranUeNgapId;                       // Mandatory
  NAS_PDU nasPdu;                                   // Mandatory
  UserLocationInformation userLocationInformation;  // Mandatory
  RRCEstablishmentCause rRCEstablishmentCause;      // Mandatory
  FiveGSTmsi* fivegSTmsi;                           // 5G-S-TMSI (Optional)
  // TODO: AMF Set ID (Optional)
  UEContextRequest* uEContextRequest;  // Optional
  // TODO: Allowed NSSAI (Optional)
  // TODO: Source to Target AMF Information Reroute (Optional)
};

}  // namespace ngap
#endif
