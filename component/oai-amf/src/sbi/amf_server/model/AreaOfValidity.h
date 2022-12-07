/**
 * Namf_Communication
 * AMF Communication Service © 2019, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */
/*
 * AreaOfValidity.h
 *
 *
 */

#ifndef AreaOfValidity_H_
#define AreaOfValidity_H_

#include "Tai.h"
#include <vector>
#include <nlohmann/json.hpp>

namespace oai {
namespace amf {
namespace model {

/// <summary>
///
/// </summary>
class AreaOfValidity {
 public:
  AreaOfValidity();
  virtual ~AreaOfValidity();

  void validate();

  /////////////////////////////////////////////
  /// AreaOfValidity members

  /// <summary>
  ///
  /// </summary>
  std::vector<Tai>& getTaiList();

  friend void to_json(nlohmann::json& j, const AreaOfValidity& o);
  friend void from_json(const nlohmann::json& j, AreaOfValidity& o);

 protected:
  std::vector<Tai> m_TaiList;
};

}  // namespace model
}  // namespace amf
}  // namespace oai

#endif /* AreaOfValidity_H_ */
