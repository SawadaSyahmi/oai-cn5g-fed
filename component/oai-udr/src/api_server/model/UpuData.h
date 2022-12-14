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
 * UpuData.h
 *
 *
 */

#ifndef UpuData_H_
#define UpuData_H_

#include <nlohmann/json.hpp>
#include <string>

#include "UeUpdateStatus.h"

namespace oai::udr::model {

/// <summary>
///
/// </summary>
class UpuData {
 public:
  UpuData();
  virtual ~UpuData();

  void validate();

  /////////////////////////////////////////////
  /// UpuData members

  /// <summary>
  ///
  /// </summary>
  std::string getProvisioningTime() const;
  void setProvisioningTime(std::string const& value);
  /// <summary>
  ///
  /// </summary>
  UeUpdateStatus getUeUpdateStatus() const;
  void setUeUpdateStatus(UeUpdateStatus const& value);
  /// <summary>
  ///
  /// </summary>
  std::string getUpuXmacIue() const;
  void setUpuXmacIue(std::string const& value);
  bool upuXmacIueIsSet() const;
  void unsetUpuXmacIue();
  /// <summary>
  ///
  /// </summary>
  std::string getUpuMacIue() const;
  void setUpuMacIue(std::string const& value);
  bool upuMacIueIsSet() const;
  void unsetUpuMacIue();

  friend void to_json(nlohmann::json& j, const UpuData& o);
  friend void from_json(const nlohmann::json& j, UpuData& o);

 protected:
  std::string m_ProvisioningTime;

  UeUpdateStatus m_UeUpdateStatus;

  std::string m_UpuXmacIue;
  bool m_UpuXmacIueIsSet;
  std::string m_UpuMacIue;
  bool m_UpuMacIueIsSet;
};

}  // namespace oai::udr::model

#endif /* UpuData_H_ */
