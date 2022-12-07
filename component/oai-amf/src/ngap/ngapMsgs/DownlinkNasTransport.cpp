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

#include "DownlinkNasTransport.hpp"
#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
DownLinkNasTransportMsg::DownLinkNasTransportMsg() : NgapUEMessage() {
  downLinkNasTransportIEs = nullptr;
  oldAmfName              = nullptr;
  ranPagingPriority       = nullptr;
  indexToRFSP             = nullptr;

  setMessageType(NgapMessageType::DOWNLINK_NAS_TRANSPORT);
  initialize();
}

//------------------------------------------------------------------------------
DownLinkNasTransportMsg::~DownLinkNasTransportMsg() {
  if (oldAmfName) delete oldAmfName;
  if (ranPagingPriority) delete ranPagingPriority;
  if (indexToRFSP) delete indexToRFSP;
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::initialize() {
  downLinkNasTransportIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice.DownlinkNASTransport);
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.setAMF_UE_NGAP_ID(id);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setRanUeNgapId(const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.setRanUeNgapId(ran_ue_ngap_id);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setOldAmfName(const std::string name) {
  if (!oldAmfName) oldAmfName = new AmfName();
  oldAmfName->setValue(name);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_OldAMF;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AMFName;

  int ret = oldAmfName->encode2AmfName(&ie->value.choice.AMFName);
  if (!ret) {
    Logger::ngap().error("Encode oldAmfName IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode oldAmfName IE error");
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setRanPagingPriority(uint8_t pagingPriority) {
  if (!ranPagingPriority) ranPagingPriority = new RANPagingPriority();
  ranPagingPriority->setRANPagingPriority(pagingPriority);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RANPagingPriority;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_RANPagingPriority;

  int ret = ranPagingPriority->encode2RANPagingPriority(
      ie->value.choice.RANPagingPriority);
  if (!ret) {
    Logger::ngap().error("Encode RANPagingPriority IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setNasPdu(uint8_t* nas, size_t sizeofnas) {
  nasPdu.setNasPdu(nas, sizeofnas);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_NAS_PDU;

  int ret = nasPdu.encode2octetstring(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NAS_PDU IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NAS_PDU IE error");
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setIndex2Rat_Frequency_SelectionPriority(
    uint8_t value) {
  if (!indexToRFSP) indexToRFSP = new IndexToRFSP();
  indexToRFSP->setIndexToRFSP(value);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_IndexToRFSP;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_IndexToRFSP;

  int ret = indexToRFSP->encode2IndexToRFSP(ie->value.choice.IndexToRFSP);
  if (!ret) {
    Logger::ngap().error("Encode IndexToRFSP IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode IndexToRFSP IE error");
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_DownlinkNASTransport &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_DownlinkNASTransport) {
      downLinkNasTransportIEs =
          &ngapPdu->choice.initiatingMessage->value.choice.DownlinkNASTransport;
    } else {
      Logger::ngap().error("Decode NGAP DownlinkNASTransport error");
      return false;
    }
  } else {
    Logger::ngap().error("Decode NGAP MessageType IE error");
    return false;
  }
  for (int i = 0; i < downLinkNasTransportIEs->protocolIEs.list.count; i++) {
    switch (downLinkNasTransportIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  downLinkNasTransportIEs->protocolIEs.list.array[i]
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
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decodefromRAN_UE_NGAP_ID(
                  downLinkNasTransportIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_OldAMF: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_AMFName) {
          oldAmfName = new AmfName();
          if (!oldAmfName->decodefromAmfName(
                  &downLinkNasTransportIEs->protocolIEs.list.array[i]
                       ->value.choice.AMFName)) {
            Logger::ngap().error("Decode NGAP OldAMFName IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP OldAMFName IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RANPagingPriority: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_RANPagingPriority) {
          ranPagingPriority = new RANPagingPriority();
          if (!ranPagingPriority->decodefromRANPagingPriority(
                  downLinkNasTransportIEs->protocolIEs.list.array[i]
                      ->value.choice.RANPagingPriority)) {
            Logger::ngap().error("Decode NGAP RANPagingPriority IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP RANPagingPriority IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_NAS_PDU) {
          if (!nasPdu.decodefromoctetstring(
                  downLinkNasTransportIEs->protocolIEs.list.array[i]
                      ->value.choice.NAS_PDU)) {
            Logger::ngap().error("Decode NGAP NAS_PDU IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP NAS_PDU IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_IndexToRFSP: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_IndexToRFSP) {
          indexToRFSP = new IndexToRFSP();
          if (!indexToRFSP->decodefromIndexToRFSP(
                  downLinkNasTransportIEs->protocolIEs.list.array[i]
                      ->value.choice.IndexToRFSP)) {
            Logger::ngap().error("Decode NGAP IndexToRFSP IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP IndexToRFSP IE error");
          return false;
        }
      } break;

      default: {
        Logger::ngap().error("Decode NGAP message PDU error");
        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getOldAmfName(std::string& name) {
  if (!oldAmfName) return false;
  oldAmfName->getValue(name);
  return true;
}

//------------------------------------------------------------------------------
uint8_t DownLinkNasTransportMsg::getRanPagingPriority() {
  if (ranPagingPriority)
    return ranPagingPriority->getRANPagingPriority();
  else
    return 0;
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getNasPdu(uint8_t*& nas, size_t& sizeofnas) {
  if (!nasPdu.getNasPdu(nas, sizeofnas)) return false;
  return true;
}

//------------------------------------------------------------------------------
uint8_t DownLinkNasTransportMsg::getIndex2Rat_Frequency_SelectionPriority() {
  if (indexToRFSP)
    return indexToRFSP->getIndexToRFSP();
  else
    return 0;
}

}  // namespace ngap
