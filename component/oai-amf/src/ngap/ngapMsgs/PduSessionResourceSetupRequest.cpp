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

#include "PduSessionResourceSetupRequest.hpp"
#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupRequestMsg::PduSessionResourceSetupRequestMsg()
    : NgapUEMessage() {
  pduSessionResourceSetupRequestIEs = nullptr;
  ranPagingPriority                 = nullptr;
  nasPdu                            = nullptr;
  uEAggregateMaxBitRate             = nullptr;

  setMessageType(NgapMessageType::PDU_SESSION_RESOURCE_SETUP_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
PduSessionResourceSetupRequestMsg::~PduSessionResourceSetupRequestMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::initialize() {
  pduSessionResourceSetupRequestIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .PDUSessionResourceSetupRequest);
}

//-----------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setUEAggregateMaxBitRate(
    const long& bit_rate_downlink, const long& bit_rate_uplink) {
  if (!uEAggregateMaxBitRate)
    uEAggregateMaxBitRate = new UEAggregateMaxBitRate();

  uEAggregateMaxBitRate->setUEAggregateMaxBitRate(
      bit_rate_downlink, bit_rate_uplink);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

  int ret = uEAggregateMaxBitRate->encode2UEAggregateMaxBitRate(
      ie->value.choice.UEAggregateMaximumBitRate);
  if (!ret) {
    Logger::ngap().error("Encode NGAP UEAggregateMaxBitRate IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UEAggregateMaxBitRate IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setAmfUeNgapId(
    const unsigned long& id) {
  amfUeNgapId.setAMF_UE_NGAP_ID(id);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.setRanUeNgapId(ran_ue_ngap_id);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setRanPagingPriority(
    const uint8_t& priority) {
  if (!ranPagingPriority) ranPagingPriority = new RANPagingPriority();

  ranPagingPriority->setRANPagingPriority(priority);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RANPagingPriority;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_RANPagingPriority;

  int ret = ranPagingPriority->encode2RANPagingPriority(
      ie->value.choice.RANPagingPriority);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
}

//------------------------------------------------------------------------------
int PduSessionResourceSetupRequestMsg::getRanPagingPriority() {
  if (!ranPagingPriority) return -1;
  return ranPagingPriority->getRANPagingPriority();
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setNasPdu(uint8_t* nas, size_t size) {
  if (!nasPdu) nasPdu = new NAS_PDU();

  nasPdu->setNasPdu(nas, size);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_PDUSessionResourceSetupRequestIEs__value_PR_NAS_PDU;

  int ret = nasPdu->encode2octetstring(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NGAP NAS_PDU IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP NAS_PDU IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestMsg::getNasPdu(
    uint8_t*& nas, size_t& sizeofnas) {
  if (!nasPdu) return false;
  if (!nasPdu->getNasPdu(nas, sizeofnas)) return false;

  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setPduSessionResourceSetupRequestList(
    const std::vector<PDUSessionResourceSetupRequestItem_t>& list) {
  std::vector<PDUSessionResourceSetupItemSUReq> itemSUReqList;

  for (int i = 0; i < list.size(); i++) {
    PDUSessionResourceSetupItemSUReq itemSUReq = {};
    PDUSessionID pDUSessionID                  = {};
    pDUSessionID.setPDUSessionID(list[i].pduSessionId);
    NAS_PDU* m_nAS_PDU = nullptr;
    if (list[i].pduSessionNAS_PDU) {
      m_nAS_PDU = new NAS_PDU();
      m_nAS_PDU->setNasPdu(
          list[i].pduSessionNAS_PDU, list[i].sizeofpduSessionNAS_PDU);
    }
    S_NSSAI s_NSSAI = {};
    s_NSSAI.setSst(list[i].s_nssai.sst);
    if (list[i].s_nssai.sd.size()) s_NSSAI.setSd(list[i].s_nssai.sd);
    itemSUReq.setPDUSessionResourceSetupItemSUReq(
        pDUSessionID, m_nAS_PDU, s_NSSAI,
        list[i].pduSessionResourceSetupRequestTransfer);
    itemSUReqList.push_back(itemSUReq);
  }

  pduSessionResourceSetupRequestList.setPDUSessionResourceSetupListSUReq(
      itemSUReqList);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSUReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_PDUSessionResourceSetupListSUReq;

  int ret = pduSessionResourceSetupRequestList
                .encode2PDUSessionResourceSetupListSUReq(
                    &ie->value.choice.PDUSessionResourceSetupListSUReq);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSUReq IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSUReq IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestMsg::getPduSessionResourceSetupRequestList(
    std::vector<PDUSessionResourceSetupRequestItem_t>& list) {
  std::vector<PDUSessionResourceSetupItemSUReq> itemSUReqList;
  pduSessionResourceSetupRequestList.getPDUSessionResourceSetupListSUReq(
      itemSUReqList);

  for (auto& item : itemSUReqList) {
    PDUSessionResourceSetupRequestItem_t request = {};

    PDUSessionID pDUSessionID = {};
    NAS_PDU* nAS_PDU          = nullptr;
    S_NSSAI s_NSSAI           = {};
    item.getPDUSessionResourceSetupItemSUReq(
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
bool PduSessionResourceSetupRequestMsg::decodeFromPdu(
    Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_PDUSessionResourceSetup &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_PDUSessionResourceSetupRequest) {
      pduSessionResourceSetupRequestIEs =
          &ngapPdu->choice.initiatingMessage->value.choice
               .PDUSessionResourceSetupRequest;
    } else {
      Logger::ngap().error(
          "Check PDUSessionResourceSetupRequest message error!");

      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < pduSessionResourceSetupRequestIEs->protocolIEs.list.count;
       i++) {
    switch (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
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
        if (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decodefromRAN_UE_NGAP_ID(
                  pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RANPagingPriority: {
        if (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_RANPagingPriority) {
          ranPagingPriority = new RANPagingPriority();
          if (!ranPagingPriority->decodefromRANPagingPriority(
                  pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.RANPagingPriority)) {
            Logger::ngap().error("Decoded NGAP RANPagingPriority IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RANPagingPriority IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_NAS_PDU) {
          nasPdu = new NAS_PDU();
          if (!nasPdu->decodefromoctetstring(
                  pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.NAS_PDU)) {
            Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSUReq: {
        if (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_PDUSessionResourceSetupListSUReq) {
          if (!pduSessionResourceSetupRequestList
                   .decodefromPDUSessionResourceSetupListSUReq(
                       &pduSessionResourceSetupRequestIEs->protocolIEs.list
                            .array[i]
                            ->value.choice.PDUSessionResourceSetupListSUReq)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceSetupListSUReq IE error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceSetupListSUReq IE error");

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
