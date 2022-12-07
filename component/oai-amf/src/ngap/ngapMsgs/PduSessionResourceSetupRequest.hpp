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

#ifndef _PDU_SESSION_RESOURCE_SETUP_REQUEST_H_
#define _PDU_SESSION_RESOURCE_SETUP_REQUEST_H_

#include "PDUSessionResourceSetupListSUReq.hpp"
#include "RANPagingPriority.hpp"
#include "UEAggregateMaxBitRate.hpp"
#include "NgapUEMessage.hpp"

extern "C" {
#include "Ngap_InitialContextSetupRequest.h"
}

namespace ngap {

class PduSessionResourceSetupRequestMsg : public NgapUEMessage {
 public:
  PduSessionResourceSetupRequestMsg();
  virtual ~PduSessionResourceSetupRequestMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setRanPagingPriority(const uint8_t& priority);
  int getRanPagingPriority();

  void setNasPdu(uint8_t* nas, size_t size);
  bool getNasPdu(uint8_t*& nas, size_t& size);

  void setPduSessionResourceSetupRequestList(
      const std::vector<PDUSessionResourceSetupRequestItem_t>& list);
  bool getPduSessionResourceSetupRequestList(
      std::vector<PDUSessionResourceSetupRequestItem_t>& list);

  void setUEAggregateMaxBitRate(
      const long& bit_rate_downlink, const long& bit_rate_uplink);
  void getUEAggregateMaxBitRate(long& bit_rate_downlink, long& bit_rate_uplink);

 private:
  Ngap_PDUSessionResourceSetupRequest_t* pduSessionResourceSetupRequestIEs;

  RANPagingPriority* ranPagingPriority;  // Optional
  NAS_PDU* nasPdu;                       // Optional
  PDUSessionResourceSetupListSUReq
      pduSessionResourceSetupRequestList;        // Mandatory
  UEAggregateMaxBitRate* uEAggregateMaxBitRate;  // Optional
};

}  // namespace ngap
#endif
