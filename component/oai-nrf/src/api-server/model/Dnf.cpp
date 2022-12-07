/**
 * NRF NFDiscovery Service
 * NRF NFDiscovery Service. © 2019, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */

#include "Dnf.h"

namespace oai {
namespace nrf {
namespace model {

Dnf::Dnf() {}

Dnf::~Dnf() {}

void Dnf::validate() {
  // TODO: implement validation
}

void to_json(nlohmann::json& j, const Dnf& o) {
  j             = nlohmann::json();
  j["dnfUnits"] = o.m_DnfUnits;
}

void from_json(const nlohmann::json& j, Dnf& o) {
  j.at("dnfUnits").get_to(o.m_DnfUnits);
}

std::vector<DnfUnit>& Dnf::getDnfUnits() {
  return m_DnfUnits;
}
void Dnf::setDnfUnits(std::vector<DnfUnit> const& value) {
  m_DnfUnits = value;
}

}  // namespace model
}  // namespace nrf
}  // namespace oai