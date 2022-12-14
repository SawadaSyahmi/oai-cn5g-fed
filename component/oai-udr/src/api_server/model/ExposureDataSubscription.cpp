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

#include "ExposureDataSubscription.h"

namespace oai::udr::model {

ExposureDataSubscription::ExposureDataSubscription() {
  m_NotificationUri        = "";
  m_Expiry                 = "";
  m_ExpiryIsSet            = false;
  m_SupportedFeatures      = "";
  m_SupportedFeaturesIsSet = false;
}

ExposureDataSubscription::~ExposureDataSubscription() {}

void ExposureDataSubscription::validate() {
  // TODO: implement validation
}

void to_json(nlohmann::json& j, const ExposureDataSubscription& o) {
  j                          = nlohmann::json();
  j["notificationUri"]       = o.m_NotificationUri;
  j["monitoredResourceUris"] = o.m_MonitoredResourceUris;
  if (o.expiryIsSet()) j["expiry"] = o.m_Expiry;
  if (o.supportedFeaturesIsSet())
    j["supportedFeatures"] = o.m_SupportedFeatures;
}

void from_json(const nlohmann::json& j, ExposureDataSubscription& o) {
  j.at("notificationUri").get_to(o.m_NotificationUri);
  j.at("monitoredResourceUris").get_to(o.m_MonitoredResourceUris);
  if (j.find("expiry") != j.end()) {
    j.at("expiry").get_to(o.m_Expiry);
    o.m_ExpiryIsSet = true;
  }
  if (j.find("supportedFeatures") != j.end()) {
    j.at("supportedFeatures").get_to(o.m_SupportedFeatures);
    o.m_SupportedFeaturesIsSet = true;
  }
}

std::string ExposureDataSubscription::getNotificationUri() const {
  return m_NotificationUri;
}
void ExposureDataSubscription::setNotificationUri(std::string const& value) {
  m_NotificationUri = value;
}
std::vector<std::string>& ExposureDataSubscription::getMonitoredResourceUris() {
  return m_MonitoredResourceUris;
}
void ExposureDataSubscription::setMonitoredResourceUris(
    std::vector<std::string> const& value) {
  m_MonitoredResourceUris = value;
}
std::string ExposureDataSubscription::getExpiry() const {
  return m_Expiry;
}
void ExposureDataSubscription::setExpiry(std::string const& value) {
  m_Expiry      = value;
  m_ExpiryIsSet = true;
}
bool ExposureDataSubscription::expiryIsSet() const {
  return m_ExpiryIsSet;
}
void ExposureDataSubscription::unsetExpiry() {
  m_ExpiryIsSet = false;
}
std::string ExposureDataSubscription::getSupportedFeatures() const {
  return m_SupportedFeatures;
}
void ExposureDataSubscription::setSupportedFeatures(std::string const& value) {
  m_SupportedFeatures      = value;
  m_SupportedFeaturesIsSet = true;
}
bool ExposureDataSubscription::supportedFeaturesIsSet() const {
  return m_SupportedFeaturesIsSet;
}
void ExposureDataSubscription::unsetSupportedFeatures() {
  m_SupportedFeaturesIsSet = false;
}

}  // namespace oai::udr::model
