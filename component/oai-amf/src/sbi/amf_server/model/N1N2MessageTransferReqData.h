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
 * N1N2MessageTransferReqData.h
 *
 *
 */

#ifndef N1N2MessageTransferReqData_H_
#define N1N2MessageTransferReqData_H_

#include "N1MessageContainer.h"
#include <string>
#include "N2InfoContainer.h"
#include "Arp.h"
#include "AreaOfValidity.h"
#include <nlohmann/json.hpp>

namespace oai {
namespace amf {
namespace model {

/// <summary>
///
/// </summary>
class N1N2MessageTransferReqData {
 public:
  N1N2MessageTransferReqData();
  virtual ~N1N2MessageTransferReqData();

  void validate();

  /////////////////////////////////////////////
  /// N1N2MessageTransferReqData members

  /// <summary>
  ///
  /// </summary>
  N1MessageContainer getN1MessageContainer() const;
  void setN1MessageContainer(N1MessageContainer const& value);
  bool n1MessageContainerIsSet() const;
  void unsetN1MessageContainer();
  /// <summary>
  ///
  /// </summary>
  N2InfoContainer getN2InfoContainer() const;
  void setN2InfoContainer(N2InfoContainer const& value);
  bool n2InfoContainerIsSet() const;
  void unsetN2InfoContainer();
  /// <summary>
  ///
  /// </summary>
  bool isSkipInd() const;
  void setSkipInd(bool const value);
  bool skipIndIsSet() const;
  void unsetSkipInd();
  /// <summary>
  ///
  /// </summary>
  bool isLastMsgIndication() const;
  void setLastMsgIndication(bool const value);
  bool lastMsgIndicationIsSet() const;
  void unsetLastMsgIndication();
  /// <summary>
  ///
  /// </summary>
  int32_t getPduSessionId() const;
  void setPduSessionId(int32_t const value);
  bool pduSessionIdIsSet() const;
  void unsetPduSessionId();
  /// <summary>
  ///
  /// </summary>
  std::string getLcsCorrelationId() const;
  void setLcsCorrelationId(std::string const& value);
  bool lcsCorrelationIdIsSet() const;
  void unsetLcsCorrelationId();
  /// <summary>
  ///
  /// </summary>
  int32_t getPpi() const;
  void setPpi(int32_t const value);
  bool ppiIsSet() const;
  void unsetPpi();
  /// <summary>
  ///
  /// </summary>
  Arp getArp() const;
  void setArp(Arp const& value);
  bool arpIsSet() const;
  void unsetArp();
  /// <summary>
  ///
  /// </summary>
  int32_t get5qi() const;
  void set5qi(int32_t const value);
  bool _5qiIsSet() const;
  void unset_5qi();
  /// <summary>
  ///
  /// </summary>
  std::string getN1n2FailureTxfNotifURI() const;
  void setN1n2FailureTxfNotifURI(std::string const& value);
  bool n1n2FailureTxfNotifURIIsSet() const;
  void unsetN1n2FailureTxfNotifURI();
  /// <summary>
  ///
  /// </summary>
  bool isSmfReallocationInd() const;
  void setSmfReallocationInd(bool const value);
  bool smfReallocationIndIsSet() const;
  void unsetSmfReallocationInd();
  /// <summary>
  ///
  /// </summary>
  AreaOfValidity getAreaOfValidity() const;
  void setAreaOfValidity(AreaOfValidity const& value);
  bool areaOfValidityIsSet() const;
  void unsetAreaOfValidity();
  /// <summary>
  ///
  /// </summary>
  std::string getSupportedFeatures() const;
  void setSupportedFeatures(std::string const& value);
  bool supportedFeaturesIsSet() const;
  void unsetSupportedFeatures();

  friend void to_json(nlohmann::json& j, const N1N2MessageTransferReqData& o);
  friend void from_json(const nlohmann::json& j, N1N2MessageTransferReqData& o);

 protected:
  N1MessageContainer m_N1MessageContainer;
  bool m_N1MessageContainerIsSet;
  N2InfoContainer m_N2InfoContainer;
  bool m_N2InfoContainerIsSet;
  bool m_SkipInd;
  bool m_SkipIndIsSet;
  bool m_LastMsgIndication;
  bool m_LastMsgIndicationIsSet;
  int32_t m_PduSessionId;
  bool m_PduSessionIdIsSet;
  std::string m_LcsCorrelationId;
  bool m_LcsCorrelationIdIsSet;
  int32_t m_Ppi;
  bool m_PpiIsSet;
  Arp m_Arp;
  bool m_ArpIsSet;
  int32_t m__5qi;
  bool m__5qiIsSet;
  std::string m_N1n2FailureTxfNotifURI;
  bool m_N1n2FailureTxfNotifURIIsSet;
  bool m_SmfReallocationInd;
  bool m_SmfReallocationIndIsSet;
  AreaOfValidity m_AreaOfValidity;
  bool m_AreaOfValidityIsSet;
  std::string m_SupportedFeatures;
  bool m_SupportedFeaturesIsSet;
};

}  // namespace model
}  // namespace amf
}  // namespace oai

#endif /* N1N2MessageTransferReqData_H_ */
