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

#include "NGSetupResponse.hpp"
#include "logger.hpp"
#include "amf.hpp"
#include "conversions.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
NGSetupResponseMsg::NGSetupResponseMsg() : NgapMessage() {
  ngSetupResponsIEs = nullptr;
  NgapMessage::setMessageType(NgapMessageType::NG_SETUP_RESPONSE);
  initialize();
  // criticalityDiagnostics = nullptr;
}

//------------------------------------------------------------------------------
NGSetupResponseMsg::~NGSetupResponseMsg() {}

//------------------------------------------------------------------------------
void NGSetupResponseMsg::initialize() {
  ngSetupResponsIEs =
      &(ngapPdu->choice.successfulOutcome->value.choice.NGSetupResponse);
}

//------------------------------------------------------------------------------
void NGSetupResponseMsg::setAMFName(const std::string name) {
  amfName.setValue(name);
  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMFName;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_AMFName;

  if (!amfName.encode2AmfName(&ie->value.choice.AMFName)) {
    Logger::ngap().error("Encode NGAP AMFName IE error");
    free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngSetupResponsIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMFName IE error");
}

//------------------------------------------------------------------------------
void NGSetupResponseMsg::setGUAMIList(std::vector<struct GuamiItem_s> list) {
  ServedGUAMIItem servedGUAMIItem = {};
  for (int i = 0; i < list.size(); i++) {
    GUAMI guami = {};
    guami.setGUAMI(
        list[i].mcc, list[i].mnc, list[i].regionID, list[i].AmfSetID,
        list[i].AmfPointer);
    servedGUAMIItem.setGUAMI(guami);

    if (list[i].backupAMFName.size() > 0) {
      AmfName* backupamfname = new AmfName();
      backupamfname->setValue(list[i].backupAMFName);
      servedGUAMIItem.setBackupAMFName(backupamfname);
    }
    servedGUAMIList.addServedGUAMIItem(servedGUAMIItem);
  }

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_ServedGUAMIList;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_ServedGUAMIList;

  if (!servedGUAMIList.encode2ServedGUAMIList(
          &ie->value.choice.ServedGUAMIList)) {
    Logger::ngap().error("Encode NGAP ServedGUAMIList IE error");
    free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngSetupResponsIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP ServedGUAMIList IE error");
}

//------------------------------------------------------------------------------
void NGSetupResponseMsg::setRelativeAmfCapacity(long capacity) {
  relativeAmfCapacity.setValue(capacity);

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RelativeAMFCapacity;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_RelativeAMFCapacity;

  if (!relativeAmfCapacity.encode2RelativeAMFCapacity(
          &ie->value.choice.RelativeAMFCapacity)) {
    Logger::ngap().error("Encode NGAP RelativeAMFCapacity IE error");
    free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngSetupResponsIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP RelativeAMFCapacity IE error");
}

//------------------------------------------------------------------------------
void NGSetupResponseMsg::setPlmnSupportList(
    std::vector<PlmnSliceSupport_t> list) {
  std::vector<PLMNSupportItem> plmnSupportItems;

  for (int i = 0; i < list.size(); i++) {
    PLMNSupportItem plmnSupportItem = {};
    PlmnId plmn                     = {};
    plmn.setMccMnc(list[i].mcc, list[i].mnc);
    Logger::ngap().debug(
        "MCC %s, MNC %s", list[i].mcc.c_str(), list[i].mnc.c_str());

    std::vector<S_NSSAI> snssais;
    for (int j = 0; j < list[i].slice_list.size(); j++) {
      S_NSSAI snssai = {};
      snssai.setSst(list[i].slice_list[j].sst);

      uint32_t sd = SD_NO_VALUE;
      if (!list[i].slice_list[j].sd.empty()) {
        conv::sd_string_to_int(list[i].slice_list[j].sd, sd);
      }
      snssai.setSd(sd);
      snssais.push_back(snssai);
    }
    plmnSupportItem.setPlmnSliceSupportList(plmn, snssais);
    plmnSupportItems.push_back(plmnSupportItem);
  }

  plmnSupportList.addPLMNSupportItems(plmnSupportItems);

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_PLMNSupportList;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_PLMNSupportList;

  if (!plmnSupportList.encode2PLMNSupportList(
          &ie->value.choice.PLMNSupportList)) {
    Logger::ngap().error("Encode NGAP PLMNSupportList IE error");
    free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngSetupResponsIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP PLMNSupportList IE error");
}

//------------------------------------------------------------------------------
bool NGSetupResponseMsg::decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_NGSetup &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_NGSetupResponse) {
      ngSetupResponsIEs =
          &ngapPdu->choice.successfulOutcome->value.choice.NGSetupResponse;
    } else {
      Logger::ngap().error("Check NGSetupResponse message error");
      return false;
    }
  } else {
    Logger::ngap().error(
        "MessageType error, ngapPdu->present %d", ngapPdu->present);
    return false;
  }
  for (int i = 0; i < ngSetupResponsIEs->protocolIEs.list.count; i++) {
    switch (ngSetupResponsIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMFName: {
        if (ngSetupResponsIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            ngSetupResponsIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_AMFName) {
          if (!amfName.decodefromAmfName(
                  &ngSetupResponsIEs->protocolIEs.list.array[i]
                       ->value.choice.AMFName)) {
            Logger::ngap().error("Decoded NGAP AMFName error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP AMFName error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_ServedGUAMIList: {
        if (ngSetupResponsIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            ngSetupResponsIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_ServedGUAMIList) {
          if (!servedGUAMIList.decodefromServedGUAMIList(
                  &ngSetupResponsIEs->protocolIEs.list.array[i]
                       ->value.choice.ServedGUAMIList)) {
            Logger::ngap().error("Decoded NGAP ServedGUAMIList error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP ServedGUAMIList error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RelativeAMFCapacity: {
        if (ngSetupResponsIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ngSetupResponsIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_RelativeAMFCapacity) {
          if (!relativeAmfCapacity.decodefromRelativeAMFCapacity(
                  &ngSetupResponsIEs->protocolIEs.list.array[i]
                       ->value.choice.RelativeAMFCapacity)) {
            Logger::ngap().error("Decoded NGAP RelativeAMFCapacity error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RelativeAMFCapacity error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PLMNSupportList: {
        if (ngSetupResponsIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            ngSetupResponsIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_PLMNSupportList) {
          if (!plmnSupportList.decodefromPLMNSupportList(
                  &ngSetupResponsIEs->protocolIEs.list.array[i]
                       ->value.choice.PLMNSupportList)) {
            Logger::ngap().error("Decoded NGAP PLMNSupportList error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP PLMNSupportList error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        Logger::ngap().debug("Decoded NGAP CriticalityDiagnostics");
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
bool NGSetupResponseMsg::getAMFName(std::string& name) {
  amfName.getValue(name);
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupResponseMsg::getGUAMIList(std::vector<struct GuamiItem_s>& list) {
  std::vector<ServedGUAMIItem> servedGUAMIItems;
  servedGUAMIList.getServedGUAMIItems(servedGUAMIItems);

  for (std::vector<ServedGUAMIItem>::iterator it = std::begin(servedGUAMIItems);
       it != std::end(servedGUAMIItems); ++it) {
    GuamiItem_t guamiItem = {};
    GUAMI guami           = {};
    it->getGUAMI(guami);
    guami.getGUAMI(
        guamiItem.mcc, guamiItem.mnc, guamiItem.regionID, guamiItem.AmfSetID,
        guamiItem.AmfPointer);

    AmfName* backupAMFName;
    if (it->getBackupAMFName(backupAMFName)) {
      backupAMFName->getValue(guamiItem.backupAMFName);
    } else {
      guamiItem.backupAMFName = "None";
    }

    list.push_back(guamiItem);
  }

  return true;
}

//------------------------------------------------------------------------------
long NGSetupResponseMsg::getRelativeAmfCapacity() {
  return relativeAmfCapacity.getValue();
}

//------------------------------------------------------------------------------
bool NGSetupResponseMsg::getPlmnSupportList(
    std::vector<PlmnSliceSupport_t>& list) {
  std::vector<PLMNSupportItem> plmnsupportItemItems;
  plmnSupportList.getPLMNSupportItems(plmnsupportItemItems);

  for (std::vector<PLMNSupportItem>::iterator it =
           std::begin(plmnsupportItemItems);
       it < std::end(plmnsupportItemItems); ++it) {
    PlmnSliceSupport_t plmnSliceSupport = {};
    PlmnId plmn                         = {};
    std::vector<S_NSSAI> snssais;

    it->getPlmnSliceSupportList(plmn, snssais);
    plmn.getMcc(plmnSliceSupport.mcc);
    plmn.getMnc(plmnSliceSupport.mnc);
    for (std::vector<S_NSSAI>::iterator it = std::begin(snssais);
         it < std::end(snssais); ++it) {
      S_Nssai snssai = {};
      it->getSst(snssai.sst);
      it->getSd(snssai.sd);
      plmnSliceSupport.slice_list.push_back(snssai);
    }

    list.push_back(plmnSliceSupport);
  }

  return true;
}

}  // namespace ngap
