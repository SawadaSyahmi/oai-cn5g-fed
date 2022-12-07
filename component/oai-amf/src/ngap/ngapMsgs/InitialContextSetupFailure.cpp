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

#include "InitialContextSetupFailure.hpp"
#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
InitialContextSetupFailureMsg::InitialContextSetupFailureMsg() {
  initialContextSetupFailureIEs              = nullptr;
  pduSessionResourceFailedToSetupFailureList = nullptr;

  setMessageType(NgapMessageType::INITIAL_CONTEXT_SETUP_FAILURE);
  initialize();
}

//------------------------------------------------------------------------------
InitialContextSetupFailureMsg::~InitialContextSetupFailureMsg() {
  if (initialContextSetupFailureIEs) free(initialContextSetupFailureIEs);
  if (pduSessionResourceFailedToSetupFailureList)
    delete (pduSessionResourceFailedToSetupFailureList);
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::initialize() {
  initialContextSetupFailureIEs = &(ngapPdu->choice.unsuccessfulOutcome->value
                                        .choice.InitialContextSetupFailure);
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.setAMF_UE_NGAP_ID(id);

  Ngap_InitialContextSetupFailureIEs_t* ie =
      (Ngap_InitialContextSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupFailureIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupFailureIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.setRanUeNgapId(ran_ue_ngap_id);

  Ngap_InitialContextSetupFailureIEs_t* ie =
      (Ngap_InitialContextSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupFailureIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupFailureIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::setPduSessionResourceFailedToSetupList(
    const std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  if (!pduSessionResourceFailedToSetupFailureList)
    pduSessionResourceFailedToSetupFailureList =
        new PDUSessionResourceFailedToSetupListCxtFail();

  std::vector<PDUSessionResourceFailedToSetupItemCxtFail> itemCxtFailList;
  itemCxtFailList.reserve(list.size());

  for (int i = 0; i < list.size(); i++) {
    PDUSessionResourceFailedToSetupItemCxtFail itemCxtFail = {};
    PDUSessionID pDUSessionID                              = {};
    pDUSessionID.setPDUSessionID(list[i].pduSessionId);

    itemCxtFail.setPDUSessionResourceFailedToSetupItemCxtFail(
        pDUSessionID, list[i].pduSessionResourceSetupUnsuccessfulTransfer);
    itemCxtFailList.push_back(itemCxtFail);
  }

  pduSessionResourceFailedToSetupFailureList
      ->setPDUSessionResourceFailedToSetupListCxtFail(itemCxtFailList);

  Ngap_InitialContextSetupFailureIEs_t* ie =
      (Ngap_InitialContextSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupFailureIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListCxtFail;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupFailureIEs__value_PR_PDUSessionResourceFailedToSetupListCxtFail;

  int ret =
      pduSessionResourceFailedToSetupFailureList
          ->encode2PDUSessionResourceFailedToSetupListCxtFail(
              &ie->value.choice.PDUSessionResourceFailedToSetupListCxtFail);
  if (!ret) {
    Logger::ngap().error(
        "Encode PDUSessionResourceFailedToSetupListCxtFail IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode PDUSessionResourceFailedToSetupListCxtFail IE error");
}

//------------------------------------------------------------------------------
bool InitialContextSetupFailureMsg::decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_unsuccessfulOutcome) {
    if (ngapPdu->choice.unsuccessfulOutcome &&
        ngapPdu->choice.unsuccessfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_InitialContextSetup &&
        ngapPdu->choice.unsuccessfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.unsuccessfulOutcome->value.present ==
            Ngap_UnsuccessfulOutcome__value_PR_InitialContextSetupFailure) {
      initialContextSetupFailureIEs =
          &ngapPdu->choice.unsuccessfulOutcome->value.choice
               .InitialContextSetupFailure;
    } else {
      Logger::ngap().error("Check InitialContextSetupFailure message error!");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < initialContextSetupFailureIEs->protocolIEs.list.count;
       i++) {
    switch (initialContextSetupFailureIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupFailureIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  initialContextSetupFailureIEs->protocolIEs.list.array[i]
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
        if (initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupFailureIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decodefromRAN_UE_NGAP_ID(
                  initialContextSetupFailureIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListCxtFail: {
        if (initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupFailureIEs__value_PR_PDUSessionResourceFailedToSetupListCxtFail) {
          pduSessionResourceFailedToSetupFailureList =
              new PDUSessionResourceFailedToSetupListCxtFail();
          if (!pduSessionResourceFailedToSetupFailureList
                   ->decodefromPDUSessionResourceFailedToSetupListCxtFail(
                       &initialContextSetupFailureIEs->protocolIEs.list
                            .array[i]
                            ->value.choice
                            .PDUSessionResourceFailedToSetupListCxtFail)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceFailedToSetupListCxtFail IE "
                "error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceFailedToSetupListCxtFail IE "
              "error");
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
bool InitialContextSetupFailureMsg::getPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  if (!pduSessionResourceFailedToSetupFailureList) return false;

  std::vector<PDUSessionResourceFailedToSetupItemCxtFail> itemCxtFailList;

  pduSessionResourceFailedToSetupFailureList
      ->getPDUSessionResourceFailedToSetupListCxtFail(itemCxtFailList);

  for (std::vector<PDUSessionResourceFailedToSetupItemCxtFail>::iterator it =
           std::begin(itemCxtFailList);
       it < std::end(itemCxtFailList); ++it) {
    PDUSessionResourceFailedToSetupItem_t failedToFailure = {};

    PDUSessionID pDUSessionID = {};
    it->getPDUSessionResourceFailedToSetupItemCxtFail(
        pDUSessionID,
        failedToFailure.pduSessionResourceSetupUnsuccessfulTransfer);
    pDUSessionID.getPDUSessionID(failedToFailure.pduSessionId);

    list.push_back(failedToFailure);
  }

  return true;
}

}  // namespace ngap
