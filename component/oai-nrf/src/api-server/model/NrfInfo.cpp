/**
 * NRF NFManagement Service
 * NRF NFManagement Service. © 2019, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */

#include "NrfInfo.h"

namespace oai {
namespace nrf {
namespace model {

NrfInfo::NrfInfo() {
  m_ServedUdrInfoIsSet   = false;
  m_ServedUdmInfoIsSet   = false;
  m_ServedAusfInfoIsSet  = false;
  m_ServedAmfInfoIsSet   = false;
  m_ServedSmfInfoIsSet   = false;
  m_ServedUpfInfoIsSet   = false;
  m_ServedPcfInfoIsSet   = false;
  m_ServedBsfInfoIsSet   = false;
  m_ServedChfInfoIsSet   = false;
  m_ServedNwdafInfoIsSet = false;
}

NrfInfo::~NrfInfo() {}

void NrfInfo::validate() {
  // TODO: implement validation
}

void to_json(nlohmann::json& j, const NrfInfo& o) {
  j = nlohmann::json();
  if (o.servedUdrInfoIsSet() || !o.m_ServedUdrInfo.empty())
    j["servedUdrInfo"] = o.m_ServedUdrInfo;
  if (o.servedUdmInfoIsSet() || !o.m_ServedUdmInfo.empty())
    j["servedUdmInfo"] = o.m_ServedUdmInfo;
  if (o.servedAusfInfoIsSet() || !o.m_ServedAusfInfo.empty())
    j["servedAusfInfo"] = o.m_ServedAusfInfo;
  if (o.servedAmfInfoIsSet() || !o.m_ServedAmfInfo.empty())
    j["servedAmfInfo"] = o.m_ServedAmfInfo;
  if (o.servedSmfInfoIsSet() || !o.m_ServedSmfInfo.empty())
    j["servedSmfInfo"] = o.m_ServedSmfInfo;
  if (o.servedUpfInfoIsSet() || !o.m_ServedUpfInfo.empty())
    j["servedUpfInfo"] = o.m_ServedUpfInfo;
  if (o.servedPcfInfoIsSet() || !o.m_ServedPcfInfo.empty())
    j["servedPcfInfo"] = o.m_ServedPcfInfo;
  if (o.servedBsfInfoIsSet() || !o.m_ServedBsfInfo.empty())
    j["servedBsfInfo"] = o.m_ServedBsfInfo;
  if (o.servedChfInfoIsSet() || !o.m_ServedChfInfo.empty())
    j["servedChfInfo"] = o.m_ServedChfInfo;
  if (o.servedNwdafInfoIsSet() || !o.m_ServedNwdafInfo.empty())
    j["servedNwdafInfo"] = o.m_ServedNwdafInfo;
}

void from_json(const nlohmann::json& j, NrfInfo& o) {
  if (j.find("servedUdrInfo") != j.end()) {
    j.at("servedUdrInfo").get_to(o.m_ServedUdrInfo);
    o.m_ServedUdrInfoIsSet = true;
  }
  if (j.find("servedUdmInfo") != j.end()) {
    j.at("servedUdmInfo").get_to(o.m_ServedUdmInfo);
    o.m_ServedUdmInfoIsSet = true;
  }
  if (j.find("servedAusfInfo") != j.end()) {
    j.at("servedAusfInfo").get_to(o.m_ServedAusfInfo);
    o.m_ServedAusfInfoIsSet = true;
  }
  if (j.find("servedAmfInfo") != j.end()) {
    j.at("servedAmfInfo").get_to(o.m_ServedAmfInfo);
    o.m_ServedAmfInfoIsSet = true;
  }
  if (j.find("servedSmfInfo") != j.end()) {
    j.at("servedSmfInfo").get_to(o.m_ServedSmfInfo);
    o.m_ServedSmfInfoIsSet = true;
  }
  if (j.find("servedUpfInfo") != j.end()) {
    j.at("servedUpfInfo").get_to(o.m_ServedUpfInfo);
    o.m_ServedUpfInfoIsSet = true;
  }
  if (j.find("servedPcfInfo") != j.end()) {
    j.at("servedPcfInfo").get_to(o.m_ServedPcfInfo);
    o.m_ServedPcfInfoIsSet = true;
  }
  if (j.find("servedBsfInfo") != j.end()) {
    j.at("servedBsfInfo").get_to(o.m_ServedBsfInfo);
    o.m_ServedBsfInfoIsSet = true;
  }
  if (j.find("servedChfInfo") != j.end()) {
    j.at("servedChfInfo").get_to(o.m_ServedChfInfo);
    o.m_ServedChfInfoIsSet = true;
  }
  if (j.find("servedNwdafInfo") != j.end()) {
    j.at("servedNwdafInfo").get_to(o.m_ServedNwdafInfo);
    o.m_ServedNwdafInfoIsSet = true;
  }
}

std::map<std::string, UdrInfo>& NrfInfo::getServedUdrInfo() {
  return m_ServedUdrInfo;
}
void NrfInfo::setServedUdrInfo(std::map<std::string, UdrInfo> const& value) {
  m_ServedUdrInfo      = value;
  m_ServedUdrInfoIsSet = true;
}
bool NrfInfo::servedUdrInfoIsSet() const {
  return m_ServedUdrInfoIsSet;
}
void NrfInfo::unsetServedUdrInfo() {
  m_ServedUdrInfoIsSet = false;
}
std::map<std::string, UdmInfo>& NrfInfo::getServedUdmInfo() {
  return m_ServedUdmInfo;
}
void NrfInfo::setServedUdmInfo(std::map<std::string, UdmInfo> const& value) {
  m_ServedUdmInfo      = value;
  m_ServedUdmInfoIsSet = true;
}
bool NrfInfo::servedUdmInfoIsSet() const {
  return m_ServedUdmInfoIsSet;
}
void NrfInfo::unsetServedUdmInfo() {
  m_ServedUdmInfoIsSet = false;
}
std::map<std::string, AusfInfo>& NrfInfo::getServedAusfInfo() {
  return m_ServedAusfInfo;
}
void NrfInfo::setServedAusfInfo(std::map<std::string, AusfInfo> const& value) {
  m_ServedAusfInfo      = value;
  m_ServedAusfInfoIsSet = true;
}
bool NrfInfo::servedAusfInfoIsSet() const {
  return m_ServedAusfInfoIsSet;
}
void NrfInfo::unsetServedAusfInfo() {
  m_ServedAusfInfoIsSet = false;
}
std::map<std::string, AmfInfo>& NrfInfo::getServedAmfInfo() {
  return m_ServedAmfInfo;
}
void NrfInfo::setServedAmfInfo(std::map<std::string, AmfInfo> const& value) {
  m_ServedAmfInfo      = value;
  m_ServedAmfInfoIsSet = true;
}
bool NrfInfo::servedAmfInfoIsSet() const {
  return m_ServedAmfInfoIsSet;
}
void NrfInfo::unsetServedAmfInfo() {
  m_ServedAmfInfoIsSet = false;
}
std::map<std::string, SmfInfo>& NrfInfo::getServedSmfInfo() {
  return m_ServedSmfInfo;
}
void NrfInfo::setServedSmfInfo(std::map<std::string, SmfInfo> const& value) {
  m_ServedSmfInfo      = value;
  m_ServedSmfInfoIsSet = true;
}
bool NrfInfo::servedSmfInfoIsSet() const {
  return m_ServedSmfInfoIsSet;
}
void NrfInfo::unsetServedSmfInfo() {
  m_ServedSmfInfoIsSet = false;
}
std::map<std::string, UpfInfo>& NrfInfo::getServedUpfInfo() {
  return m_ServedUpfInfo;
}
void NrfInfo::setServedUpfInfo(std::map<std::string, UpfInfo> const& value) {
  m_ServedUpfInfo      = value;
  m_ServedUpfInfoIsSet = true;
}
bool NrfInfo::servedUpfInfoIsSet() const {
  return m_ServedUpfInfoIsSet;
}
void NrfInfo::unsetServedUpfInfo() {
  m_ServedUpfInfoIsSet = false;
}
std::map<std::string, PcfInfo>& NrfInfo::getServedPcfInfo() {
  return m_ServedPcfInfo;
}
void NrfInfo::setServedPcfInfo(std::map<std::string, PcfInfo> const& value) {
  m_ServedPcfInfo      = value;
  m_ServedPcfInfoIsSet = true;
}
bool NrfInfo::servedPcfInfoIsSet() const {
  return m_ServedPcfInfoIsSet;
}
void NrfInfo::unsetServedPcfInfo() {
  m_ServedPcfInfoIsSet = false;
}
std::map<std::string, BsfInfo>& NrfInfo::getServedBsfInfo() {
  return m_ServedBsfInfo;
}
void NrfInfo::setServedBsfInfo(std::map<std::string, BsfInfo> const& value) {
  m_ServedBsfInfo      = value;
  m_ServedBsfInfoIsSet = true;
}
bool NrfInfo::servedBsfInfoIsSet() const {
  return m_ServedBsfInfoIsSet;
}
void NrfInfo::unsetServedBsfInfo() {
  m_ServedBsfInfoIsSet = false;
}
std::map<std::string, ChfInfo>& NrfInfo::getServedChfInfo() {
  return m_ServedChfInfo;
}
void NrfInfo::setServedChfInfo(std::map<std::string, ChfInfo> const& value) {
  m_ServedChfInfo      = value;
  m_ServedChfInfoIsSet = true;
}
bool NrfInfo::servedChfInfoIsSet() const {
  return m_ServedChfInfoIsSet;
}
void NrfInfo::unsetServedChfInfo() {
  m_ServedChfInfoIsSet = false;
}
std::map<std::string, NwdafInfo>& NrfInfo::getServedNwdafInfo() {
  return m_ServedNwdafInfo;
}
void NrfInfo::setServedNwdafInfo(
    std::map<std::string, NwdafInfo> const& value) {
  m_ServedNwdafInfo      = value;
  m_ServedNwdafInfoIsSet = true;
}
bool NrfInfo::servedNwdafInfoIsSet() const {
  return m_ServedNwdafInfoIsSet;
}
void NrfInfo::unsetServedNwdafInfo() {
  m_ServedNwdafInfoIsSet = false;
}

}  // namespace model
}  // namespace nrf
}  // namespace oai
