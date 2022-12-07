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

#include "InitialUEMessage.hpp"
#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
InitialUEMessageMsg::InitialUEMessageMsg() : NgapMessage() {
  initialUEMessageIEs = nullptr;
  uEContextRequest    = nullptr;
  fivegSTmsi          = nullptr;

  NgapMessage::setMessageType(NgapMessageType::NG_SETUP_RESPONSE);
  initialize();
}

//------------------------------------------------------------------------------
InitialUEMessageMsg::~InitialUEMessageMsg() {
  if (uEContextRequest) delete uEContextRequest;
  if (fivegSTmsi) delete fivegSTmsi;
}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::initialize() {
  initialUEMessageIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice.InitialUEMessage);
}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::setRanUENgapID(const uint32_t& value) {
  ranUeNgapId.setRanUeNgapId(value);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialUEMessageIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::setNasPdu(uint8_t* nas, size_t size) {
  nasPdu.setNasPdu(nas, size);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_NAS_PDU;

  int ret = nasPdu.encode2octetstring(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NAS PDU IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialUEMessageIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NAS PDU IE error");
}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::setUserLocationInfoNR(
    const struct NrCgi_s& cig, const struct Tai_s& tai) {
  UserLocationInformationNR* informationNR = new UserLocationInformationNR();
  NR_CGI nR_CGI                            = {};
  nR_CGI.setNR_CGI(cig.mcc, cig.mnc, cig.nrCellID);

  TAI tai_nr = {};
  tai_nr.setTAI(tai);
  informationNR->setInformationNR(nR_CGI, tai_nr);
  userLocationInformation.setInformation(informationNR);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialUEMessage_IEs__value_PR_UserLocationInformation;

  int ret = userLocationInformation.encodefromUserLocationInformation(
      &ie->value.choice.UserLocationInformation);
  if (!ret) {
    Logger::ngap().error("Encode UserLocationInformation IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialUEMessageIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UserLocationInformation IE error");
}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::setRRCEstablishmentCause(
    const e_Ngap_RRCEstablishmentCause& cause) {
  rRCEstablishmentCause.setRRCEstablishmentCause(cause);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RRCEstablishmentCause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_RRCEstablishmentCause;

  int ret = rRCEstablishmentCause.encode2RRCEstablishmentCause(
      ie->value.choice.RRCEstablishmentCause);
  if (!ret) {
    Logger::ngap().error("Encode RRCEstablishmentCause IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialUEMessageIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RRCEstablishmentCause IE error");
}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::setUeContextRequest(
    const e_Ngap_UEContextRequest& ueCtxReq) {
  if (!uEContextRequest) uEContextRequest = new UEContextRequest();

  uEContextRequest->setUEContextRequest(ueCtxReq);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UEContextRequest;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_UEContextRequest;

  int ret = uEContextRequest->encode2UEContextRequest(
      ie->value.choice.UEContextRequest);
  if (!ret) {
    Logger::ngap().error("Encode UEContextRequest IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialUEMessageIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UEContextRequest IE error");
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_InitialUEMessage &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_InitialUEMessage) {
      initialUEMessageIEs =
          &ngapPdu->choice.initiatingMessage->value.choice.InitialUEMessage;
    } else {
      Logger::ngap().error("Check InitialUEMessage message error");
      return false;
    }
  } else {
    Logger::ngap().error("Check MessageType error");
    return false;
  }
  for (int i = 0; i < initialUEMessageIEs->protocolIEs.list.count; i++) {
    switch (initialUEMessageIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decodefromRAN_UE_NGAP_ID(
                  initialUEMessageIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
          Logger::ngap().debug(
              "Received RanUeNgapId %d ", ranUeNgapId.getRanUeNgapId());

        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_NAS_PDU) {
          if (!nasPdu.decodefromoctetstring(
                  initialUEMessageIEs->protocolIEs.list.array[i]
                      ->value.choice.NAS_PDU)) {
            Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UserLocationInformation: {
        // TODO: to be verified
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_UserLocationInformation) {
          if (!userLocationInformation.decodefromUserLocationInformation(
                  &initialUEMessageIEs->protocolIEs.list.array[i]
                       ->value.choice.UserLocationInformation)) {
            Logger::ngap().error(
                "Decoded NGAP UserLocationInformation IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP UserLocationInformation IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RRCEstablishmentCause: {
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_RRCEstablishmentCause) {
          if (!rRCEstablishmentCause.decodefromRRCEstablishmentCause(
                  initialUEMessageIEs->protocolIEs.list.array[i]
                      ->value.choice.RRCEstablishmentCause)) {
            Logger::ngap().error("Decoded NGAP RRCEstablishmentCause IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RRCEstablishmentCause IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UEContextRequest: {
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_UEContextRequest) {
          uEContextRequest = new UEContextRequest();
          if (!uEContextRequest->decodefromUEContextRequest(
                  initialUEMessageIEs->protocolIEs.list.array[i]
                      ->value.choice.UEContextRequest)) {
            Logger::ngap().error("Decoded NGAP UEContextRequest IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP UEContextRequest IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_FiveG_S_TMSI: {
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_FiveG_S_TMSI) {
          fivegSTmsi = new FiveGSTmsi();
          if (!fivegSTmsi->decodefrompdu(
                  initialUEMessageIEs->protocolIEs.list.array[i]
                      ->value.choice.FiveG_S_TMSI)) {
            Logger::ngap().error("Decoded NGAP FiveG_S_TMSI IE error");
            return false;
          }
        }
      } break;

      default: {
        Logger::ngap().warn(
            "Not decoded IE %d",
            initialUEMessageIEs->protocolIEs.list.array[i]->id);
        return true;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::getRanUENgapID(uint32_t& value) {
  value = ranUeNgapId.getRanUeNgapId();
  return true;
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::getNasPdu(uint8_t*& nas, size_t& size) {
  if (!nasPdu.getNasPdu(nas, size)) return false;
  return true;
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::getUserLocationInfoNR(
    struct NrCgi_s& cig, struct Tai_s& tai) {
  UserLocationInformationNR* informationNR;
  userLocationInformation.getInformation(informationNR);
  if (userLocationInformation.getChoiceOfUserLocationInformation() !=
      Ngap_UserLocationInformation_PR_userLocationInformationNR)
    return false;
  NR_CGI nR_CGI = {};
  TAI nR_TAI    = {};
  informationNR->getInformationNR(nR_CGI, nR_TAI);
  nR_CGI.getNR_CGI(cig);
  nR_TAI.getTAI(tai);

  return true;
}

//------------------------------------------------------------------------------
int InitialUEMessageMsg::getRRCEstablishmentCause() {
  return rRCEstablishmentCause.getRRCEstablishmentCause();
}

//------------------------------------------------------------------------------
int InitialUEMessageMsg::getUeContextRequest() {
  if (uEContextRequest) {
    return uEContextRequest->getUEContextRequest();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::get5GS_TMSI(std::string& _5GsTmsi) {
  if (fivegSTmsi) {
    fivegSTmsi->getValue(_5GsTmsi);
    return true;
  } else
    return false;
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::get5GS_TMSI(
    std ::string& setid, std ::string& pointer, std ::string& tmsi) {
  if (fivegSTmsi) {
    fivegSTmsi->getValue(setid, pointer, tmsi);
    return true;
  } else
    return false;
}

}  // namespace ngap
