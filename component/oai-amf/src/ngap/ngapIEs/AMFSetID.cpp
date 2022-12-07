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

#include "AMFSetID.hpp"

#include "String2Value.hpp"

#include <string>

namespace ngap {

//------------------------------------------------------------------------------
AMFSetID::AMFSetID() {
  setId = 0;
}

//------------------------------------------------------------------------------
AMFSetID::~AMFSetID() {}

//------------------------------------------------------------------------------
void AMFSetID::setAMFSetID(const std::string& charid) {
  setId = fromString<uint16_t>(charid);
}

//------------------------------------------------------------------------------
void AMFSetID::setAMFSetID(const uint16_t& set_id) {
  setId = set_id;
}
//------------------------------------------------------------------------------
void AMFSetID::getAMFSetID(std::string& charid) {
  charid = std::to_string(setId);
}

//------------------------------------------------------------------------------
bool AMFSetID::encode2bitstring(Ngap_AMFSetID_t& amfSetId) {
  amfSetId.size   = 2;
  uint8_t* buffer = (uint8_t*) calloc(1, sizeof(uint16_t));
  if (!buffer) return false;
  //*(uint16_t *)buffer = setId & 0x3ff;
  buffer[0]            = ((setId & 0x03fc) >> 2);
  buffer[1]            = ((setId & 0x0003) << 6);
  amfSetId.buf         = buffer;
  amfSetId.bits_unused = 6;

  return true;
}

//------------------------------------------------------------------------------
bool AMFSetID::decodefrombitstring(Ngap_AMFSetID_t& amfSetId) {
  if (!amfSetId.buf) return false;
  for (int i = 0; i < amfSetId.size; i++) {
    printf("%x ", amfSetId.buf[i]);
  }
  printf("\n");
  uint16_t temp = 0;
  temp |= amfSetId.buf[0] << 8;
  temp |= amfSetId.buf[1];
  setId = (temp & 0xffc0) >> 6;  // 1111 1111 11 00 0000

  return true;
}
}  // namespace ngap
