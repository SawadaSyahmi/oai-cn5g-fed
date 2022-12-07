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
 * RequestIndication.h
 *
 * Possible values are - UE_REQ_PDU_SES_MOD - UE_REQ_PDU_SES_REL - PDU_SES_MOB -
 * NW_REQ_PDU_SES_AUTH - NW_REQ_PDU_SES_MOD - NW_REQ_PDU_SES_REL -
 * EBI_ASSIGNMENT_REQ
 */

#ifndef RequestIndication_H_
#define RequestIndication_H_

#include <nlohmann/json.hpp>

namespace oai {
namespace smf_server {
namespace model {

/// <summary>
/// Possible values are - UE_REQ_PDU_SES_MOD - UE_REQ_PDU_SES_REL - PDU_SES_MOB
/// - NW_REQ_PDU_SES_AUTH - NW_REQ_PDU_SES_MOD - NW_REQ_PDU_SES_REL -
/// EBI_ASSIGNMENT_REQ
/// </summary>
class RequestIndication {
 public:
  RequestIndication();
  virtual ~RequestIndication();

  void validate();

  /////////////////////////////////////////////
  /// RequestIndication members

  friend void to_json(nlohmann::json& j, const RequestIndication& o);
  friend void from_json(const nlohmann::json& j, RequestIndication& o);

 protected:
};

}  // namespace model
}  // namespace smf_server
}  // namespace oai

#endif /* RequestIndication_H_ */
