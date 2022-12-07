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

#ifndef _INITIAL_CONTEXT_SETUP_REQUEST_H_
#define _INITIAL_CONTEXT_SETUP_REQUEST_H_

#include "AMFName.hpp"
#include "AllowedNssai.hpp"
#include "CoreNetworkAssistanceInformation.hpp"
#include "GUAMI.hpp"
#include "PDUSessionResourceSetupListCxtReq.hpp"
#include "SecurityKey.hpp"
#include "UEAggregateMaxBitRate.hpp"
#include "UERadioCapability.hpp"
#include "UESecurityCapabilities.hpp"
#include "NgapUEMessage.hpp"

extern "C" {
#include "Ngap_InitialContextSetupRequest.h"
}

namespace ngap {

class InitialContextSetupRequestMsg : public NgapUEMessage {
 public:
  InitialContextSetupRequestMsg();
  virtual ~InitialContextSetupRequestMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setOldAmfName(const std::string& name);
  bool getOldAmfName(std::string& name);

  void setUEAggregateMaxBitRate(
      const long& bit_rate_downlink, const long& bit_rate_uplink);
  bool getUEAggregateMaxBitRate(long& bit_rate_downlink, long& bit_rate_uplink);

  void setCoreNetworkAssistanceInfo(
      const uint16_t& ueIdentityIndexValue /*10bits*/,
      const e_Ngap_PagingDRX& ueSpecificDrx,
      const uint8_t& periodicRegUpdateTimer, const bool& micoModeInd,
      const std::vector<Tai_t>& taiListForRRcInactive);

  bool getCoreNetworkAssistanceInfo(
      uint16_t& ueIdentityIndexValue /*10bits*/, int& ueSpecificDrx,
      uint8_t& periodicRegUpdateTimer, bool& micoModeInd,
      std::vector<Tai_t>& taiListForRRcInactive);

  void setGuami(const Guami_t& value);
  bool getGuami(Guami_t& value);

  void setPduSessionResourceSetupRequestList(
      const std::vector<PDUSessionResourceSetupRequestItem_t>& list);
  bool getPduSessionResourceSetupRequestList(
      std::vector<PDUSessionResourceSetupRequestItem_t>& list);

  void setAllowedNssai(const std::vector<S_Nssai>& list);
  bool getAllowedNssai(std::vector<S_Nssai>& list);

  void setUESecurityCapability(
      const uint16_t& NR_EncryptionAlgs,
      const uint16_t& NR_IntegrityProtectionAlgs,
      const uint16_t& E_UTRA_EncryptionAlgs,
      const uint16_t& E_UTRA_IntegrityProtectionAlgs);
  bool getUESecurityCapability(
      uint16_t& NR_EncryptionAlgs, uint16_t& NR_IntegrityProtectionAlgs,
      uint16_t& E_UTRA_EncryptionAlgs,
      uint16_t& E_UTRA_IntegrityProtectionAlgs);

  void setSecurityKey(uint8_t* key);   // 256bits
  bool getSecurityKey(uint8_t*& key);  // 256bits

  void setNasPdu(uint8_t* nas, size_t size);
  bool getNasPdu(uint8_t*& nas, size_t& size);

  void setUERadioCapability(uint8_t* buf, size_t size);

 private:
  Ngap_InitialContextSetupRequest_t* initialContextSetupRequestIEs;

  AmfName* oldAmfName;                                   // Optional
  UEAggregateMaxBitRate* uEAggregateMaxBitRate;          // Conditional
  CoreNetworkAssistanceInfo* coreNetworkAssistanceInfo;  // Optional
  GUAMI guami;                                           // Mandatory
  PDUSessionResourceSetupListCxtReq*
      pduSessionResourceSetupRequestList;         // Optional
  AllowedNSSAI allowedNssai;                      // Mandatory
  UESecurityCapabilities uESecurityCapabilities;  // Mandatory
  SecurityKey securityKey;                        // Mandatory
  UERadioCapability* ueRadioCapability;           // Optional
  NAS_PDU* nasPdu;                                // Optional
  // TODO: Trace Activation (Optional)
  // TODO: Mobility Restriction List
  // TODO: Index to RAT/Frequency Selection Priority
  // TODO: Masked IMEISV
  // TODO: Emergency Fallback Indicator
  // TODO: RRC Inactive Transition Report Request
  // TODO: UE Radio Capability for Paging
  // TODO: Redirection for Voice EPS Fallback
  // TODO: Location Reporting Request Type
  // TODO: CN Assisted RAN Parameters Tuning
};

}  // namespace ngap
#endif
