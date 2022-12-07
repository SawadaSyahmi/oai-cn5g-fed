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

#include "UEContextReleaseComplete.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}
using namespace ngap;

//------------------------------------------------------------------------------
UEContextReleaseCompleteMsg::UEContextReleaseCompleteMsg() : NgapUEMessage() {
  ies                     = nullptr;
  userLocationInformation = nullptr;
  setMessageType(NgapMessageType::UE_CONTEXT_RELEASE_COMPLETE);
  initialize();
}

//------------------------------------------------------------------------------
UEContextReleaseCompleteMsg::~UEContextReleaseCompleteMsg() {}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::initialize() {
  ies = &(
      ngapPdu->choice.successfulOutcome->value.choice.UEContextReleaseComplete);
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.setAMF_UE_NGAP_ID(id);
  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_AMF_UE_NGAP_ID;
  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.setRanUeNgapId(ran_ue_ngap_id);
  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_RAN_UE_NGAP_ID;
  int ret = ranUeNgapId.encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setUserLocationInfoNR(
    const NrCgi_t& cig, const Tai_t& tai) {
  if (!userLocationInformation)
    userLocationInformation = new UserLocationInformation();

  UserLocationInformationNR* informationNR = new UserLocationInformationNR();
  NR_CGI nR_CGI                            = {};
  nR_CGI.setNR_CGI(cig.mcc, cig.mnc, cig.nrCellID);

  TAI tai_nr = {};
  tai_nr.setTAI(tai);
  informationNR->setInformationNR(nR_CGI, tai_nr);
  userLocationInformation->setInformation(informationNR);

  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_UserLocationInformation;

  int ret = userLocationInformation->encodefromUserLocationInformation(
      &ie->value.choice.UserLocationInformation);
  if (!ret) {
    Logger::ngap().error("Encode NGAP UserLocationInformation IE error");
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UserLocationInformation IE error");
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::getUserLocationInfoNR(
    NrCgi_t& cig, Tai_t& tai) {
  if (userLocationInformation) {
    UserLocationInformationNR* informationNR = new UserLocationInformationNR();
    userLocationInformation->getInformation(informationNR);

    NR_CGI nR_CGI = {};
    TAI tai_nr    = {};
    informationNR->getInformationNR(nR_CGI, tai_nr);
    PlmnId plmnId_cgi             = {};
    NRCellIdentity nRCellIdentity = {};

    nR_CGI.getNR_CGI(plmnId_cgi, nRCellIdentity);
    cig.nrCellID = nRCellIdentity.getNRCellIdentity();
    plmnId_cgi.getMcc(cig.mcc);
    plmnId_cgi.getMnc(cig.mnc);

    PlmnId plmnId = {};
    TAC tac       = {};
    tai_nr.getTAI(plmnId, tac);

    plmnId.getMcc(tai.mcc);
    plmnId.getMnc(tai.mnc);
    tai.tac = tac.getTac() & 0x00ffffff;
  }
}

//------------------------------------------------------------------------------
bool UEContextReleaseCompleteMsg::decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;
  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_UEContextRelease &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_UEContextReleaseComplete) {
      ies = &ngapPdu->choice.successfulOutcome->value.choice
                 .UEContextReleaseComplete;
    } else {
      Logger::ngap().error("Check UEContextReleaseComplete message error");
      return false;
    }
  } else {
    Logger::ngap().error(
        "TypeOfMessage of UEContextReleaseComplete is not SuccessfulOutcome");

    return false;
  }
  for (int i = 0; i < ies->protocolIEs.list.count; i++) {
    switch (ies->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseComplete_IEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  ies->protocolIEs.list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::ngap().error("Decode NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseComplete_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decodefromRAN_UE_NGAP_ID(
                  ies->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
        // TODO: User Location Information
        // TODO: Information on Recommended Cells and RAN Nodes for Paging
        // TODO: PDU Session Resource List
        // TODO: Criticality Diagnostics

      default: {
        Logger::ngap().error("Decoded NGAP message PDU error!");
        return false;
      }
    }
  }

  return true;
}
