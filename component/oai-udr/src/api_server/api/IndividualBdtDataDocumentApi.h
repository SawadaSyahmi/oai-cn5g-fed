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
/*
 * IndividualBdtDataDocumentApi.h
 *
 *
 */

#ifndef IndividualBdtDataDocumentApi_H_
#define IndividualBdtDataDocumentApi_H_

#include <pistache/http.h>
#include <pistache/http_headers.h>
#include <pistache/optional.h>
#include <pistache/router.h>

#include <string>

#include "BdtData.h"
#include "BdtDataPatch.h"
#include "ProblemDetails.h"

namespace oai::udr::api {

using namespace oai::udr::model;

class IndividualBdtDataDocumentApi {
 public:
  IndividualBdtDataDocumentApi(std::shared_ptr<Pistache::Rest::Router>);
  virtual ~IndividualBdtDataDocumentApi() {}
  void init();

  const std::string base = "/nudr-dr/";

 private:
  void setupRoutes();

  void create_individual_bdt_data_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);
  void delete_individual_bdt_data_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);
  void read_individual_bdt_data_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);
  void update_individual_bdt_data_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);
  void individual_bdt_data_document_api_default_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);

  std::shared_ptr<Pistache::Rest::Router> router;

  /// <summary>
  /// Creates an BDT data resource associated with an BDT reference Id
  /// </summary>
  /// <remarks>
  ///
  /// </remarks>
  /// <param name="bdtReferenceId"></param>
  /// <param name="bdtData"></param>
  virtual void create_individual_bdt_data(
      const std::string& bdtReferenceId, const BdtData& bdtData,
      Pistache::Http::ResponseWriter& response) = 0;

  /// <summary>
  /// Deletes an BDT data resource associated with an BDT reference Id
  /// </summary>
  /// <remarks>
  ///
  /// </remarks>
  /// <param name="bdtReferenceId"></param>
  virtual void delete_individual_bdt_data(
      const std::string& bdtReferenceId,
      Pistache::Http::ResponseWriter& response) = 0;

  /// <summary>
  /// Retrieves the BDT data information associated with a BDT reference Id
  /// </summary>
  /// <remarks>
  ///
  /// </remarks>
  /// <param name="bdtReferenceId"></param>
  /// <param name="suppFeat">Supported Features (optional, default to
  /// &quot;&quot;)</param>
  virtual void read_individual_bdt_data(
      const std::string& bdtReferenceId,
      const Pistache::Optional<std::string>& suppFeat,
      Pistache::Http::ResponseWriter& response) = 0;

  /// <summary>
  /// Modifies an BDT data resource associated with an BDT reference Id
  /// </summary>
  /// <remarks>
  ///
  /// </remarks>
  /// <param name="bdtReferenceId"></param>
  /// <param name="bdtDataPatch"></param>
  virtual void update_individual_bdt_data(
      const std::string& bdtReferenceId, const BdtDataPatch& bdtDataPatch,
      Pistache::Http::ResponseWriter& response) = 0;
};

}  // namespace oai::udr::api

#endif /* IndividualBdtDataDocumentApi_H_ */
