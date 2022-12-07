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

#include "PduSessionResourceReleaseCommand.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceReleaseCommandMsg::PduSessionResourceReleaseCommandMsg()
    : NgapUEMessage() {
  pduSessionResourceReleaseCommandIEs = nullptr;
  ranPagingPriority                   = nullptr;
  nasPdu                              = nullptr;

  setMessageType(NgapMessageType::PDU_SESSION_RESOURCE_RELEASE_COMMAND);
  initialize();
}

//------------------------------------------------------------------------------
PduSessionResourceReleaseCommandMsg::~PduSessionResourceReleaseCommandMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::initialize() {
  pduSessionResourceReleaseCommandIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .PDUSessionResourceReleaseCommand);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setAmfUeNgapId(
    const unsigned long& id) {
  amfUeNgapId.setAMF_UE_NGAP_ID(id);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::nas_mm().warn("Encode AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.setRanUeNgapId(ran_ue_ngap_id);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::nas_mm().warn("Encode RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setRanPagingPriority(
    const uint8_t& priority) {
  if (!ranPagingPriority) ranPagingPriority = new RANPagingPriority();

  ranPagingPriority->setRANPagingPriority(priority);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RANPagingPriority;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_RANPagingPriority;

  int ret = ranPagingPriority->encode2RANPagingPriority(
      ie->value.choice.RANPagingPriority);
  if (!ret) {
    Logger::nas_mm().warn("Encode RANPagingPriority IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode RANPagingPriority IE error");
}

//------------------------------------------------------------------------------
int PduSessionResourceReleaseCommandMsg::getRanPagingPriority() {
  if (!ranPagingPriority) return -1;
  return ranPagingPriority->getRANPagingPriority();
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setNasPdu(uint8_t* nas, size_t size) {
  if (!nasPdu) nasPdu = new NAS_PDU();

  nasPdu->setNasPdu(nas, size);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_NAS_PDU;

  int ret = nasPdu->encode2octetstring(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::nas_mm().warn("encode NAS_PDU IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode NAS_PDU IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseCommandMsg::getNasPdu(
    uint8_t*& nas, size_t& sizeofnas) {
  if (!nasPdu) return false;
  if (!nasPdu->getNasPdu(nas, sizeofnas)) return false;

  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setPduSessionResourceToReleaseList(
    const std::vector<PDUSessionResourceToReleaseItem_t>& list) {
  std::vector<PDUSessionResourceToReleaseItemRelCmd> itemRelCmdList;

  for (int i = 0; i < list.size(); i++) {
    PDUSessionResourceToReleaseItemRelCmd itemRelCmd = {};
    PDUSessionID pDUSessionID                        = {};
    pDUSessionID.setPDUSessionID(list[i].pduSessionId);
    itemRelCmd.setPDUSessionResourceToReleaseItemRelCmd(
        pDUSessionID, list[i].pduSessionResourceReleaseCommandTransfer);
    itemRelCmdList.push_back(itemRelCmd);
  }

  pduSessionResourceToReleaseList.setPDUSessionResourceToReleaseListRelCmd(
      itemRelCmdList);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceToReleaseListRelCmd;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_PDUSessionResourceToReleaseListRelCmd;

  int ret = pduSessionResourceToReleaseList
                .encode2PDUSessionResourceToReleaseListRelCmd(
                    &ie->value.choice.PDUSessionResourceToReleaseListRelCmd);
  if (!ret) {
    Logger::nas_mm().warn(
        "Encode PDUSessionResourceToReleaseListRelCmd IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::nas_mm().warn(
        "Encode PDUSessionResourceToReleaseListRelCmd IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseCommandMsg::getPduSessionResourceToReleaseList(
    std::vector<PDUSessionResourceToReleaseItem_t>& list) {
  std::vector<PDUSessionResourceToReleaseItemRelCmd> itemRelCmdList;
  pduSessionResourceToReleaseList.getPDUSessionResourceToReleaseListRelCmd(
      itemRelCmdList);

  for (auto& item : itemRelCmdList) {
    PDUSessionResourceToReleaseItem_t rel = {};
    PDUSessionID pDUSessionID             = {};

    item.getPDUSessionResourceToReleaseItemRelCmd(
        pDUSessionID, rel.pduSessionResourceReleaseCommandTransfer);
    pDUSessionID.getPDUSessionID(rel.pduSessionId);

    list.push_back(rel);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseCommandMsg::decodeFromPdu(
    Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_PDUSessionResourceRelease &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_PDUSessionResourceReleaseCommand) {
      pduSessionResourceReleaseCommandIEs =
          &ngapPdu->choice.initiatingMessage->value.choice
               .PDUSessionResourceReleaseCommand;
    } else {
      Logger::nas_mm().warn(
          "Check PDUSessionResourceReleaseCommand message error!");
      return false;
    }
  } else {
    Logger::nas_mm().warn("MessageType error");
    return false;
  }

  for (int i = 0;
       i < pduSessionResourceReleaseCommandIEs->protocolIEs.list.count; i++) {
    switch (
        pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  pduSessionResourceReleaseCommandIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::nas_mm().warn("Decoded NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn("Decoded NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decodefromRAN_UE_NGAP_ID(
                  pduSessionResourceReleaseCommandIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::nas_mm().warn("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_RANPagingPriority: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_RANPagingPriority) {
          ranPagingPriority = new RANPagingPriority();
          if (!ranPagingPriority->decodefromRANPagingPriority(
                  pduSessionResourceReleaseCommandIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.RANPagingPriority)) {
            Logger::nas_mm().warn("Decoded NGAP RANPagingPriority IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn("Decoded NGAP RANPagingPriority IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_NAS_PDU) {
          nasPdu = new NAS_PDU();
          if (!nasPdu->decodefromoctetstring(
                  pduSessionResourceReleaseCommandIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.NAS_PDU)) {
            Logger::nas_mm().warn("Decoded NGAP NAS_PDU IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn("Decoded NGAP NAS_PDU IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_PDUSessionResourceToReleaseListRelCmd: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_PDUSessionResourceToReleaseListRelCmd) {
          if (!pduSessionResourceToReleaseList
                   .decodefromPDUSessionResourceToReleaseListRelCmd(
                       &pduSessionResourceReleaseCommandIEs->protocolIEs.list
                            .array[i]
                            ->value.choice
                            .PDUSessionResourceToReleaseListRelCmd)) {
            Logger::nas_mm().warn(
                "Decoded NGAP PDUSessionResourceToReleaseListRelCmd IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn(
              "Decoded NGAP PDUSessionResourceToReleaseListRelCmd IE error");
          return false;
        }
      } break;
      default: {
        Logger::nas_mm().warn("Decoded NGAP message PDU error");
        return false;
      }
    }
  }

  return true;
}
}  // namespace ngap
