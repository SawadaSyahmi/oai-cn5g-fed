/**
 * AUSF API
 * AUSF UE Authentication Service. © 2020, 3GPP Organizational Partners (ARIB,
 * ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.1
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */

/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

#include "UEAuthenticationCtx.h"

namespace oai {
namespace ausf_server {
namespace model {

UEAuthenticationCtx::UEAuthenticationCtx() {
  m_ServingNetworkName      = "";
  m_ServingNetworkNameIsSet = false;
}

UEAuthenticationCtx::~UEAuthenticationCtx() {}

void UEAuthenticationCtx::validate() {
  // TODO: implement validation
}

void to_json(nlohmann::json& j, const UEAuthenticationCtx& o) {
  j               = nlohmann::json();
  j["authType"]   = o.m_AuthType;
  j["5gAuthData"] = o.m_r_5gAuthData;
  j["_links"]     = o.m__links;
  if (o.servingNetworkNameIsSet())
    j["servingNetworkName"] = o.m_ServingNetworkName;
}

void from_json(const nlohmann::json& j, UEAuthenticationCtx& o) {
  j.at("authType").get_to(o.m_AuthType);
  j.at("5gAuthData").get_to(o.m_r_5gAuthData);
  j.at("_links").get_to(o.m__links);
  if (j.find("servingNetworkName") != j.end()) {
    j.at("servingNetworkName").get_to(o.m_ServingNetworkName);
    o.m_ServingNetworkNameIsSet = true;
  }
}

std::string UEAuthenticationCtx::getAuthType() const {
  return m_AuthType;
}
void UEAuthenticationCtx::setAuthType(std::string const& value) {
  m_AuthType = value;
}
Av5gAka UEAuthenticationCtx::getR5gAuthData() const {
  return m_r_5gAuthData;
}
void UEAuthenticationCtx::setR5gAuthData(Av5gAka const& value) {
  m_r_5gAuthData = value;
}
std::map<std::string, LinksValueSchema>& UEAuthenticationCtx::getLinks() {
  return m__links;
}
void UEAuthenticationCtx::setLinks(
    std::map<std::string, LinksValueSchema> const& value) {
  m__links = value;
}
std::string UEAuthenticationCtx::getServingNetworkName() const {
  return m_ServingNetworkName;
}
void UEAuthenticationCtx::setServingNetworkName(std::string const& value) {
  m_ServingNetworkName      = value;
  m_ServingNetworkNameIsSet = true;
}
bool UEAuthenticationCtx::servingNetworkNameIsSet() const {
  return m_ServingNetworkNameIsSet;
}
void UEAuthenticationCtx::unsetServingNetworkName() {
  m_ServingNetworkNameIsSet = false;
}

}  // namespace model
}  // namespace ausf_server
}  // namespace oai
