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

#include "HandoverRequestAck.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
HandoverRequestAck::HandoverRequestAck() : NgapUEMessage() {
  PDUSessionResourceFailedToSetupList = nullptr;
  CriticalityDiagnostics              = nullptr;
  handoverRequestAckIEs               = nullptr;
  setMessageType(NgapMessageType::HANDOVER_REQUEST_ACKNOWLEDGE);
  initialize();
}

//------------------------------------------------------------------------------
HandoverRequestAck::~HandoverRequestAck() {}

//------------------------------------------------------------------------------
void HandoverRequestAck::initialize() {
  handoverRequestAckIEs = &(ngapPdu->choice.successfulOutcome->value.choice
                                .HandoverRequestAcknowledge);
}

//------------------------------------------------------------------------------
void HandoverRequestAck::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.setAMF_UE_NGAP_ID(id);

  Ngap_HandoverRequestAcknowledgeIEs_t* ie =
      (Ngap_HandoverRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_HandoverRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverRequestAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void HandoverRequestAck::setRanUeNgapId(const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.setRanUeNgapId(ran_ue_ngap_id);

  Ngap_HandoverRequestAcknowledgeIEs_t* ie =
      (Ngap_HandoverRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_HandoverRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverRequestAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
OCTET_STRING_t HandoverRequestAck::getTargetToSource_TransparentContainer() {
  return TargetToSource_TransparentContainer;
}

void HandoverRequestAck::setTargetToSource_TransparentContainer(
    const OCTET_STRING_t& targetTosource) {
  Ngap_HandoverRequestAcknowledgeIEs_t* ie =
      (Ngap_HandoverRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_HandoverRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverRequestAcknowledgeIEs__value_PR_TargetToSource_TransparentContainer;
  ie->value.choice.TargetToSource_TransparentContainer = targetTosource;
  int ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP TargetToSource_TransparentContainer IE error");
}

//------------------------------------------------------------------------------
bool HandoverRequestAck::getPDUSessionResourceAdmittedList(
    std::vector<PDUSessionResourceAdmittedItem_t>& list) {
  std::vector<PDUSessionResourceAdmittedItem> admittedItemList;
  pduSessionResourceAdmittedList.getPDUSessionResourceAdmittedList(
      admittedItemList);

  for (auto& item : admittedItemList) {
    PDUSessionResourceAdmittedItem_t response = {};
    PDUSessionID pDUSessionID                 = {};
    item.getPDUSessionResourceAdmittedItem(
        pDUSessionID, response.handoverRequestAcknowledgeTransfer);
    pDUSessionID.getPDUSessionID(response.pduSessionId);
    list.push_back(response);
  }

  return true;
}

//------------------------------------------------------------------------------
void HandoverRequestAck::setPDUSessionResourceAdmittedList(
    const PDUSessionResourceAdmittedList& admittedList) {
  pduSessionResourceAdmittedList = admittedList;
  Ngap_HandoverRequestAcknowledgeIEs_t* ie =
      (Ngap_HandoverRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_HandoverRequestAcknowledgeIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceAdmittedList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_HandoverRequestAcknowledgeIEs__value_PR_PDUSessionResourceAdmittedList;

  pduSessionResourceAdmittedList.encode2PDUSessionResourceAdmittedList(
      &ie->value.choice.PDUSessionResourceAdmittedList);

  int ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP PDUSessionResourceAdmittedList IE error");
}

//------------------------------------------------------------------------------
bool HandoverRequestAck::decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  if (!ngapMsgPdu) return false;
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_HandoverResourceAllocation &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_HandoverRequestAcknowledge) {
      handoverRequestAckIEs = &ngapPdu->choice.successfulOutcome->value.choice
                                   .HandoverRequestAcknowledge;
    } else {
      Logger::ngap().error("Check handoverRequestAck message error");
      return false;
    }
  } else {
    Logger::ngap().error("handoverRequestAck MessageType error");
    return false;
  }
  for (int i = 0; i < handoverRequestAckIEs->protocolIEs.list.count; i++) {
    switch (handoverRequestAckIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  handoverRequestAckIEs->protocolIEs.list.array[i]
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
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decodefromRAN_UE_NGAP_ID(
                  handoverRequestAckIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceAdmittedList: {
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_PDUSessionResourceAdmittedList) {
          if (!pduSessionResourceAdmittedList
                   .decodefromPDUSessionResourceAdmittedList(
                       &handoverRequestAckIEs->protocolIEs.list.array[i]
                            ->value.choice.PDUSessionResourceAdmittedList)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceAdmittedList IE error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceAdmittedList IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer: {
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_TargetToSource_TransparentContainer) {
          TargetToSource_TransparentContainer =
              handoverRequestAckIEs->protocolIEs.list.array[i]
                  ->value.choice.TargetToSource_TransparentContainer;
        } else {
          Logger::ngap().error(
              "Decoded NGAP TargetToSource_TransparentContainer IE error");

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

}  // namespace ngap
