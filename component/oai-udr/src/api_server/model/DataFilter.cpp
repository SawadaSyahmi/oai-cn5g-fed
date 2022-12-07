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

#include "DataFilter.h"

namespace oai::udr::model {

DataFilter::DataFilter() {
  m_DnnsIsSet             = false;
  m_SnssaisIsSet          = false;
  m_InternalGroupIdsIsSet = false;
  m_SupisIsSet            = false;
  m_AppIdsIsSet           = false;
  m_UeIpv4sIsSet          = false;
  m_UeIpv6sIsSet          = false;
  m_UeMacsIsSet           = false;
}

DataFilter::~DataFilter() {}

void DataFilter::validate() {
  // TODO: implement validation
}

void to_json(nlohmann::json& j, const DataFilter& o) {
  j            = nlohmann::json();
  j["dataInd"] = o.m_DataInd;
  if (o.dnnsIsSet() || !o.m_Dnns.empty()) j["dnns"] = o.m_Dnns;
  if (o.snssaisIsSet() || !o.m_Snssais.empty()) j["snssais"] = o.m_Snssais;
  if (o.internalGroupIdsIsSet() || !o.m_InternalGroupIds.empty())
    j["internalGroupIds"] = o.m_InternalGroupIds;
  if (o.supisIsSet() || !o.m_Supis.empty()) j["supis"] = o.m_Supis;
  if (o.appIdsIsSet() || !o.m_AppIds.empty()) j["appIds"] = o.m_AppIds;
  if (o.ueIpv4sIsSet() || !o.m_UeIpv4s.empty()) j["ueIpv4s"] = o.m_UeIpv4s;
  if (o.ueIpv6sIsSet() || !o.m_UeIpv6s.empty()) j["ueIpv6s"] = o.m_UeIpv6s;
  if (o.ueMacsIsSet() || !o.m_UeMacs.empty()) j["ueMacs"] = o.m_UeMacs;
}

void from_json(const nlohmann::json& j, DataFilter& o) {
  j.at("dataInd").get_to(o.m_DataInd);
  if (j.find("dnns") != j.end()) {
    j.at("dnns").get_to(o.m_Dnns);
    o.m_DnnsIsSet = true;
  }
  if (j.find("snssais") != j.end()) {
    j.at("snssais").get_to(o.m_Snssais);
    o.m_SnssaisIsSet = true;
  }
  if (j.find("internalGroupIds") != j.end()) {
    j.at("internalGroupIds").get_to(o.m_InternalGroupIds);
    o.m_InternalGroupIdsIsSet = true;
  }
  if (j.find("supis") != j.end()) {
    j.at("supis").get_to(o.m_Supis);
    o.m_SupisIsSet = true;
  }
  if (j.find("appIds") != j.end()) {
    j.at("appIds").get_to(o.m_AppIds);
    o.m_AppIdsIsSet = true;
  }
  if (j.find("ueIpv4s") != j.end()) {
    j.at("ueIpv4s").get_to(o.m_UeIpv4s);
    o.m_UeIpv4sIsSet = true;
  }
  if (j.find("ueIpv6s") != j.end()) {
    j.at("ueIpv6s").get_to(o.m_UeIpv6s);
    o.m_UeIpv6sIsSet = true;
  }
  if (j.find("ueMacs") != j.end()) {
    j.at("ueMacs").get_to(o.m_UeMacs);
    o.m_UeMacsIsSet = true;
  }
}

DataInd DataFilter::getDataInd() const {
  return m_DataInd;
}
void DataFilter::setDataInd(DataInd const& value) {
  m_DataInd = value;
}
std::vector<std::string>& DataFilter::getDnns() {
  return m_Dnns;
}
void DataFilter::setDnns(std::vector<std::string> const& value) {
  m_Dnns      = value;
  m_DnnsIsSet = true;
}
bool DataFilter::dnnsIsSet() const {
  return m_DnnsIsSet;
}
void DataFilter::unsetDnns() {
  m_DnnsIsSet = false;
}
std::vector<Snssai>& DataFilter::getSnssais() {
  return m_Snssais;
}
void DataFilter::setSnssais(std::vector<Snssai> const& value) {
  m_Snssais      = value;
  m_SnssaisIsSet = true;
}
bool DataFilter::snssaisIsSet() const {
  return m_SnssaisIsSet;
}
void DataFilter::unsetSnssais() {
  m_SnssaisIsSet = false;
}
std::vector<std::string>& DataFilter::getInternalGroupIds() {
  return m_InternalGroupIds;
}
void DataFilter::setInternalGroupIds(std::vector<std::string> const& value) {
  m_InternalGroupIds      = value;
  m_InternalGroupIdsIsSet = true;
}
bool DataFilter::internalGroupIdsIsSet() const {
  return m_InternalGroupIdsIsSet;
}
void DataFilter::unsetInternalGroupIds() {
  m_InternalGroupIdsIsSet = false;
}
std::vector<std::string>& DataFilter::getSupis() {
  return m_Supis;
}
void DataFilter::setSupis(std::vector<std::string> const& value) {
  m_Supis      = value;
  m_SupisIsSet = true;
}
bool DataFilter::supisIsSet() const {
  return m_SupisIsSet;
}
void DataFilter::unsetSupis() {
  m_SupisIsSet = false;
}
std::vector<std::string>& DataFilter::getAppIds() {
  return m_AppIds;
}
void DataFilter::setAppIds(std::vector<std::string> const& value) {
  m_AppIds      = value;
  m_AppIdsIsSet = true;
}
bool DataFilter::appIdsIsSet() const {
  return m_AppIdsIsSet;
}
void DataFilter::unsetAppIds() {
  m_AppIdsIsSet = false;
}
std::vector<std::string>& DataFilter::getUeIpv4s() {
  return m_UeIpv4s;
}
void DataFilter::setUeIpv4s(std::vector<std::string> const& value) {
  m_UeIpv4s      = value;
  m_UeIpv4sIsSet = true;
}
bool DataFilter::ueIpv4sIsSet() const {
  return m_UeIpv4sIsSet;
}
void DataFilter::unsetUeIpv4s() {
  m_UeIpv4sIsSet = false;
}
std::vector<Ipv6Addr>& DataFilter::getUeIpv6s() {
  return m_UeIpv6s;
}
void DataFilter::setUeIpv6s(std::vector<Ipv6Addr> const& value) {
  m_UeIpv6s      = value;
  m_UeIpv6sIsSet = true;
}
bool DataFilter::ueIpv6sIsSet() const {
  return m_UeIpv6sIsSet;
}
void DataFilter::unsetUeIpv6s() {
  m_UeIpv6sIsSet = false;
}
std::vector<std::string>& DataFilter::getUeMacs() {
  return m_UeMacs;
}
void DataFilter::setUeMacs(std::vector<std::string> const& value) {
  m_UeMacs      = value;
  m_UeMacsIsSet = true;
}
bool DataFilter::ueMacsIsSet() const {
  return m_UeMacsIsSet;
}
void DataFilter::unsetUeMacs() {
  m_UeMacsIsSet = false;
}

}  // namespace oai::udr::model
