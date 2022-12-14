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
 * ExternalUnrelatedClass.h
 *
 *
 */

#ifndef ExternalUnrelatedClass_H_
#define ExternalUnrelatedClass_H_

#include <nlohmann/json.hpp>
#include <vector>

#include "AfExternal.h"
#include "LcsClientExternal.h"
#include "LcsClientGroupExternal.h"

namespace oai::udr::model {

/// <summary>
///
/// </summary>
class ExternalUnrelatedClass {
 public:
  ExternalUnrelatedClass();
  virtual ~ExternalUnrelatedClass();

  void validate();

  /////////////////////////////////////////////
  /// ExternalUnrelatedClass members

  /// <summary>
  ///
  /// </summary>
  std::vector<LcsClientExternal>& getLcsClientExternals();
  void setLcsClientExternals(std::vector<LcsClientExternal> const& value);
  bool lcsClientExternalsIsSet() const;
  void unsetLcsClientExternals();
  /// <summary>
  ///
  /// </summary>
  std::vector<AfExternal>& getAfExternals();
  void setAfExternals(std::vector<AfExternal> const& value);
  bool afExternalsIsSet() const;
  void unsetAfExternals();
  /// <summary>
  ///
  /// </summary>
  std::vector<LcsClientGroupExternal>& getLcsClientGroupExternals();
  void setLcsClientGroupExternals(
      std::vector<LcsClientGroupExternal> const& value);
  bool lcsClientGroupExternalsIsSet() const;
  void unsetLcsClientGroupExternals();

  friend void to_json(nlohmann::json& j, const ExternalUnrelatedClass& o);
  friend void from_json(const nlohmann::json& j, ExternalUnrelatedClass& o);

 protected:
  std::vector<LcsClientExternal> m_LcsClientExternals;
  bool m_LcsClientExternalsIsSet;
  std::vector<AfExternal> m_AfExternals;
  bool m_AfExternalsIsSet;
  std::vector<LcsClientGroupExternal> m_LcsClientGroupExternals;
  bool m_LcsClientGroupExternalsIsSet;
};

}  // namespace oai::udr::model

#endif /* ExternalUnrelatedClass_H_ */
