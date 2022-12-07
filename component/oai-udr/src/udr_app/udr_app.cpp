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

/*! \file udr_app.cpp
 \brief
 \author
 \company Eurecom
 \date 2020
 \email: contact@openairinterface.org
 */

#include "udr_app.hpp"

#include "3gpp_29.500.h"
#include "AccessAndMobilitySubscriptionData.h"
#include "AuthenticationSubscription.h"
#include "cassandra_db.hpp"
#include "logger.hpp"
#include "mysql_db.hpp"
#include "udr_config.hpp"
#include "udr_nrf.hpp"

using namespace oai::udr::app;
using namespace oai::udr::model;
using namespace oai::udr::config;

extern udr_app* udr_app_inst;
extern udr_config udr_cfg;
udr_nrf* udr_nrf_inst = nullptr;

//------------------------------------------------------------------------------
udr_app::udr_app(const std::string& config_file, udr_event& ev)
    : event_sub(ev) {
  Logger::udr_app().startup("Starting...");

  // Use the appropriate DB connector to initialize the connection to the DB
  if (udr_cfg.db_type == DB_TYPE_CASSANDRA) {
    db_connector = std::make_shared<cassandra_db>();
  } else {
    db_connector = std::make_shared<mysql_db>(ev);
  }

  if (!db_connector->initialize()) {
    Logger::udr_app().error("Error when initializing a connection with DB");
    return;
  }

  if (!db_connector->connect(MAX_FIRST_CONNECTION_RETRY)) {
    Logger::udr_app().warn("Could not establish the connection to the DB");
  }

  // Register to NRF
  if (udr_cfg.register_nrf) {
    try {
      udr_nrf_inst = new udr_nrf(ev);
      udr_nrf_inst->register_to_nrf();
      Logger::udr_app().info("NRF TASK Created ");
    } catch (std::exception& e) {
      Logger::udr_app().error("Cannot create NRF TASK: %s", e.what());
      throw;
    }
  }

  Logger::udr_app().startup("Started");
}

//------------------------------------------------------------------------------
udr_app::~udr_app() {
  Logger::udr_app().debug("Delete UDR APP instance...");
  // Close DB connection
  db_connector->close_connection();
}

//------------------------------------------------------------------------------
void udr_app::handle_query_am_data(const std::string& ue_id,
                                   const std::string& serving_plmn_id,
                                   nlohmann::json& response_data, long& code) {
  Logger::udr_app().info(
      "Retrieve the access and mobility subscription data of a UE");

  if (db_connector->query_am_data(ue_id, serving_plmn_id, response_data)) {
    code = HTTP_STATUS_CODE_200_OK;
    Logger::udr_app().info("AccessAndMobilitySubscriptionData Data: %s",
                           response_data.dump().c_str());
    // TODO: headers
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_create_amf_context_3gpp(
    const std::string& ue_id,
    Amf3GppAccessRegistration& amf3GppAccessRegistration,
    nlohmann::json& response_data, long& code) {
  Logger::udr_app().info(
      "Store the AMF context data of a UE using 3gpp access in the UDR");

  if (db_connector->create_amf_context_3gpp(ue_id, amf3GppAccessRegistration)) {
    code = HTTP_STATUS_CODE_201_CREATED;
    Logger::udr_app().info("Amf3GppAccessRegistration Data: %s",
                           response_data.dump().c_str());
    // TODO: Location
    // TODO: CODE 204
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_query_amf_context_3gpp(const std::string& ue_id,
                                            nlohmann::json& response_data,
                                            long& code) {
  Logger::udr_app().info(
      "Retrieve the AMF context data of a UE using 3gpp access");

  if (db_connector->query_amf_context_3gpp(ue_id, response_data)) {
    code = HTTP_STATUS_CODE_200_OK;
    Logger::udr_app().info("Amf3GppAccessRegistration Data: %s",
                           response_data.dump().c_str());
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_create_authentication_status(
    const std::string& ue_id, const oai::udr::model::AuthEvent& authEvent,
    nlohmann::json& response_data, long& code) {
  Logger::udr_app().info("Store the Authentication Status data of a UE");

  if (db_connector->insert_authentication_status(ue_id, authEvent,
                                                 response_data)) {
    code = HTTP_STATUS_CODE_204_NO_CONTENT;
    Logger::udr_app().info(
        "Successful stored the Authentication Status data of a UE");
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_delete_authentication_status(const std::string& ue_id,
                                                  nlohmann::json& response_data,
                                                  long& code) {
  Logger::udr_app().info("Remove the Authentication Status data of a UE");

  if (db_connector->delete_authentication_status(ue_id)) {
    code = HTTP_STATUS_CODE_204_NO_CONTENT;
    Logger::udr_app().info(
        "Successful removed the Authentication Status data of a UE");
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_query_authentication_status(const std::string& ue_id,
                                                 nlohmann::json& response_data,
                                                 long& code) {
  Logger::udr_app().info("Retrieve the Authentication Status data of a UE");

  if (db_connector->query_authentication_status(ue_id, response_data)) {
    code = HTTP_STATUS_CODE_200_OK;
    Logger::udr_app().info("AuthEvent Data: %s", response_data.dump().c_str());
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_create_authentication_data(
    const std::string& ue_id,
    const AuthenticationSubscription& authentication_subscription,
    nlohmann::json& response_data, long& code) {
  Logger::udr_app().info("Create an authentication subscription data of a UE");

  if (db_connector->insert_authentication_subscription(
          ue_id, authentication_subscription, response_data)) {
    code = HTTP_STATUS_CODE_201_CREATED;
    Logger::udr_app().info("AuthenticationSubscription: %s",
                           response_data.dump().c_str());
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_delete_authentication_data(const std::string& ue_id,
                                                nlohmann::json& response_data,
                                                long& code) {
  Logger::udr_app().info("Delete an authentication subscription data of a UE");

  if (db_connector->delete_authentication_subscription(ue_id)) {
    code = HTTP_STATUS_CODE_204_NO_CONTENT;
    Logger::udr_app().info(
        "Successful removed the authentication subscription data of a UE");
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_modify_authentication_subscription(
    const std::string& ue_id, const std::vector<PatchItem>& patchItem,
    nlohmann::json& response_data, long& code) {
  Logger::udr_app().info("Modify the authentication subscription data of a UE");

  if (db_connector->update_authentication_subscription(ue_id, patchItem,
                                                       response_data)) {
    code = HTTP_STATUS_CODE_204_NO_CONTENT;
    Logger::udr_app().info(
        "Successful modified the Authentication subscription data of a UE");
    // TODO: Code 200

  } else {
    code = HTTP_STATUS_CODE_403_FORBIDDEN;
    // TODO: problem details
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_read_authentication_subscription(
    const std::string& ue_id, nlohmann::json& response_data, long& code) {
  Logger::udr_app().info(
      "Retrieve the authentication subscription data of a UE");

  if (db_connector->query_authentication_subscription(ue_id, response_data)) {
    code = HTTP_STATUS_CODE_200_OK;
    Logger::udr_app().info("AuthenticationSubscription: %s",
                           response_data.dump().c_str());
  } else {
    code = HTTP_STATUS_CODE_404_NOT_FOUND;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_query_sdm_subscription(const std::string& ue_id,
                                            const std::string& subs_id,
                                            nlohmann::json& response_data,
                                            long& code) {
  Logger::udr_app().info(
      "Retrieve a individual sdmSubscription identified by subsId");

  if (db_connector->query_sdm_subscription(ue_id, subs_id, response_data)) {
    code = HTTP_STATUS_CODE_200_OK;
    Logger::udr_app().info("SdmSubscriptions: %s",
                           response_data.dump().c_str());
  } else {
    code = HTTP_STATUS_CODE_404_NOT_FOUND;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_remove_sdm_subscription(const std::string& ue_id,
                                             const std::string& subs_id,
                                             nlohmann::json& response_data,
                                             long& code) {
  Logger::udr_app().info("Delete a SDMSubscriptions");

  if (db_connector->delete_sdm_subscription(ue_id, subs_id)) {
    code = HTTP_STATUS_CODE_204_NO_CONTENT;
    Logger::udr_app().info("Successful removed a SDMSubscriptions");
  } else {
    code = HTTP_STATUS_CODE_404_NOT_FOUND;  // TODO
  }
  return;

  Logger::udr_app().debug("SdmSubscription DELETE - successful");
}

//------------------------------------------------------------------------------
void udr_app::handle_update_sdm_subscription(const std::string& ue_id,
                                             const std::string& subs_id,
                                             SdmSubscription& sdmSubscription,
                                             nlohmann::json& response_data,
                                             long& code) {
  Logger::udr_app().info("Update an individual SDM subscriptions of a UE");

  if (db_connector->update_sdm_subscription(ue_id, subs_id, sdmSubscription,
                                            response_data)) {
    code = HTTP_STATUS_CODE_204_NO_CONTENT;
    Logger::udr_app().info("Successful updated a SDMSubscription");
  } else {
    code = HTTP_STATUS_CODE_404_NOT_FOUND;
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_create_sdm_subscriptions(const std::string& ue_id,
                                              SdmSubscription& sdmSubscription,
                                              nlohmann::json& response_data,
                                              long& code) {
  Logger::udr_app().info("Create an individual SDM subscription");

  if (db_connector->create_sdm_subscriptions(ue_id, sdmSubscription,
                                             response_data)) {
    code = HTTP_STATUS_CODE_201_CREATED;
    Logger::udr_app().info("SdmSubscription: %s", response_data.dump().c_str());
    // TODO: Location
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_query_sdm_subscriptions(const std::string& ue_id,
                                             nlohmann::json& response_data,
                                             long& code) {
  Logger::udr_app().info("Retrieve the SDM subscriptions of a UE");

  if (db_connector->query_sdm_subscriptions(ue_id, response_data)) {
    code = HTTP_STATUS_CODE_200_OK;
    Logger::udr_app().info("SdmSubscriptions CREATE: %s",
                           response_data.dump().c_str());
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_query_sm_data(const std::string& ue_id,
                                   const std::string& serving_plmn_id,
                                   nlohmann::json& response_data, long& code,
                                   const oai::udr::model::Snssai& snssai,
                                   const std::string& dnn) {
  Logger::udr_app().info(
      "Retrieve the Session Management subscription data of a UE");

  if (db_connector->query_sm_data(ue_id, serving_plmn_id, response_data, snssai,
                                  dnn)) {
    code = HTTP_STATUS_CODE_200_OK;
    Logger::udr_app().info("SessionManagementSubscriptionData: %s",
                           response_data.dump().c_str());
    // TODO: Headers
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_create_smf_context_non_3gpp(
    const std::string& ue_id, const int32_t& pdu_session_id,
    const SmfRegistration& smfRegistration, nlohmann::json& response_data,
    long& code) {
  Logger::udr_app().info(
      "Create an individual SMF context data of a UE in the UDR");

  if (db_connector->insert_smf_context_non_3gpp(
          ue_id, pdu_session_id, smfRegistration, response_data)) {
    code = HTTP_STATUS_CODE_201_CREATED;
    Logger::udr_app().info("SmfRegistration: %s", response_data.dump().c_str());
    // TODO: Location
    // TODO: Code 204
  } else {
    code = HTTP_STATUS_CODE_404_NOT_FOUND;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_delete_smf_context(const std::string& ue_id,
                                        const int32_t& pdu_session_id,
                                        nlohmann::json& response_data,
                                        long& code) {
  Logger::udr_app().info(
      "Remove an individual SMF context data of a UE the UDR");

  if (db_connector->delete_smf_context(ue_id, pdu_session_id)) {
    code = HTTP_STATUS_CODE_204_NO_CONTENT;
    Logger::udr_app().info("Successful deleted SMF context data");
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_query_smf_registration(const std::string& ue_id,
                                            const int32_t& pdu_session_id,
                                            nlohmann::json& response_data,
                                            long& code) {
  Logger::udr_app().info("Retrieve the individual SMF registration of a UE");

  if (db_connector->query_smf_registration(ue_id, pdu_session_id,
                                           response_data)) {
    code = HTTP_STATUS_CODE_200_OK;
    Logger::udr_app().info("SmfRegistration: %s", response_data.dump().c_str());
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_query_smf_reg_list(const std::string& ue_id,
                                        nlohmann::json& response_data,
                                        long& code) {
  Logger::udr_app().info("Retrieve the SMF registration list of a UE");

  if (db_connector->query_smf_reg_list(ue_id, response_data)) {
    code = HTTP_STATUS_CODE_200_OK;
    Logger::udr_app().info("SmfRegList: %s", response_data.dump().c_str());
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}

//------------------------------------------------------------------------------
void udr_app::handle_query_smf_select_data(const std::string& ue_id,
                                           const std::string& serving_plmn_id,
                                           nlohmann::json& response_data,
                                           long& code) {
  Logger::udr_app().info(
      "Retrieve the SMF selection subscription data of a UE");

  if (db_connector->query_smf_select_data(ue_id, serving_plmn_id,
                                          response_data)) {
    code = HTTP_STATUS_CODE_200_OK;
    Logger::udr_app().info("SmfSelectionSubscriptionData: %s",
                           response_data.dump().c_str());
  } else {
    code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;  // TODO
  }
  return;
}
