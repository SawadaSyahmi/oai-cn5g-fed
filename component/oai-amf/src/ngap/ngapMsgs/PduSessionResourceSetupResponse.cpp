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

#include "PduSessionResourceSetupResponse.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupResponseMsg::PduSessionResourceSetupResponseMsg()
    : NgapUEMessage() {
  pduSessionResourceSetupResponseIEs          = nullptr;
  pduSessionResourceSetupResponseList         = nullptr;
  pduSessionResourceFailedToSetupResponseList = nullptr;

  setMessageType(NgapMessageType::PDU_SESSION_RESOURCE_SETUP_RESPONSE);
  initialize();
}

//------------------------------------------------------------------------------
PduSessionResourceSetupResponseMsg::~PduSessionResourceSetupResponseMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::initialize() {
  pduSessionResourceSetupResponseIEs =
      &(ngapPdu->choice.successfulOutcome->value.choice
            .PDUSessionResourceSetupResponse);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setAmfUeNgapId(
    const unsigned long& id) {
  amfUeNgapId.setAMF_UE_NGAP_ID(id);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.setRanUeNgapId(ran_ue_ngap_id);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setPduSessionResourceSetupResponseList(
    const std::vector<PDUSessionResourceSetupResponseItem_t>& list) {
  if (!pduSessionResourceSetupResponseList)
    pduSessionResourceSetupResponseList =
        new PDUSessionResourceSetupListSURes();

  std::vector<PDUSessionResourceSetupItemSURes> itemSUResList;

  for (int i = 0; i < list.size(); i++) {
    PDUSessionResourceSetupItemSURes itemSURes = {};
    PDUSessionID pDUSessionID                  = {};
    pDUSessionID.setPDUSessionID(list[i].pduSessionId);

    itemSURes.setPDUSessionResourceSetupItemSURes(
        pDUSessionID, list[i].pduSessionResourceSetupResponseTransfer);
    itemSUResList.push_back(itemSURes);
  }

  pduSessionResourceSetupResponseList->setPDUSessionResourceSetupListSURes(
      itemSUResList);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSURes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceSetupListSURes;

  int ret = pduSessionResourceSetupResponseList
                ->encode2PDUSessionResourceSetupListSURes(
                    &ie->value.choice.PDUSessionResourceSetupListSURes);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSURes IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSURes IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseMsg::getPduSessionResourceSetupResponseList(
    std::vector<PDUSessionResourceSetupResponseItem_t>& list) {
  if (!pduSessionResourceSetupResponseList) return false;

  std::vector<PDUSessionResourceSetupItemSURes> itemSUResList;
  pduSessionResourceSetupResponseList->getPDUSessionResourceSetupListSURes(
      itemSUResList);

  for (auto& item : itemSUResList) {
    PDUSessionResourceSetupResponseItem_t response = {};

    PDUSessionID pDUSessionID = {};
    item.getPDUSessionResourceSetupItemSURes(
        pDUSessionID, response.pduSessionResourceSetupResponseTransfer);
    pDUSessionID.getPDUSessionID(response.pduSessionId);

    list.push_back(response);
  }

  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setPduSessionResourceFailedToSetupList(
    const std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  if (!pduSessionResourceFailedToSetupResponseList)
    pduSessionResourceFailedToSetupResponseList =
        new PDUSessionResourceFailedToSetupListSURes();

  std::vector<PDUSessionResourceFailedToSetupItemSURes> itemSUResList;

  for (int i = 0; i < list.size(); i++) {
    PDUSessionResourceFailedToSetupItemSURes itemSURes = {};
    PDUSessionID pDUSessionID                          = {};
    pDUSessionID.setPDUSessionID(list[i].pduSessionId);

    itemSURes.setPDUSessionResourceFailedToSetupItemSURes(
        pDUSessionID, list[i].pduSessionResourceSetupUnsuccessfulTransfer);
    itemSUResList.push_back(itemSURes);
  }

  pduSessionResourceFailedToSetupResponseList
      ->setPDUSessionResourceFailedToSetupListSURes(itemSUResList);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListSURes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListSURes;

  int ret = pduSessionResourceFailedToSetupResponseList
                ->encode2PDUSessionResourceFailedToSetupListSURes(
                    &ie->value.choice.PDUSessionResourceFailedToSetupListSURes);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceFailedToSetupListSURes IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceFailedToSetupListSURes IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseMsg::getPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  if (!pduSessionResourceFailedToSetupResponseList) return false;

  std::vector<PDUSessionResourceFailedToSetupItemSURes> itemSUResList;
  pduSessionResourceFailedToSetupResponseList
      ->getPDUSessionResourceFailedToSetupListSURes(itemSUResList);

  for (auto& item : itemSUResList) {
    PDUSessionResourceFailedToSetupItem_t failedToResponse = {};
    PDUSessionID pDUSessionID                              = {};

    item.getPDUSessionResourceFailedToSetupItemSURes(
        pDUSessionID,
        failedToResponse.pduSessionResourceSetupUnsuccessfulTransfer);
    pDUSessionID.getPDUSessionID(failedToResponse.pduSessionId);

    list.push_back(failedToResponse);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseMsg::decodeFromPdu(
    Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_PDUSessionResourceSetup &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_PDUSessionResourceSetupResponse) {
      pduSessionResourceSetupResponseIEs =
          &ngapPdu->choice.successfulOutcome->value.choice
               .PDUSessionResourceSetupResponse;
    } else {
      Logger::ngap().error(
          "Check PDUSessionResourceSetupResponse message error!");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0;
       i < pduSessionResourceSetupResponseIEs->protocolIEs.list.count; i++) {
    switch (pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        // TODO: to be verified
        if (/*pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_PDUSessionResourceSetupResponseIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error!");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error!");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        // TODO: to be verified
        if (/*pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_PDUSessionResourceSetupResponseIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decodefromRAN_UE_NGAP_ID(
                  pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error!");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error!");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSURes: {
        if (pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceSetupListSURes) {
          pduSessionResourceSetupResponseList =
              new PDUSessionResourceSetupListSURes();
          if (!pduSessionResourceSetupResponseList
                   ->decodefromPDUSessionResourceSetupListSURes(
                       &pduSessionResourceSetupResponseIEs->protocolIEs.list
                            .array[i]
                            ->value.choice.PDUSessionResourceSetupListSURes)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceSetupListSURes IE error!");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceSetupListSURes IE error!");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListSURes: {
        if (pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListSURes) {
          pduSessionResourceFailedToSetupResponseList =
              new PDUSessionResourceFailedToSetupListSURes();
          if (!pduSessionResourceFailedToSetupResponseList
                   ->decodefromPDUSessionResourceFailedToSetupListSURes(
                       &pduSessionResourceSetupResponseIEs->protocolIEs.list
                            .array[i]
                            ->value.choice
                            .PDUSessionResourceFailedToSetupListSURes)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceFailedToSetupListSURes IE "
                "error!");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceFailedToSetupListSURes IE "
              "error!");
          return false;
        }
      } break;

      default: {
        Logger::ngap().error("Decoded NGAP message PDU error!");
        return false;
      }
    }
  }

  return true;
}

}  // namespace ngap
