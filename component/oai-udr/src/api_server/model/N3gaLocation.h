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
/**
 * Nudr_DataRepository API OpenAPI file
 * Unified Data Repository Service. © 2020, 3GPP Organizational Partners (ARIB,
 * ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 2.1.2
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */
/*
 * N3gaLocation.h
 *
 *
 */

#ifndef N3gaLocation_H_
#define N3gaLocation_H_

#include <nlohmann/json.hpp>
#include <string>

#include "HfcNodeId.h"
#include "Ipv6Addr.h"
#include "LineType.h"
#include "Tai.h"
#include "TnapId.h"
#include "TwapId.h"

namespace oai::udr::model {

/// <summary>
///
/// </summary>
class N3gaLocation {
 public:
  N3gaLocation();
  virtual ~N3gaLocation();

  void validate();

  /////////////////////////////////////////////
  /// N3gaLocation members

  /// <summary>
  ///
  /// </summary>
  Tai getN3gppTai() const;
  void setN3gppTai(Tai const& value);
  bool n3gppTaiIsSet() const;
  void unsetN3gppTai();
  /// <summary>
  ///
  /// </summary>
  std::string getN3IwfId() const;
  void setN3IwfId(std::string const& value);
  bool n3IwfIdIsSet() const;
  void unsetN3IwfId();
  /// <summary>
  ///
  /// </summary>
  std::string getUeIpv4Addr() const;
  void setUeIpv4Addr(std::string const& value);
  bool ueIpv4AddrIsSet() const;
  void unsetUeIpv4Addr();
  /// <summary>
  ///
  /// </summary>
  Ipv6Addr getUeIpv6Addr() const;
  void setUeIpv6Addr(Ipv6Addr const& value);
  bool ueIpv6AddrIsSet() const;
  void unsetUeIpv6Addr();
  /// <summary>
  ///
  /// </summary>
  int32_t getPortNumber() const;
  void setPortNumber(int32_t const value);
  bool portNumberIsSet() const;
  void unsetPortNumber();
  /// <summary>
  ///
  /// </summary>
  TnapId getTnapId() const;
  void setTnapId(TnapId const& value);
  bool tnapIdIsSet() const;
  void unsetTnapId();
  /// <summary>
  ///
  /// </summary>
  TwapId getTwapId() const;
  void setTwapId(TwapId const& value);
  bool twapIdIsSet() const;
  void unsetTwapId();
  /// <summary>
  ///
  /// </summary>
  HfcNodeId getHfcNodeId() const;
  void setHfcNodeId(HfcNodeId const& value);
  bool hfcNodeIdIsSet() const;
  void unsetHfcNodeId();
  /// <summary>
  ///
  /// </summary>
  std::string getGli() const;
  void setGli(std::string const& value);
  bool gliIsSet() const;
  void unsetGli();
  /// <summary>
  ///
  /// </summary>
  LineType getW5gbanLineType() const;
  void setW5gbanLineType(LineType const& value);
  bool w5gbanLineTypeIsSet() const;
  void unsetW5gbanLineType();
  /// <summary>
  ///
  /// </summary>
  std::string getGci() const;
  void setGci(std::string const& value);
  bool gciIsSet() const;
  void unsetGci();

  friend void to_json(nlohmann::json& j, const N3gaLocation& o);
  friend void from_json(const nlohmann::json& j, N3gaLocation& o);

 protected:
  Tai m_N3gppTai;
  bool m_N3gppTaiIsSet;
  std::string m_N3IwfId;
  bool m_N3IwfIdIsSet;
  std::string m_UeIpv4Addr;
  bool m_UeIpv4AddrIsSet;
  Ipv6Addr m_UeIpv6Addr;
  bool m_UeIpv6AddrIsSet;
  int32_t m_PortNumber;
  bool m_PortNumberIsSet;
  TnapId m_TnapId;
  bool m_TnapIdIsSet;
  TwapId m_TwapId;
  bool m_TwapIdIsSet;
  HfcNodeId m_HfcNodeId;
  bool m_HfcNodeIdIsSet;
  std::string m_Gli;
  bool m_GliIsSet;
  LineType m_W5gbanLineType;
  bool m_W5gbanLineTypeIsSet;
  std::string m_Gci;
  bool m_GciIsSet;
};

}  // namespace oai::udr::model

#endif /* N3gaLocation_H_ */
