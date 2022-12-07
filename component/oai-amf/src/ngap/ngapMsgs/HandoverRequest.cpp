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

#include "HandoverRequest.hpp"
#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
HandoverRequest::HandoverRequest() : NgapMessage() {
  mobilityRestrictionList = nullptr;
  handoverRequestIEs      = nullptr;

  setMessageType(NgapMessageType::HANDOVER_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
HandoverRequest::~HandoverRequest() {}

//------------------------------------------------------------------------------
void HandoverRequest::initialize() {
  handoverRequestIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice.HandoverRequest);
}

//------------------------------------------------------------------------------
unsigned long HandoverRequest::getAmfUeNgapId() {
  return amfUeNgapId.getAMF_UE_NGAP_ID();
}

//------------------------------------------------------------------------------
bool HandoverRequest::decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_HandoverResourceAllocation &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_HandoverRequest) {
      handoverRequestIEs =
          &ngapPdu->choice.initiatingMessage->value.choice.HandoverRequest;
    } else {
      Logger::ngap().error("Check HandoverRequest error");
      return false;
    }
  } else {
    Logger::ngap().error("HandoverRequest MessageType error");
    return false;
  }
  for (int i = 0; i < handoverRequestIEs->protocolIEs.list.count; i++) {
    switch (handoverRequestIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (handoverRequestIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverRequestIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  handoverRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::ngap().error("Decode NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_HandoverType: {
        if (handoverRequestIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverRequestIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestIEs__value_PR_HandoverType) {
          handoverType = handoverRequestIEs->protocolIEs.list.array[i]
                             ->value.choice.HandoverType;
        } else {
          Logger::ngap().error("Decode NGAP Handover Type IE error");
          return false;
        }
      } break;

      default: {
        Logger::ngap().error("Decode NGAP HandoverRequest PDU error");
        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
void HandoverRequest::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.setAMF_UE_NGAP_ID(id);

  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void HandoverRequest::setHandoverType(const long& type)  // 0--intra5gs
{
  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_HandoverType;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_HandoverType;
  ie->value.choice.HandoverType = type;
  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode HandoverType IE error");
}

//------------------------------------------------------------------------------
void HandoverRequest::setCause(
    const Ngap_Cause_PR& causePresent, const long& value) {
  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_Cause;

  cause.setChoiceOfCause(causePresent);
  cause.setValue(value);
  cause.encode2Cause(&(ie->value.choice.Cause));
  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode Cause IE error");
}

//------------------------------------------------------------------------------
void HandoverRequest::setUEAggregateMaximumBitRate(
    const long& bitRateDownlink, const long& bitRateUplink) {
  ueAggregateMaximumBitRate.setUEAggregateMaxBitRate(
      bitRateDownlink, bitRateUplink);

  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverRequestIEs__value_PR_UEAggregateMaximumBitRate;
  ueAggregateMaximumBitRate.encode2UEAggregateMaxBitRate(
      ie->value.choice.UEAggregateMaximumBitRate);

  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode UEAggregateMaximumBitRate IE error");
}

//------------------------------------------------------------------------------
void HandoverRequest::setUESecurityCapabilities(
    const uint16_t& nREncryptionAlgs, const uint16_t& nRIntegrityProtectionAlgs,
    const uint16_t& eUTRAEncryptionAlgs,
    const uint16_t& eUTRAIntegrityProtectionAlgs) {
  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UESecurityCapabilities;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_UESecurityCapabilities;
  ueSecurityCapabilities.setUESecurityCapabilities(
      nREncryptionAlgs, nRIntegrityProtectionAlgs, eUTRAEncryptionAlgs,
      eUTRAIntegrityProtectionAlgs);
  ueSecurityCapabilities.encode2UESecurityCapabilities(
      (ie->value.choice.UESecurityCapabilities));

  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UESecurityCapabilities IE error");
}

//------------------------------------------------------------------------------
void HandoverRequest::setGUAMI(
    const PlmnId& m_plmnId, const AMFRegionID& m_aMFRegionID,
    const AMFSetID& m_aMFSetID, const AMFPointer& m_aMFPointer) {
  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_GUAMI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_GUAMI;
  guami.setGUAMI(m_plmnId, m_aMFRegionID, m_aMFSetID, m_aMFPointer);
  guami.encode2GUAMI(&(ie->value.choice.GUAMI));

  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode GUAMI IE error");
}

//------------------------------------------------------------------------------
void HandoverRequest::setGUAMI(
    const std::string& mcc, const std::string& mnc, const std::string& regionId,
    const std::string& setId, const std::string& pointer) {
  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_GUAMI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_GUAMI;
  guami.setGUAMI(mcc, mnc, regionId, setId, pointer);
  guami.encode2GUAMI(&(ie->value.choice.GUAMI));

  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);

  if (ret != 0) Logger::ngap().error("Encode GUAMI IE error");
}

//------------------------------------------------------------------------------
void HandoverRequest::setAllowedNSSAI(std::vector<S_NSSAI>& list) {
  for (auto& it : list) {
    Ngap_AllowedNSSAI_Item_t* item =
        (Ngap_AllowedNSSAI_Item_t*) calloc(1, sizeof(Ngap_AllowedNSSAI_Item_t));
    it.encode2S_NSSAI(&item->s_NSSAI);
    int ret = ASN_SEQUENCE_ADD(&allowedNSSAI.list, item);
    if (ret != 0)
      Logger::ngap().error(
          "Encode PDUSessionResourceHandoverListItem IE error");
  }
  asn_fprint(stderr, &asn_DEF_Ngap_AllowedNSSAI, &allowedNSSAI);
  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AllowedNSSAI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_AllowedNSSAI;
  ie->value.choice.AllowedNSSAI = allowedNSSAI;
  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AllowedNSSAI IE error");
}

//------------------------------------------------------------------------------
void HandoverRequest::setSecurityContext(const long& count, uint8_t* buffer) {
  SecurityKey securitykey;
  securitykey.setSecurityKey(buffer);
  securitykey.encode2bitstring(securityContext.nextHopNH);
  securityContext.nextHopChainingCount = count;

  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_SecurityContext;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_SecurityContext;
  ie->value.choice.SecurityContext = securityContext;
  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode SecurityContext IE error");
}

//------------------------------------------------------------------------------
void HandoverRequest::setPduSessionResourceSetupList(
    const std::vector<PDUSessionResourceSetupRequestItem_t>& list) {
  PDUSessionResourceSetupItemHOReq* m_pduSessionResourceSetupItemHOReq =
      new PDUSessionResourceSetupItemHOReq[list.size()]();

  for (int i = 0; i < list.size(); i++) {
    PDUSessionID* m_pDUSessionID = new PDUSessionID();
    m_pDUSessionID->setPDUSessionID(list[i].pduSessionId);
    S_NSSAI* m_s_NSSAI = new S_NSSAI();
    m_s_NSSAI->setSst(list[i].s_nssai.sst);
    if (list[i].s_nssai.sd.size()) m_s_NSSAI->setSd(list[i].s_nssai.sd);
    m_pduSessionResourceSetupItemHOReq[i].setPDUSessionResourceSetupItemHOReq(
        m_pDUSessionID, m_s_NSSAI,
        list[i].pduSessionResourceSetupRequestTransfer);
  }

  pDUSessionResourceSetupList.setPDUSessionResourceSetupListHOReq(
      m_pduSessionResourceSetupItemHOReq, list.size());

  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListHOReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverRequestIEs__value_PR_PDUSessionResourceSetupListHOReq;

  int ret = pDUSessionResourceSetupList.encode2PDUSessionResourceSetupListHOReq(
      &ie->value.choice.PDUSessionResourceSetupListHOReq);
  if (!ret) {
    Logger::ngap().error("Encode PDUSessionResourceSetupListSUReq IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode PDUSessionResourceSetupListSUReq IE error");
}

//------------------------------------------------------------------------------
void HandoverRequest::setSourceToTarget_TransparentContainer(
    const OCTET_STRING_t& sourceTotarget) {
  SourceToTarget_TransparentContainer = sourceTotarget;
  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_SourceToTarget_TransparentContainer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverRequestIEs__value_PR_SourceToTarget_TransparentContainer;
  ie->value.choice.SourceToTarget_TransparentContainer = sourceTotarget;
  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode SourceToTarget_TransparentContainer IE error");
}

//------------------------------------------------------------------------------
void HandoverRequest::setMobilityRestrictionList(const PlmnId& m_plmnId) {
  if (!mobilityRestrictionList) {
    mobilityRestrictionList = new MobilityRestrictionList();
  }
  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_MobilityRestrictionList;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_MobilityRestrictionList;
  mobilityRestrictionList->setMobilityRestrictionList(m_plmnId);
  mobilityRestrictionList->encodeMobilityRestrictionList(
      &(ie->value.choice.MobilityRestrictionList));
  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode MobilityRestrictionList IE error");
}

}  // namespace ngap
