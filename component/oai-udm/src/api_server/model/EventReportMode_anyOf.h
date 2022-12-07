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
 * Nudm_EE
 * Nudm Event Exposure Service. © 2021, 3GPP Organizational Partners (ARIB,
 * ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.2.0-alpha.3
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */
/*
 * EventReportMode_anyOf.h
 *
 *
 */

#ifndef EventReportMode_anyOf_H_
#define EventReportMode_anyOf_H_

#include <nlohmann/json.hpp>

namespace oai::udm::model {

/// <summary>
///
/// </summary>
class EventReportMode_anyOf {
 public:
  EventReportMode_anyOf();
  virtual ~EventReportMode_anyOf() = default;

  enum class eEventReportMode_anyOf {
    // To have a valid default value.
    // Avoiding nameclashes with user defined
    // enum values
    INVALID_VALUE_OPENAPI_GENERATED = 0,
    PERIODIC,
    ON_EVENT_DETECTION
  };

  /// <summary>
  /// Validate the current data in the model. Throws a ValidationException on
  /// failure.
  /// </summary>
  void validate() const;

  /// <summary>
  /// Validate the current data in the model. Returns false on error and writes
  /// an error message into the given stringstream.
  /// </summary>
  bool validate(std::stringstream& msg) const;

  bool operator==(const EventReportMode_anyOf& rhs) const;
  bool operator!=(const EventReportMode_anyOf& rhs) const;

  /////////////////////////////////////////////
  /// EventReportMode_anyOf members

  EventReportMode_anyOf::eEventReportMode_anyOf getValue() const;
  void setValue(EventReportMode_anyOf::eEventReportMode_anyOf value);

  friend void to_json(nlohmann::json& j, const EventReportMode_anyOf& o);
  friend void from_json(const nlohmann::json& j, EventReportMode_anyOf& o);

 protected:
  EventReportMode_anyOf::eEventReportMode_anyOf m_value =
      EventReportMode_anyOf::eEventReportMode_anyOf::
          INVALID_VALUE_OPENAPI_GENERATED;

  // Helper overload for validate. Used when one model stores another model and
  // calls it's validate.
  bool validate(std::stringstream& msg, const std::string& pathPrefix) const;
};

}  // namespace oai::udm::model

#endif /* EventReportMode_anyOf_H_ */