/**
 * Nsmf_PDUSession
 * SMF PDU Session Service. © 2019, 3GPP Organizational Partners (ARIB, ATIS,
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
 * QosFlowAddModifyRequestItem.h
 *
 *
 */

#ifndef QosFlowAddModifyRequestItem_H_
#define QosFlowAddModifyRequestItem_H_

#include "QosFlowProfile.h"
#include <string>
#include <nlohmann/json.hpp>

namespace oai {
namespace smf_server {
namespace model {

/// <summary>
///
/// </summary>
class QosFlowAddModifyRequestItem {
 public:
  QosFlowAddModifyRequestItem();
  virtual ~QosFlowAddModifyRequestItem();

  void validate();

  /////////////////////////////////////////////
  /// QosFlowAddModifyRequestItem members

  /// <summary>
  ///
  /// </summary>
  int32_t getQfi() const;
  void setQfi(int32_t const value);
  /// <summary>
  ///
  /// </summary>
  int32_t getEbi() const;
  void setEbi(int32_t const value);
  bool ebiIsSet() const;
  void unsetEbi();
  /// <summary>
  ///
  /// </summary>
  std::string getQosRules() const;
  void setQosRules(std::string const& value);
  bool qosRulesIsSet() const;
  void unsetQosRules();
  /// <summary>
  ///
  /// </summary>
  std::string getQosFlowDescription() const;
  void setQosFlowDescription(std::string const& value);
  bool qosFlowDescriptionIsSet() const;
  void unsetQosFlowDescription();
  /// <summary>
  ///
  /// </summary>
  QosFlowProfile getQosFlowProfile() const;
  void setQosFlowProfile(QosFlowProfile const& value);
  bool qosFlowProfileIsSet() const;
  void unsetQosFlowProfile();

  friend void to_json(nlohmann::json& j, const QosFlowAddModifyRequestItem& o);
  friend void from_json(
      const nlohmann::json& j, QosFlowAddModifyRequestItem& o);

 protected:
  int32_t m_Qfi;

  int32_t m_Ebi;
  bool m_EbiIsSet;
  std::string m_QosRules;
  bool m_QosRulesIsSet;
  std::string m_QosFlowDescription;
  bool m_QosFlowDescriptionIsSet;
  QosFlowProfile m_QosFlowProfile;
  bool m_QosFlowProfileIsSet;
};

}  // namespace model
}  // namespace smf_server
}  // namespace oai

#endif /* QosFlowAddModifyRequestItem_H_ */
