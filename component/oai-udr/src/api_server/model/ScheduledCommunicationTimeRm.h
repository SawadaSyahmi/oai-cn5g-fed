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
 * ScheduledCommunicationTimeRm.h
 *
 *
 */

#ifndef ScheduledCommunicationTimeRm_H_
#define ScheduledCommunicationTimeRm_H_

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "NullValue.h"
#include "ScheduledCommunicationTime.h"

namespace oai::udr::model {

/// <summary>
///
/// </summary>
class ScheduledCommunicationTimeRm {
 public:
  ScheduledCommunicationTimeRm();
  virtual ~ScheduledCommunicationTimeRm();

  void validate();

  /////////////////////////////////////////////
  /// ScheduledCommunicationTimeRm members

  /// <summary>
  /// Identifies the day(s) of the week. If absent, it indicates every day of
  /// the week.
  /// </summary>
  std::vector<int32_t>& getDaysOfWeek();
  void setDaysOfWeek(std::vector<int32_t> const value);
  bool daysOfWeekIsSet() const;
  void unsetDaysOfWeek();
  /// <summary>
  /// String with format partial-time or full-time as defined in clause 5.6 of
  /// IETF RFC 3339. Examples, 20:15:00, 20:15:00-08:00 (for 8 hours behind
  /// UTC).
  /// </summary>
  std::string getTimeOfDayStart() const;
  void setTimeOfDayStart(std::string const& value);
  bool timeOfDayStartIsSet() const;
  void unsetTimeOfDayStart();
  /// <summary>
  /// String with format partial-time or full-time as defined in clause 5.6 of
  /// IETF RFC 3339. Examples, 20:15:00, 20:15:00-08:00 (for 8 hours behind
  /// UTC).
  /// </summary>
  std::string getTimeOfDayEnd() const;
  void setTimeOfDayEnd(std::string const& value);
  bool timeOfDayEndIsSet() const;
  void unsetTimeOfDayEnd();

  friend void to_json(nlohmann::json& j, const ScheduledCommunicationTimeRm& o);
  friend void from_json(
      const nlohmann::json& j, ScheduledCommunicationTimeRm& o);

 protected:
  std::vector<int32_t> m_DaysOfWeek;
  bool m_DaysOfWeekIsSet;
  std::string m_TimeOfDayStart;
  bool m_TimeOfDayStartIsSet;
  std::string m_TimeOfDayEnd;
  bool m_TimeOfDayEndIsSet;
};

}  // namespace oai::udr::model

#endif /* ScheduledCommunicationTimeRm_H_ */
