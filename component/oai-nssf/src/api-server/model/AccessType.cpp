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

#include "AccessType.h"

namespace oai {
namespace nssf_server {
namespace model {

AccessType::AccessType() {}

AccessType::~AccessType() {}

void AccessType::validate() {
  // TODO: implement validation
}

bool AccessType::operator==(const AccessType& rhs) const {
  return

      getValue() == rhs.getValue();
}

bool AccessType::operator!=(const AccessType& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const AccessType& o) {
  j = nlohmann::json();
  to_json(j, o.m_value);
}

void from_json(const nlohmann::json& j, AccessType& o) {
  from_json(j, o.m_value);
}

AccessType_anyOf AccessType::getValue() const {
  return m_value;
}

void AccessType::setValue(AccessType_anyOf value) {
  m_value = value;
}

AccessType_anyOf::eAccessType_anyOf AccessType::getEnumValue() const {
  return m_value.getValue();
}

void AccessType::setEnumValue(AccessType_anyOf::eAccessType_anyOf value) {
  m_value.setValue(value);
}

}  // namespace model
}  // namespace nssf_server
}  // namespace oai
