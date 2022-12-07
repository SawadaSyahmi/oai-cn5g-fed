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

#include "HandoverCommandMsg.hpp"

#include "logger.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceHandoverItem.h"
#include "dynamic_memory_check.h"
}

using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
HandoverCommandMsg::HandoverCommandMsg() : NgapUEMessage() {
  nASSecurityParametersFromNGRAN       = nullptr;
  pDUSessionResourceToReleaseListHOCmd = nullptr;
  criticalityDiagnostics               = nullptr;
  handoverCommandIEs                   = nullptr;
  pDUSessionResourceHandoverListIsSet  = false;

  setMessageType(NgapMessageType::HANDOVER_COMMAND);
  initialize();
}

//------------------------------------------------------------------------------
HandoverCommandMsg::~HandoverCommandMsg() {
  // TODO:
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::initialize() {
  handoverCommandIEs =
      &ngapPdu->choice.successfulOutcome->value.choice.HandoverCommand;
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.setAMF_UE_NGAP_ID(id);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverCommandIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setRanUeNgapId(const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.setRanUeNgapId(ran_ue_ngap_id);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverCommandIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setHandoverType(const long& type) {
  handoverType = type;
  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_HandoverType;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverCommandIEs__value_PR_HandoverType;
  ie->value.choice.HandoverType = type;
  int ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);

  if (ret != 0) Logger::ngap().error("Encode HandoverType IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setPduSessionResourceHandoverList(
    const PDUSessionResourceHandoverList& list) {
  pDUSessionResourceHandoverList      = list;
  pDUSessionResourceHandoverListIsSet = true;

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceHandoverList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceHandoverList;

  if (!pDUSessionResourceHandoverList.encode(
          ie->value.choice.PDUSessionResourceHandoverList)) {
    Logger::ngap().error("Encode PDUSessionResourceHandoverListItem IE error");
    free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode PDUSessionResourceHandoverList IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::getPduSessionResourceHandoverList(
    PDUSessionResourceHandoverList& list) {
  if (pDUSessionResourceHandoverListIsSet) {
    list = pDUSessionResourceHandoverList;
  }
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setTargetToSource_TransparentContainer(
    const OCTET_STRING_t& targetTosource) {
  targetToSource_TransparentContainer = targetTosource;

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverCommandIEs__value_PR_TargetToSource_TransparentContainer;
  ie->value.choice.TargetToSource_TransparentContainer = targetTosource;
  int ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode HandoverType IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::getTargetToSource_TransparentContainer(
    OCTET_STRING_t& targetTosource) {
  targetTosource = targetToSource_TransparentContainer;
}
//------------------------------------------------------------------------------
bool HandoverCommandMsg::decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  if (!ngapMsgPdu) return false;
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_HandoverPreparation &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_HandoverCommand) {
      handoverCommandIEs =
          &ngapPdu->choice.successfulOutcome->value.choice.HandoverCommand;
    } else {
      Logger::ngap().error("Check Handover Command message error");
      return false;
    }
  } else {
    Logger::ngap().error("Handover Command MessageType error");
    return false;
  }
  for (int i = 0; i < handoverCommandIEs->protocolIEs.list.count; i++) {
    switch (handoverCommandIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  handoverCommandIEs->protocolIEs.list.array[i]
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
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decodefromRAN_UE_NGAP_ID(
                  handoverCommandIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_HandoverType: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_HandoverType) {
          handoverType = handoverCommandIEs->protocolIEs.list.array[i]
                             ->value.choice.HandoverType;
        } else {
          Logger::ngap().error("Decoded NGAP Handover Type IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceHandoverList: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceHandoverList) {
          if (pDUSessionResourceHandoverList.decode(
                  handoverCommandIEs->protocolIEs.list.array[i]
                      ->value.choice.PDUSessionResourceHandoverList)) {
            pDUSessionResourceHandoverListIsSet = true;
          } else {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceHandoverList IE error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceHandoverList IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceToReleaseListHOCmd: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceToReleaseListHOCmd) {
          // TODO:
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceToReleaseListHOCmd IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_TargetToSource_TransparentContainer) {
        } else {
          Logger::ngap().error(
              "Decoded NGAP TargetToSource_TransparentContainer IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_CriticalityDiagnostics) {
        } else {
          Logger::ngap().error("Decoded NGAP CriticalityDiagnostics IE error");
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

}  // namespace ngap
