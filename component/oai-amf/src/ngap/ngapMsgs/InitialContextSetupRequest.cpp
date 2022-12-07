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

#include "InitialContextSetupRequest.hpp"
#include "logger.hpp"
#include "amf.hpp"
#include "conversions.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
InitialContextSetupRequestMsg::InitialContextSetupRequestMsg()
    : NgapUEMessage() {
  initialContextSetupRequestIEs      = nullptr;
  oldAmfName                         = nullptr;
  uEAggregateMaxBitRate              = nullptr;
  coreNetworkAssistanceInfo          = nullptr;
  pduSessionResourceSetupRequestList = nullptr;
  nasPdu                             = nullptr;
  ueRadioCapability                  = nullptr;

  setMessageType(NgapMessageType::INITIAL_CONTEXT_SETUP_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
InitialContextSetupRequestMsg::~InitialContextSetupRequestMsg() {}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::initialize() {
  initialContextSetupRequestIEs = &(ngapPdu->choice.initiatingMessage->value
                                        .choice.InitialContextSetupRequest);
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.setAMF_UE_NGAP_ID(id);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.setRanUeNgapId(ran_ue_ngap_id);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setOldAmfName(const std::string& name) {
  if (!oldAmfName) oldAmfName = new AmfName();
  oldAmfName->setValue(name);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_OldAMF;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_AMFName;

  int ret = oldAmfName->encode2AmfName(&ie->value.choice.AMFName);
  if (!ret) {
    Logger::ngap().error("Encode oldAmfName IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode oldAmfName IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUEAggregateMaxBitRate(
    const long& bit_rate_downlink, const long& bit_rate_uplink) {
  if (!uEAggregateMaxBitRate)
    uEAggregateMaxBitRate = new UEAggregateMaxBitRate();

  uEAggregateMaxBitRate->setUEAggregateMaxBitRate(
      bit_rate_downlink, bit_rate_uplink);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

  int ret = uEAggregateMaxBitRate->encode2UEAggregateMaxBitRate(
      ie->value.choice.UEAggregateMaximumBitRate);
  if (!ret) {
    Logger::ngap().error("Encode UEAggregateMaxBitRate IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UEAggregateMaxBitRate IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setCoreNetworkAssistanceInfo(
    const uint16_t& ueIdentityIndexValue /*10bits*/,
    const e_Ngap_PagingDRX& ueSpecificDrx,
    const uint8_t& periodicRegUpdateTimer, const bool& micoModeInd,
    const std::vector<Tai_t>& taiListForRRcInactive) {
  if (!coreNetworkAssistanceInfo)
    coreNetworkAssistanceInfo = new CoreNetworkAssistanceInfo();

  UEIdentityIndexValue m_ueIdentityIndexValue = {};
  m_ueIdentityIndexValue.setUEIdentityIndexValue(ueIdentityIndexValue);
  DefaultPagingDRX* m_pagingDRX = new DefaultPagingDRX();
  m_pagingDRX->setValue(ueSpecificDrx);
  PeriodicRegistrationUpdateTimer m_periodicRegUpdateTimer = {};
  m_periodicRegUpdateTimer.setPeriodicRegistrationUpdateTimer(
      periodicRegUpdateTimer);

  std::vector<TAI> taiList;
  for (int i = 0; i < taiListForRRcInactive.size(); i++) {
    TAI m_tai = {};
    m_tai.setTAI(
        taiListForRRcInactive[i].mcc, taiListForRRcInactive[i].mnc,
        taiListForRRcInactive[i].tac);
    taiList.push_back(m_tai);
  }

  coreNetworkAssistanceInfo->setCoreNetworkAssistanceInfo(
      m_ueIdentityIndexValue, m_pagingDRX, m_periodicRegUpdateTimer,
      micoModeInd, taiList);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_CoreNetworkAssistanceInformation;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_CoreNetworkAssistanceInformation;

  int ret = coreNetworkAssistanceInfo->encode2CoreNetworkAssistanceInfo(
      &ie->value.choice.CoreNetworkAssistanceInformation);
  if (!ret) {
    Logger::ngap().error("Encode CoreNetworkAssistanceInformation IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode CoreNetworkAssistanceInformation IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setGuami(const Guami_t& value) {
  guami.setGUAMI(
      value.mcc, value.mnc, value.regionID, value.AmfSetID, value.AmfPointer);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_GUAMI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_GUAMI;

  int ret = guami.encode2GUAMI(&ie->value.choice.GUAMI);
  if (!ret) {
    Logger::ngap().error("Encode GUAMI IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode GUAMI IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setPduSessionResourceSetupRequestList(
    const std::vector<PDUSessionResourceSetupRequestItem_t>& list) {
  if (!pduSessionResourceSetupRequestList)
    pduSessionResourceSetupRequestList =
        new PDUSessionResourceSetupListCxtReq();

  std::vector<PDUSessionResourceSetupItemCxtReq>
      pduSessionResourceSetupItemCxtReqList;
  pduSessionResourceSetupItemCxtReqList.reserve(list.size());

  for (int i = 0; i < list.size(); i++) {
    PDUSessionResourceSetupItemCxtReq pduSessionResourceSetupItemCxtReq = {};
    PDUSessionID pDUSessionID                                           = {};
    pDUSessionID.setPDUSessionID(list[i].pduSessionId);
    NAS_PDU* nAS_PDU = nullptr;
    if (list[i].pduSessionNAS_PDU) {
      nAS_PDU = new NAS_PDU();
      nAS_PDU->setNasPdu(
          list[i].pduSessionNAS_PDU, list[i].sizeofpduSessionNAS_PDU);
    }
    S_NSSAI s_NSSAI = {};
    s_NSSAI.setSst(list[i].s_nssai.sst);
    if (list[i].s_nssai.sd.size()) s_NSSAI.setSd(list[i].s_nssai.sd);
    pduSessionResourceSetupItemCxtReq.setPDUSessionResourceSetupItemCxtReq(
        pDUSessionID, nAS_PDU, s_NSSAI,
        list[i].pduSessionResourceSetupRequestTransfer);
    pduSessionResourceSetupItemCxtReqList.push_back(
        pduSessionResourceSetupItemCxtReq);
  }

  pduSessionResourceSetupRequestList->setPDUSessionResourceSetupListCxtReq(
      pduSessionResourceSetupItemCxtReqList);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_PDUSessionResourceSetupListCxtReq;

  int ret = pduSessionResourceSetupRequestList
                ->encode2PDUSessionResourceSetupListCxtReq(
                    &ie->value.choice.PDUSessionResourceSetupListCxtReq);
  if (!ret) {
    Logger::ngap().error("Encode PDUSessionResourceSetupListCxtReq IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode PDUSessionResourceSetupListCxtReq IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setAllowedNssai(
    const std::vector<S_Nssai>& list) {
  std::vector<S_NSSAI> snssaiList;

  for (int i = 0; i < list.size(); i++) {
    S_NSSAI snssai = {};
    snssai.setSst(list[i].sst);
    uint32_t sd = SD_NO_VALUE;
    if (!list[i].sd.empty()) {
      conv::sd_string_to_int(list[i].sd, sd);
    }
    snssai.setSd(sd);
    snssaiList.push_back(snssai);
  }
  allowedNssai.setAllowedNSSAI(snssaiList);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AllowedNSSAI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_AllowedNSSAI;

  int ret = allowedNssai.encode2AllowedNSSAI(&ie->value.choice.AllowedNSSAI);
  if (!ret) {
    Logger::ngap().error("Encode AllowedNSSAI IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AllowedNSSAI IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUESecurityCapability(
    const uint16_t& NR_EncryptionAlgs,
    const uint16_t& NR_IntegrityProtectionAlgs,
    const uint16_t& E_UTRA_EncryptionAlgs,
    const uint16_t& E_UTRA_IntegrityProtectionAlgs) {
  uESecurityCapabilities.setUESecurityCapabilities(
      NR_EncryptionAlgs, NR_IntegrityProtectionAlgs, E_UTRA_EncryptionAlgs,
      E_UTRA_IntegrityProtectionAlgs);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UESecurityCapabilities;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

  int ret = uESecurityCapabilities.encode2UESecurityCapabilities(
      ie->value.choice.UESecurityCapabilities);
  if (!ret) {
    Logger::ngap().error("Encode UESecurityCapabilities IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UESecurityCapabilities IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setSecurityKey(uint8_t* key) {
  securityKey.setSecurityKey(key);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_SecurityKey;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_SecurityKey;

  int ret = securityKey.encode2bitstring(ie->value.choice.SecurityKey);
  if (!ret) {
    Logger::ngap().error("Encode SecurityKey IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode SecurityKey IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setNasPdu(uint8_t* nas, size_t size) {
  if (!nasPdu) nasPdu = new NAS_PDU();

  nasPdu->setNasPdu(nas, size);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_NAS_PDU;

  int ret = nasPdu->encode2octetstring(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NAS PDU error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NAS PDU error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUERadioCapability(
    uint8_t* buffer, size_t size) {
  if (!ueRadioCapability) ueRadioCapability = new UERadioCapability();

  ueRadioCapability->setUERadioCapability(buffer, size);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UERadioCapability;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

  int ret = ueRadioCapability->encode2UERadioCapability(
      ie->value.choice.UERadioCapability);
  if (!ret) {
    Logger::ngap().error("Encode UERadioCapability IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UERadioCapability IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_InitialContextSetup &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_InitialContextSetupRequest) {
      initialContextSetupRequestIEs = &ngapPdu->choice.initiatingMessage->value
                                           .choice.InitialContextSetupRequest;
    } else {
      Logger::ngap().error("Check InitialContextSetupRequest message error!");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < initialContextSetupRequestIEs->protocolIEs.list.count;
       i++) {
    switch (initialContextSetupRequestIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decodefromRAN_UE_NGAP_ID(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_OldAMF: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_AMFName) {
          oldAmfName = new AmfName();
          if (!oldAmfName->decodefromAmfName(
                  &initialContextSetupRequestIEs->protocolIEs.list.array[i]
                       ->value.choice.AMFName)) {
            Logger::ngap().error("Decoded NGAP OldAMFName IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP OldAMFName IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate) {
          uEAggregateMaxBitRate = new UEAggregateMaxBitRate();
          if (!uEAggregateMaxBitRate->decodefromUEAggregateMaxBitRate(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.UEAggregateMaximumBitRate)) {
            Logger::ngap().error(
                "Decoded NGAP UEAggregateMaximumBitRate IE error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP UEAggregateMaximumBitRate IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CoreNetworkAssistanceInformation: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_CoreNetworkAssistanceInformation) {
          coreNetworkAssistanceInfo = new CoreNetworkAssistanceInfo();
          if (!coreNetworkAssistanceInfo->decodefromCoreNetworkAssistanceInfo(
                  &initialContextSetupRequestIEs->protocolIEs.list.array[i]
                       ->value.choice.CoreNetworkAssistanceInformation)) {
            Logger::ngap().error(
                "Decoded NGAP CoreNetworkAssistanceInformation IE error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP CoreNetworkAssistanceInformation IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_GUAMI: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_GUAMI) {
          if (!guami.decodefromGUAMI(
                  &initialContextSetupRequestIEs->protocolIEs.list.array[i]
                       ->value.choice.GUAMI)) {
            Logger::ngap().error("Decoded NGAP GUAMI IE error");

            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP GUAMI IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtReq: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_PDUSessionResourceSetupListCxtReq) {
          pduSessionResourceSetupRequestList =
              new PDUSessionResourceSetupListCxtReq();
          if (!pduSessionResourceSetupRequestList
                   ->decodefromPDUSessionResourceSetupListCxtReq(
                       &initialContextSetupRequestIEs->protocolIEs.list
                            .array[i]
                            ->value.choice.PDUSessionResourceSetupListCxtReq)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceSetupListCxtReq IE error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceSetupListCxtReq IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_AllowedNSSAI: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_AllowedNSSAI) {
          if (!allowedNssai.decodefromAllowedNSSAI(
                  &initialContextSetupRequestIEs->protocolIEs.list.array[i]
                       ->value.choice.AllowedNSSAI)) {
            Logger::ngap().error("Decoded NGAP AllowedNSSAI IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP AllowedNSSAI IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UESecurityCapabilities: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities) {
          if (!uESecurityCapabilities.decodefromUESecurityCapabilities(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.UESecurityCapabilities)) {
            Logger::ngap().error(
                "Decoded NGAP UESecurityCapabilities IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP UESecurityCapabilities IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_SecurityKey: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_SecurityKey) {
          if (!securityKey.decodefrombitstring(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.SecurityKey)) {
            Logger::ngap().error("Decoded NGAP SecurityKey IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP SecurityKey IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_NAS_PDU) {
          nasPdu = new NAS_PDU();
          if (!nasPdu->decodefromoctetstring(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.NAS_PDU)) {
            Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
          return false;
        }
      } break;

      default: {
        Logger::ngap().error("Decoded NGAP Message PDU error");
        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getOldAmfName(std::string& name) {
  if (!oldAmfName) return false;
  oldAmfName->getValue(name);
  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getUEAggregateMaxBitRate(
    long& bit_rate_downlink, long& bit_rate_uplink) {
  if (!uEAggregateMaxBitRate) return false;
  return uEAggregateMaxBitRate->getUEAggregateMaxBitRate(
      bit_rate_downlink, bit_rate_uplink);
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getCoreNetworkAssistanceInfo(
    uint16_t& ueIdentityIndexValue /*10bits*/, int& ueSpecificDrx /*-1*/,
    uint8_t& periodicRegUpdateTimer, bool& micoModeInd,
    std::vector<Tai_t>& taiListForRRcInactive) {
  if (!coreNetworkAssistanceInfo) return false;
  UEIdentityIndexValue m_ueIdentityIndexValue              = {};
  DefaultPagingDRX* m_pagingDRX                            = nullptr;
  PeriodicRegistrationUpdateTimer m_periodicRegUpdateTimer = {};

  std::vector<TAI> taiList;

  coreNetworkAssistanceInfo->getCoreNetworkAssistanceInfo(
      m_ueIdentityIndexValue, m_pagingDRX, m_periodicRegUpdateTimer,
      micoModeInd, taiList);
  m_ueIdentityIndexValue.getUEIdentityIndexValue(ueIdentityIndexValue);
  if (m_pagingDRX)
    ueSpecificDrx = m_pagingDRX->getValue();
  else
    ueSpecificDrx = -1;
  m_periodicRegUpdateTimer.getPeriodicRegistrationUpdateTimer(
      periodicRegUpdateTimer);

  for (std::vector<TAI>::iterator it = std::begin(taiList);
       it < std::end(taiList); ++it) {
    Tai_t tai = {};
    it->getTAI(tai);
    taiListForRRcInactive.push_back(tai);
  }
  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getGuami(Guami_t& value) {
  guami.getGUAMI(
      value.mcc, value.mnc, value.regionID, value.AmfSetID, value.AmfPointer);
  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getPduSessionResourceSetupRequestList(
    std::vector<PDUSessionResourceSetupRequestItem_t>& list) {
  if (!pduSessionResourceSetupRequestList) return false;

  std::vector<PDUSessionResourceSetupItemCxtReq>
      pduSessionResourceSetupItemCxtReqList;
  pduSessionResourceSetupRequestList->getPDUSessionResourceSetupListCxtReq(
      pduSessionResourceSetupItemCxtReqList);

  for (std::vector<PDUSessionResourceSetupItemCxtReq>::iterator it =
           std::begin(pduSessionResourceSetupItemCxtReqList);
       it < std::end(pduSessionResourceSetupItemCxtReqList); ++it) {
    PDUSessionResourceSetupRequestItem_t request = {};

    PDUSessionID pDUSessionID = {};
    NAS_PDU* nAS_PDU          = nullptr;
    S_NSSAI s_NSSAI           = {};
    it->getPDUSessionResourceSetupItemCxtReq(
        pDUSessionID, nAS_PDU, s_NSSAI,
        request.pduSessionResourceSetupRequestTransfer);
    pDUSessionID.getPDUSessionID(request.pduSessionId);
    s_NSSAI.getSst(request.s_nssai.sst);
    s_NSSAI.getSd(request.s_nssai.sd);
    if (nAS_PDU) {
      nAS_PDU->getNasPdu(
          request.pduSessionNAS_PDU, request.sizeofpduSessionNAS_PDU);
    } else {
      request.pduSessionNAS_PDU       = nullptr;
      request.sizeofpduSessionNAS_PDU = 0;
    }

    list.push_back(request);
  }

  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getAllowedNssai(
    std::vector<S_Nssai>& list) {
  std::vector<S_NSSAI> snssaiList;
  allowedNssai.getAllowedNSSAI(snssaiList);
  for (std::vector<S_NSSAI>::iterator it = std::begin(snssaiList);
       it < std::end(snssaiList); ++it) {
    S_Nssai s_nssai = {};
    it->getSst(s_nssai.sst);
    it->getSd(s_nssai.sd);
    list.push_back(s_nssai);
  }

  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getUESecurityCapability(
    uint16_t& NR_EncryptionAlgs, uint16_t& NR_IntegrityProtectionAlgs,
    uint16_t& E_UTRA_EncryptionAlgs, uint16_t& E_UTRA_IntegrityProtectionAlgs) {
  if (!uESecurityCapabilities.getUESecurityCapabilities(
          NR_EncryptionAlgs, NR_IntegrityProtectionAlgs, E_UTRA_EncryptionAlgs,
          E_UTRA_IntegrityProtectionAlgs))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getSecurityKey(uint8_t*& key) {
  if (!securityKey.getSecurityKey(key)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getNasPdu(
    uint8_t*& nas, size_t& sizeofnas) {
  if (!nasPdu) return false;
  if (!nasPdu->getNasPdu(nas, sizeofnas)) return false;

  return true;
}

}  // namespace ngap
