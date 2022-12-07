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

#include "InitialContextSetupResponse.hpp"
#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
InitialContextSetupResponseMsg::InitialContextSetupResponseMsg()
    : NgapUEMessage() {
  initialContextSetupResponseIEs              = nullptr;
  pduSessionResourceSetupResponseList         = nullptr;
  pduSessionResourceFailedToSetupResponseList = nullptr;

  setMessageType(NgapMessageType::INITIAL_CONTEXT_SETUP_RESPONSE);
  initialize();
}

//------------------------------------------------------------------------------
InitialContextSetupResponseMsg::~InitialContextSetupResponseMsg() {}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::initialize() {
  initialContextSetupResponseIEs = &(ngapPdu->choice.successfulOutcome->value
                                         .choice.InitialContextSetupResponse);
}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.setAMF_UE_NGAP_ID(id);

  Ngap_InitialContextSetupResponseIEs_t* ie =
      (Ngap_InitialContextSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupResponseIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.setRanUeNgapId(ran_ue_ngap_id);

  Ngap_InitialContextSetupResponseIEs_t* ie =
      (Ngap_InitialContextSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupResponseIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setPduSessionResourceSetupResponseList(
    const std::vector<PDUSessionResourceSetupResponseItem_t>& list) {
  if (!pduSessionResourceSetupResponseList)
    pduSessionResourceSetupResponseList =
        new PDUSessionResourceSetupListCxtRes();

  std::vector<PDUSessionResourceSetupItemCxtRes> itemCxtResList;
  for (int i = 0; i < list.size(); i++) {
    PDUSessionID m_pDUSessionID = {};
    m_pDUSessionID.setPDUSessionID(list[i].pduSessionId);
    PDUSessionResourceSetupItemCxtRes itemCxtRes = {};
    itemCxtRes.setPDUSessionResourceSetupItemCxtRes(
        m_pDUSessionID, list[i].pduSessionResourceSetupResponseTransfer);
    itemCxtResList.push_back(itemCxtRes);
  }

  pduSessionResourceSetupResponseList->setPDUSessionResourceSetupListCxtRes(
      itemCxtResList);

  Ngap_InitialContextSetupResponseIEs_t* ie =
      (Ngap_InitialContextSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtRes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceSetupListCxtRes;

  int ret = pduSessionResourceSetupResponseList
                ->encode2PDUSessionResourceSetupListCxtRes(
                    &ie->value.choice.PDUSessionResourceSetupListCxtRes);
  if (!ret) {
    Logger::ngap().error("Encode PDUSessionResourceSetupListCxtRes IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode PDUSessionResourceSetupListCxtRes IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setPduSessionResourceFailedToSetupList(
    const std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  if (!pduSessionResourceFailedToSetupResponseList)
    pduSessionResourceFailedToSetupResponseList =
        new PDUSessionResourceFailedToSetupListCxtRes();

  std::vector<PDUSessionResourceFailedToSetupItemCxtRes> itemCxtResList;
  itemCxtResList.reserve(list.size());

  for (int i = 0; i < list.size(); i++) {
    PDUSessionID pDUSessionID = {};
    pDUSessionID.setPDUSessionID(list[i].pduSessionId);
    PDUSessionResourceFailedToSetupItemCxtRes itemCxtRes = {};
    itemCxtRes.setPDUSessionResourceFailedToSetupItemCxtRes(
        pDUSessionID, list[i].pduSessionResourceSetupUnsuccessfulTransfer);
    itemCxtResList.push_back(itemCxtRes);
  }

  pduSessionResourceFailedToSetupResponseList
      ->setPDUSessionResourceFailedToSetupListCxtRes(itemCxtResList);

  Ngap_InitialContextSetupResponseIEs_t* ie =
      (Ngap_InitialContextSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupResponseIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListCxtRes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListCxtRes;

  int ret =
      pduSessionResourceFailedToSetupResponseList
          ->encode2PDUSessionResourceFailedToSetupListCxtRes(
              &ie->value.choice.PDUSessionResourceFailedToSetupListCxtRes);
  if (!ret) {
    Logger::ngap().error(
        "Encode PDUSessionResourceFailedToSetupListCxtRes IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode PDUSessionResourceFailedToSetupListCxtRes IE error");
}

//------------------------------------------------------------------------------
bool InitialContextSetupResponseMsg::decodeFromPdu(
    Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_InitialContextSetup &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_InitialContextSetupResponse) {
      initialContextSetupResponseIEs = &ngapPdu->choice.successfulOutcome->value
                                            .choice.InitialContextSetupResponse;
    } else {
      Logger::ngap().error("Check InitialContextSetupResponse message error");

      return false;
    }
  } else {
    Logger::ngap().error("MessageType error");

    return false;
  }
  for (int i = 0; i < initialContextSetupResponseIEs->protocolIEs.list.count;
       i++) {
    switch (initialContextSetupResponseIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        // TODO: to be verified
        if (/*initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            initialContextSetupResponseIEs->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_InitialContextSetupResponseIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  initialContextSetupResponseIEs->protocolIEs.list.array[i]
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
        // TODO: to be verified
        if (/*initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            initialContextSetupResponseIEs->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_InitialContextSetupResponseIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decodefromRAN_UE_NGAP_ID(
                  initialContextSetupResponseIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtRes: {
        if (initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceSetupListCxtRes) {
          pduSessionResourceSetupResponseList =
              new PDUSessionResourceSetupListCxtRes();
          if (!pduSessionResourceSetupResponseList
                   ->decodefromPDUSessionResourceSetupListCxtRes(
                       &initialContextSetupResponseIEs->protocolIEs.list
                            .array[i]
                            ->value.choice.PDUSessionResourceSetupListCxtRes)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceSetupListCxtRes IE error");

            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceSetupListCxtRes IE error");

          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListCxtRes: {
        if (initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListCxtRes) {
          pduSessionResourceFailedToSetupResponseList =
              new PDUSessionResourceFailedToSetupListCxtRes();
          if (!pduSessionResourceFailedToSetupResponseList
                   ->decodefromPDUSessionResourceFailedToSetupListCxtRes(
                       &initialContextSetupResponseIEs->protocolIEs.list
                            .array[i]
                            ->value.choice
                            .PDUSessionResourceFailedToSetupListCxtRes)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceFailedToSetupListCxtRes IE "
                "error");

            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceFailedToSetupListCxtRes IE "
              "error");

          return false;
        }
      } break;

      default: {
        Logger::ngap().error("Decoded NGAP message PDU error");

        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupResponseMsg::getPduSessionResourceSetupResponseList(
    std::vector<PDUSessionResourceSetupResponseItem_t>& list) {
  if (!pduSessionResourceSetupResponseList) return false;

  std::vector<PDUSessionResourceSetupItemCxtRes> itemCxtResList;
  pduSessionResourceSetupResponseList->getPDUSessionResourceSetupListCxtRes(
      itemCxtResList);

  for (std::vector<PDUSessionResourceSetupItemCxtRes>::iterator it =
           std::begin(itemCxtResList);
       it < std::end(itemCxtResList); ++it) {
    PDUSessionResourceSetupResponseItem_t response = {};

    PDUSessionID m_pDUSessionID = {};
    it->getPDUSessionResourceSetupItemCxtRes(
        m_pDUSessionID, response.pduSessionResourceSetupResponseTransfer);
    m_pDUSessionID.getPDUSessionID(response.pduSessionId);
    list.push_back(response);
  }

  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupResponseMsg::getPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  if (!pduSessionResourceFailedToSetupResponseList) return false;

  std::vector<PDUSessionResourceFailedToSetupItemCxtRes> itemCxtResList;
  pduSessionResourceFailedToSetupResponseList
      ->getPDUSessionResourceFailedToSetupListCxtRes(itemCxtResList);

  for (std::vector<PDUSessionResourceFailedToSetupItemCxtRes>::iterator it =
           std::begin(itemCxtResList);
       it < std::end(itemCxtResList); ++it) {
    PDUSessionResourceFailedToSetupItem_t failedToResponse = {};

    PDUSessionID pDUSessionID = {};
    it->getPDUSessionResourceFailedToSetupItemCxtRes(
        pDUSessionID,
        failedToResponse.pduSessionResourceSetupUnsuccessfulTransfer);
    pDUSessionID.getPDUSessionID(failedToResponse.pduSessionId);

    list.push_back(failedToResponse);
  }

  return true;
}

}  // namespace ngap
