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

/*! \file mysql_db.cpp
 \brief
 \author
 \company Eurecom
 \date 2022
 \email: contact@openairinterface.org
 */

#include "mysql_db.hpp"

#include <chrono>
#include <thread>

#include "AccessAndMobilitySubscriptionData.h"
#include "AuthenticationSubscription.h"
#include "ProblemDetails.h"
#include "SdmSubscription.h"
#include "logger.hpp"
#include "udr_config.hpp"

using namespace oai::udr::app;
using namespace oai::udr::model;
using namespace oai::udr::config;
extern udr_config udr_cfg;

//------------------------------------------------------------------------------
mysql_db::mysql_db(udr_event& ev)
    : database_wrapper<mysql_db>(), m_event_sub(ev), m_db_connection_status() {
  is_db_connection_active = false;
  start_event_connection_handling();
}

//------------------------------------------------------------------------------
mysql_db::~mysql_db() {
  if (db_connection_event.connected()) db_connection_event.disconnect();
  close_connection();
}

//------------------------------------------------------------------------------
bool mysql_db::initialize() {
  Logger::udr_mysql().debug("Initializing MySQL DB ...");
  if (!mysql_init(&mysql_connector)) {
    Logger::udr_mysql().error("Cannot initialize MySQL");
    throw std::runtime_error("Cannot initialize MySQL");
  }
  Logger::udr_mysql().debug("Done!");
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::connect(uint32_t num_retries) {
  Logger::udr_mysql().debug("Connecting to MySQL DB");

  int i = 0;
  while (i < num_retries) {
    // TODO: use mysql_real_connect_nonblocking (only from MySQL 8.0.16)
    if (!mysql_real_connect(
            &mysql_connector, udr_cfg.mysql.mysql_server.c_str(),
            udr_cfg.mysql.mysql_user.c_str(), udr_cfg.mysql.mysql_pass.c_str(),
            udr_cfg.mysql.mysql_db.c_str(), 0, 0, 0)) {
      Logger::udr_mysql().error(
          "An error occurred when connecting to MySQL DB (%s), retry ...",
          mysql_error(&mysql_connector));
      i++;
      set_db_connection_status(false);
      // throw std::runtime_error("Cannot connect to MySQL DB");
    } else {
      Logger::udr_mysql().info("Connected to MySQL DB");
      set_db_connection_status(true);
      return true;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  if (i == num_retries) {
    return false;
    // throw std::runtime_error("Cannot connect to MySQL DB");
  }
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::close_connection() {
  Logger::udr_mysql().debug("Close the connection with MySQL DB");
  mysql_close(&mysql_connector);
  set_db_connection_status(false);
  return true;
}

//------------------------------------------------------------------------------
void mysql_db::set_db_connection_status(bool status) {
  std::unique_lock lock(m_db_connection_status);
  is_db_connection_active = status;
}

//------------------------------------------------------------------------------
bool mysql_db::get_db_connection_status() const {
  std::shared_lock lock(m_db_connection_status);
  return is_db_connection_active;
}

//---------------------------------------------------------------------------------------------
void mysql_db::start_event_connection_handling() {
  // get current time
  uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();

  struct itimerspec its;
  its.it_value.tv_sec  = udr_cfg.mysql.connection_timeout;  // seconds
  its.it_value.tv_nsec = 0;  // 100 * 1000 * 1000; //100ms
  const uint64_t interval =
      its.it_value.tv_sec * 1000 +
      its.it_value.tv_nsec / 1000000;  // convert sec, nsec to msec

  db_connection_event = m_event_sub.subscribe_task_nf_heartbeat(
      boost::bind(&mysql_db::trigger_connection_handling_procedure, this, _1),
      interval, ms + interval);
}
//---------------------------------------------------------------------------------------------
void mysql_db::trigger_connection_handling_procedure(uint64_t ms) {
  _unused(ms);
  std::time_t current_time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  Logger::udr_mysql().debug(
      "DB Connection handling, current time: %s", std::ctime(&current_time));

  if (mysql_ping(&mysql_connector)) {
    set_db_connection_status(false);
    Logger::udr_app().warn(
        "The connection to the DB is not active, reset the connection and try "
        "again ...");
    // If couldn't connect to the DB
    // Reset the connection and try again
    close_connection();
    initialize();
    if (!connect(MAX_CONNECTION_RETRY))
      Logger::udr_app().warn("Could not establish the connection to the DB");
  } else {
    return;
  }
}

//------------------------------------------------------------------------------
bool mysql_db::insert_authentication_subscription(
    const std::string& id,
    const oai::udr::model::AuthenticationSubscription& auth_subscription,
    nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res          = nullptr;
  MYSQL_ROW row           = {};
  nlohmann::json json_tmp = {};

  std::string query =
      "SELECT * FROM AuthenticationSubscription WHERE ueid='" + id + "'";
  Logger::udr_mysql().info("MySQL Query: %s", query.c_str());

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size()) != 0) {
    Logger::udr_mysql().error(
        "Failed when executing mysql_real_query with SQL Query: %s",
        query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == nullptr) {
    Logger::udr_mysql().error(
        "mysql_store_result failure！ SQL Query: %s", query.c_str());
    return false;
  }

  row = mysql_fetch_row(res);
  if (row != nullptr) {
    Logger::udr_mysql().error("AuthenticationSubscription existed!");
    // Existed
    return false;
  }
  mysql_free_result(res);

  query =
      "INSERT INTO AuthenticationSubscription SET ueid='" + id + "'" +
      ",authenticationMethod='" + auth_subscription.getAuthenticationMethod() +
      "'" +
      (auth_subscription.encPermanentKeyIsSet() ?
           ",encPermanentKey='" + auth_subscription.getEncPermanentKey() + "'" :
           "") +
      (auth_subscription.protectionParameterIdIsSet() ?
           ",protectionParameterId='" +
               auth_subscription.getProtectionParameterId() + "'" :
           "") +
      (auth_subscription.authenticationManagementFieldIsSet() ?
           ",authenticationManagementField='" +
               auth_subscription.getAuthenticationManagementField() + "'" :
           "") +
      (auth_subscription.algorithmIdIsSet() ?
           ",algorithmId='" + auth_subscription.getAlgorithmId() + "'" :
           "") +
      (auth_subscription.encOpcKeyIsSet() ?
           ",encOpcKey='" + auth_subscription.getEncOpcKey() + "'" :
           "") +
      (auth_subscription.encTopcKeyIsSet() ?
           ",encTopcKey='" + auth_subscription.getEncTopcKey() + "'" :
           "") +
      //   (auth_subscription.vectorGenerationInHssIsSet() ?
      //   ",vectorGenerationInHss='" +
      //   auth_subscription.isVectorGenerationInHss() + "'" : "") +
      (auth_subscription.n5gcAuthMethodIsSet() ?
           ",n5gcAuthMethod='" + auth_subscription.getN5gcAuthMethod() + "'" :
           "") +
      // auth_subscription.rgAuthenticationIndIsSet() ? ",rgAuthenticationInd='"
      // + auth_subscription.() + "'" : "") +
      (auth_subscription.supiIsSet() ?
           ",supi='" + auth_subscription.getSupi() + "'" :
           "");

  if (auth_subscription.sequenceNumberIsSet()) {
    to_json(json_tmp, auth_subscription.getSequenceNumber());
    query += ",sequenceNumber='" + json_tmp.dump() + "'";
  }

  Logger::udr_mysql().info("MySQL Query: %s", query.c_str());

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query %s", query.c_str());
    return false;
  }

  to_json(json_data, auth_subscription);

  Logger::udr_mysql().debug(
      "AuthenticationSubscription POST: %s", json_data.dump().c_str());
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::delete_authentication_subscription(const std::string& id) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  const std::string query =
      "DELETE FROM AuthenticationSubscription WHERE ueid='" + id + "'";

  Logger::udr_mysql().debug("MySQL Query %s: ", query.c_str());

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query %s", query.c_str());
    return false;
  }

  Logger::udr_mysql().debug(
      "Deleted AuthenticationSubscription (with UE ID %s) successfully",
      id.c_str());
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::query_authentication_subscription(
    const std::string& id, nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  Logger::udr_mysql().info("Query Authentication Subscription");
  MYSQL_RES* res     = nullptr;
  MYSQL_ROW row      = {};
  MYSQL_FIELD* field = nullptr;
  nlohmann::json j   = {};

  AuthenticationSubscription authentication_subscription = {};
  const std::string query =
      "SELECT * FROM AuthenticationSubscription WHERE ueid='" + id + "'";
  Logger::udr_mysql().info("MySQL Query: %s", query.c_str());

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size()) != 0) {
    Logger::udr_mysql().error(
        "Failed when executing mysql_real_query with SQL Query: %s",
        query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);

  if (res == nullptr) {
    Logger::udr_mysql().error(
        "mysql_store_result failure！ SQL Query: %s", query.c_str());
    return false;
  }

  row = mysql_fetch_row(res);

  if (row != nullptr) {
    for (int i = 0; field = mysql_fetch_field(res); i++) {
      Logger::udr_mysql().debug("Row [%d]: %s ", i, field->name);
      if (!strcmp("authenticationMethod", field->name)) {
        authentication_subscription.setAuthenticationMethod(row[i]);
      } else if (!strcmp("encPermanentKey", field->name) && row[i] != nullptr) {
        authentication_subscription.setEncPermanentKey(row[i]);
      } else if (
          !strcmp("protectionParameterId", field->name) && row[i] != nullptr) {
        authentication_subscription.setProtectionParameterId(row[i]);
      } else if (!strcmp("sequenceNumber", field->name) && row[i] != nullptr) {
        SequenceNumber sequencenumber = {};
        nlohmann::json::parse(row[i]).get_to(sequencenumber);
        authentication_subscription.setSequenceNumber(sequencenumber);
      } else if (
          !strcmp("authenticationManagementField", field->name) &&
          row[i] != nullptr) {
        authentication_subscription.setAuthenticationManagementField(row[i]);
      } else if (!strcmp("algorithmId", field->name) && row[i] != nullptr) {
        authentication_subscription.setAlgorithmId(row[i]);
      } else if (!strcmp("encOpcKey", field->name) && row[i] != nullptr) {
        authentication_subscription.setEncOpcKey(row[i]);
      } else if (!strcmp("encTopcKey", field->name) && row[i] != nullptr) {
        authentication_subscription.setEncTopcKey(row[i]);
      } else if (
          !strcmp("vectorGenerationInHss", field->name) && row[i] != nullptr) {
        if (strcmp(row[i], "0"))
          authentication_subscription.setVectorGenerationInHss(true);
        else
          authentication_subscription.setVectorGenerationInHss(false);
      } else if (!strcmp("n5gcAuthMethod", field->name) && row[i] != nullptr) {
        authentication_subscription.setN5gcAuthMethod(row[i]);
      } else if (
          !strcmp("rgAuthenticationInd", field->name) && row[i] != nullptr) {
        if (strcmp(row[i], "0"))
          authentication_subscription.setRgAuthenticationInd(true);
        else
          authentication_subscription.setRgAuthenticationInd(false);
      } else if (!strcmp("supi", field->name) && row[i] != nullptr) {
        authentication_subscription.setSupi(row[i]);
      }
    }

    to_json(json_data, authentication_subscription);
  } else {
    Logger::udr_mysql().error(
        "AuthenticationSubscription no data！ SQL Query: %s", query.c_str());
  }

  mysql_free_result(res);
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::update_authentication_subscription(
    const std::string& ue_id,
    const std::vector<oai::udr::model::PatchItem>& patchItem,
    nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res = nullptr;
  MYSQL_ROW row  = {};
  const std::string select_Authenticationsubscription =
      "SELECT * from AuthenticationSubscription WHERE ueid='" + ue_id + "'";

  Logger::udr_mysql().info(
      "MySQL Query: %s", select_Authenticationsubscription.c_str());

  std::string query    = {};
  nlohmann::json tmp_j = {};

  for (int i = 0; i < patchItem.size(); i++) {
    if ((!strcmp(patchItem[i].getOp().c_str(), PATCH_OPERATION_REPLACE)) &&
        patchItem[i].valueIsSet()) {
      patchItem[i].getValue();
      SequenceNumber sequencenumber;
      nlohmann::json::parse(patchItem[i].getValue().c_str())
          .get_to(sequencenumber);

      if (mysql_real_query(
              &mysql_connector, select_Authenticationsubscription.c_str(),
              (unsigned long) select_Authenticationsubscription.size())) {
        Logger::udr_mysql().error(
            "mysql_real_query failure！SQL Query: %s",
            select_Authenticationsubscription.c_str());
        return false;
      }

      res = mysql_store_result(&mysql_connector);
      if (res == NULL) {
        Logger::udr_mysql().error(
            "mysql_store_result failure！SQL Query: %s",
            select_Authenticationsubscription.c_str());
        return false;
      }
      if (mysql_num_rows(res)) {
        nlohmann::json sequencenumber_j;
        query = "UPDATE AuthenticationSubscription SET sequenceNumber='";

        to_json(sequencenumber_j, sequencenumber);
        query += sequencenumber_j.dump() + "'";
        query += " WHERE ueid='" + ue_id + "'";
      } else {
        Logger::udr_mysql().error(
            "AuthenticationSubscription no data！ SQL Query %s",
            select_Authenticationsubscription.c_str());
      }

      Logger::udr_mysql().info("MySQL Update Cmd %s", query.c_str());
      mysql_free_result(res);

      if (mysql_real_query(
              &mysql_connector, query.c_str(), (unsigned long) query.size()) !=
          0) {
        Logger::udr_mysql().error(
            "update mysql failure！ SQL Cmd: %s", query.c_str());
        // TODO: Problem details
        return false;
      }
    }

    to_json(tmp_j, patchItem[i]);
    json_data += tmp_j;
  }

  Logger::udr_mysql().info(
      "AuthenticationSubscription PATCH: %s", json_data.dump().c_str());

  //	  code          = HTTP_STATUS_CODE_204_NO_CONTENT;
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::query_am_data(
    const std::string& ue_id, const std::string& serving_plmn_id,
    nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res     = nullptr;
  MYSQL_ROW row      = {};
  MYSQL_FIELD* field = nullptr;

  oai::udr::model::AccessAndMobilitySubscriptionData subscription_data = {};
  Logger::udr_mysql().debug("Handle Query AM Data");

  // TODO: Define query template in a header file
  const std::string query =
      "SELECT * from AccessAndMobilitySubscriptionData WHERE ueid='" + ue_id +
      "' AND servingPlmnid='" + serving_plmn_id + "'";

  Logger::udr_mysql().debug("SQL Query: %s", query.c_str());

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error("mysql_real_query failure！");
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == nullptr) {
    Logger::udr_mysql().error("mysql_store_result failure！");
    return false;
  }

  row = mysql_fetch_row(res);

  if (row != nullptr) {
    for (int i = 0; field = mysql_fetch_field(res); i++) {
      try {
        if (!strcmp("supportedFeatures", field->name) && row[i] != nullptr) {
          subscription_data.setSupportedFeatures(row[i]);
        } else if (!strcmp("gpsis", field->name) && row[i] != nullptr) {
          std::vector<std ::string> gpsis;
          nlohmann::json::parse(row[i]).get_to(gpsis);
          subscription_data.setGpsis(gpsis);
        } else if (
            !strcmp("internalGroupIds", field->name) && row[i] != nullptr) {
          std::vector<std ::string> internalgroupids;
          nlohmann::json::parse(row[i]).get_to(internalgroupids);
          subscription_data.setInternalGroupIds(internalgroupids);
        } else if (
            !strcmp("sharedVnGroupDataIds", field->name) && row[i] != nullptr) {
          std::map<std ::string, std::string> sharedvngroupdataids;
          nlohmann::json::parse(row[i]).get_to(sharedvngroupdataids);
          subscription_data.setSharedVnGroupDataIds(sharedvngroupdataids);
        } else if (
            !strcmp("subscribedUeAmbr", field->name) && row[i] != nullptr) {
          AmbrRm subscribedueambr;
          nlohmann::json::parse(row[i]).get_to(subscribedueambr);
          subscription_data.setSubscribedUeAmbr(subscribedueambr);
        } else if (!strcmp("nssai", field->name) && row[i] != nullptr) {
          Nssai nssai = {};
          nlohmann::json::parse(row[i]).get_to(nssai);
          subscription_data.setNssai(nssai);
        } else if (
            !strcmp("ratRestrictions", field->name) && row[i] != nullptr) {
          std ::vector<RatType> ratrestrictions;
          nlohmann::json::parse(row[i]).get_to(ratrestrictions);
          subscription_data.setRatRestrictions(ratrestrictions);
        } else if (
            !strcmp("forbiddenAreas", field->name) && row[i] != nullptr) {
          std ::vector<Area> forbiddenareas;
          nlohmann::json::parse(row[i]).get_to(forbiddenareas);
          subscription_data.setForbiddenAreas(forbiddenareas);
        } else if (
            !strcmp("serviceAreaRestriction", field->name) &&
            row[i] != nullptr) {
          ServiceAreaRestriction servicearearestriction;
          nlohmann::json::parse(row[i]).get_to(servicearearestriction);
          subscription_data.setServiceAreaRestriction(servicearearestriction);
        } else if (
            !strcmp("coreNetworkTypeRestrictions", field->name) &&
            row[i] != nullptr) {
          std ::vector<CoreNetworkType> corenetworktyperestrictions;
          nlohmann::json::parse(row[i]).get_to(corenetworktyperestrictions);
          subscription_data.setCoreNetworkTypeRestrictions(
              corenetworktyperestrictions);
        } else if (!strcmp("rfspIndex", field->name) && row[i] != nullptr) {
          int32_t a = std::stoi(row[i]);
          subscription_data.setRfspIndex(a);
        } else if (!strcmp("subsRegTimer", field->name) && row[i] != nullptr) {
          int32_t a = std::stoi(row[i]);
          subscription_data.setSubsRegTimer(a);
        } else if (!strcmp("ueUsageType", field->name) && row[i] != nullptr) {
          int32_t a = std::stoi(row[i]);
          subscription_data.setUeUsageType(a);
        } else if (!strcmp("mpsPriority", field->name) && row[i] != nullptr) {
          if (strcmp(row[i], "0"))
            subscription_data.setMpsPriority(true);
          else
            subscription_data.setMpsPriority(false);
        } else if (!strcmp("mcsPriority", field->name) && row[i] != nullptr) {
          if (strcmp(row[i], "0"))
            subscription_data.setMcsPriority(true);
          else
            subscription_data.setMcsPriority(false);
        } else if (!strcmp("activeTime", field->name) && row[i] != nullptr) {
          int32_t a = std::stoi(row[i]);
          subscription_data.setActiveTime(a);
        } else if (!strcmp("sorInfo", field->name) && row[i] != nullptr) {
          SorInfo sorinfo;
          nlohmann::json::parse(row[i]).get_to(sorinfo);
          subscription_data.setSorInfo(sorinfo);
        } else if (
            !strcmp("sorInfoExpectInd", field->name) && row[i] != nullptr) {
          if (strcmp(row[i], "0"))
            subscription_data.setSorInfoExpectInd(true);
          else
            subscription_data.setSorInfoExpectInd(false);
        } else if (
            !strcmp("sorafRetrieval", field->name) && row[i] != nullptr) {
          if (strcmp(row[i], "0"))
            subscription_data.setSorafRetrieval(true);
          else
            subscription_data.setSorafRetrieval(false);
        } else if (
            !strcmp("sorUpdateIndicatorList", field->name) &&
            row[i] != nullptr) {
          std ::vector<SorUpdateIndicator> sorupdateindicatorlist;
          nlohmann::json::parse(row[i]).get_to(sorupdateindicatorlist);
          subscription_data.setSorUpdateIndicatorList(sorupdateindicatorlist);
        } else if (!strcmp("upuInfo", field->name) && row[i] != nullptr) {
          UpuInfo upuinfo;
          nlohmann::json::parse(row[i]).get_to(upuinfo);
          subscription_data.setUpuInfo(upuinfo);
        } else if (!strcmp("micoAllowed", field->name) && row[i] != nullptr) {
          if (strcmp(row[i], "0"))
            subscription_data.setMicoAllowed(true);
          else
            subscription_data.setMicoAllowed(false);
        } else if (
            !strcmp("sharedAmDataIds", field->name) && row[i] != nullptr) {
          std ::vector<std ::string> sharedamdataids;
          nlohmann::json::parse(row[i]).get_to(sharedamdataids);
          subscription_data.setSharedAmDataIds(sharedamdataids);
        } else if (
            !strcmp("odbPacketServices", field->name) && row[i] != nullptr) {
          OdbPacketServices odbpacketservices;
          nlohmann::json::parse(row[i]).get_to(odbpacketservices);
          subscription_data.setOdbPacketServices(odbpacketservices);
        } else if (
            !strcmp("serviceGapTime", field->name) && row[i] != nullptr) {
          int32_t a = std::stoi(row[i]);
          subscription_data.setServiceGapTime(a);
        } else if (
            !strcmp("mdtUserConsent", field->name) && row[i] != nullptr) {
          MdtUserConsent mdtuserconsent;
          nlohmann::json::parse(row[i]).get_to(mdtuserconsent);
          subscription_data.setMdtUserConsent(mdtuserconsent);
        } else if (
            !strcmp("mdtConfiguration", field->name) && row[i] != nullptr) {
          MdtConfiguration mdtconfiguration;
          nlohmann::json::parse(row[i]).get_to(mdtconfiguration);
          subscription_data.setMdtConfiguration(mdtconfiguration);
        } else if (!strcmp("traceData", field->name) && row[i] != nullptr) {
          TraceData tracedata;
          nlohmann::json::parse(row[i]).get_to(tracedata);
          subscription_data.setTraceData(tracedata);
        } else if (!strcmp("cagData", field->name) && row[i] != nullptr) {
          CagData cagdata;
          nlohmann::json::parse(row[i]).get_to(cagdata);
          subscription_data.setCagData(cagdata);
        } else if (!strcmp("stnSr", field->name) && row[i] != nullptr) {
          subscription_data.setStnSr(row[i]);
        } else if (!strcmp("cMsisdn", field->name) && row[i] != nullptr) {
          subscription_data.setCMsisdn(row[i]);
        } else if (
            !strcmp("nbIoTUePriority", field->name) && row[i] != nullptr) {
          int32_t a = std::stoi(row[i]);
          subscription_data.setNbIoTUePriority(a);
        } else if (
            !strcmp("nssaiInclusionAllowed", field->name) &&
            row[i] != nullptr) {
          if (strcmp(row[i], "0"))
            subscription_data.setNssaiInclusionAllowed(true);
          else
            subscription_data.setNssaiInclusionAllowed(false);
        } else if (
            !strcmp("rgWirelineCharacteristics", field->name) &&
            row[i] != nullptr) {
          subscription_data.setRgWirelineCharacteristics(row[i]);
        } else if (
            !strcmp("ecRestrictionDataWb", field->name) && row[i] != nullptr) {
          EcRestrictionDataWb ecrestrictiondatawb;
          nlohmann::json::parse(row[i]).get_to(ecrestrictiondatawb);
          subscription_data.setEcRestrictionDataWb(ecrestrictiondatawb);
        } else if (
            !strcmp("ecRestrictionDataNb", field->name) && row[i] != nullptr) {
          if (strcmp(row[i], "0"))
            subscription_data.setEcRestrictionDataNb(true);
          else
            subscription_data.setEcRestrictionDataNb(false);
        } else if (
            !strcmp("expectedUeBehaviourList", field->name) &&
            row[i] != nullptr) {
          ExpectedUeBehaviourData expecteduebehaviourlist;
          nlohmann::json::parse(row[i]).get_to(expecteduebehaviourlist);
          subscription_data.setExpectedUeBehaviourList(expecteduebehaviourlist);
        } else if (
            !strcmp("primaryRatRestrictions", field->name) &&
            row[i] != nullptr) {
          std ::vector<RatType> primaryratrestrictions;
          nlohmann::json::parse(row[i]).get_to(primaryratrestrictions);
          subscription_data.setPrimaryRatRestrictions(primaryratrestrictions);
        } else if (
            !strcmp("secondaryRatRestrictions", field->name) &&
            row[i] != nullptr) {
          std ::vector<RatType> secondaryratrestrictions;
          nlohmann::json::parse(row[i]).get_to(secondaryratrestrictions);
          subscription_data.setSecondaryRatRestrictions(
              secondaryratrestrictions);
        } else if (
            !strcmp("edrxParametersList", field->name) && row[i] != nullptr) {
          std ::vector<EdrxParameters> edrxparameterslist;
          nlohmann::json::parse(row[i]).get_to(edrxparameterslist);
          subscription_data.setEdrxParametersList(edrxparameterslist);
        } else if (
            !strcmp("ptwParametersList", field->name) && row[i] != nullptr) {
          std ::vector<PtwParameters> ptwparameterslist;
          nlohmann::json::parse(row[i]).get_to(ptwparameterslist);
          subscription_data.setPtwParametersList(ptwparameterslist);
        } else if (
            !strcmp("iabOperationAllowed", field->name) && row[i] != nullptr) {
          if (strcmp(row[i], "0"))
            subscription_data.setIabOperationAllowed(true);
          else
            subscription_data.setIabOperationAllowed(false);
        } else if (
            !strcmp("wirelineForbiddenAreas", field->name) &&
            row[i] != nullptr) {
          std ::vector<WirelineArea> wirelineforbiddenareas;
          nlohmann::json::parse(row[i]).get_to(wirelineforbiddenareas);
          subscription_data.setWirelineForbiddenAreas(wirelineforbiddenareas);
        } else if (
            !strcmp("wirelineServiceAreaRestriction", field->name) &&
            row[i] != nullptr) {
          WirelineServiceAreaRestriction wirelineservicearearestriction;
          nlohmann::json::parse(row[i]).get_to(wirelineservicearearestriction);
          subscription_data.setWirelineServiceAreaRestriction(
              wirelineservicearearestriction);
        }
      } catch (std::exception e) {
        Logger::udr_mysql().error(
            " Cannot set values for Subscription Data: %s", e.what());
        return false;
      }
    }

    to_json(json_data, subscription_data);
    Logger::udr_mysql().debug(
        "AccessAndMobilitySubscriptionData GET (JSON): %s",
        json_data.dump().c_str());
  } else {
    Logger::udr_mysql().error(
        "No data available for AccessAndMobilitySubscriptionData!");
    return false;
  }

  mysql_free_result(res);
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::create_amf_context_3gpp(
    const std::string& ue_id,
    Amf3GppAccessRegistration& amf3GppAccessRegistration) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  nlohmann::json json_data = {};
  MYSQL_RES* res           = nullptr;
  MYSQL_ROW row            = {};

  const std::string select_AMF3GPPAccessRegistration =
      "SELECT * FROM Amf3GppAccessRegistration WHERE ueid='" + ue_id + "'";
  std::string query = {};

  nlohmann::json j = {};

  if (mysql_real_query(
          &mysql_connector, select_AMF3GPPAccessRegistration.c_str(),
          (unsigned long) select_AMF3GPPAccessRegistration.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query: %s",
        select_AMF3GPPAccessRegistration.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error(
        "mysql_store_result failure！ SQL Query: %s",
        select_AMF3GPPAccessRegistration.c_str());
    return false;
  }

  if (mysql_num_rows(res)) {
    query =
        "UPDATE Amf3GppAccessRegistration SET amfInstanceId='" +
        amf3GppAccessRegistration.getAmfInstanceId() + "'" +
        (amf3GppAccessRegistration.supportedFeaturesIsSet() ?
             ",supportedFeatures='" +
                 amf3GppAccessRegistration.getSupportedFeatures() + "'" :
             "") +
        (amf3GppAccessRegistration.purgeFlagIsSet() ?
             (amf3GppAccessRegistration.isPurgeFlag() ? ",purgeFlag=1" :
                                                        ",purgeFlag=0") :
             "") +
        (amf3GppAccessRegistration.peiIsSet() ?
             ",pei='" + amf3GppAccessRegistration.getPei() + "'" :
             "") +
        ",deregCallbackUri='" +
        amf3GppAccessRegistration.getDeregCallbackUri() + "'" +
        (amf3GppAccessRegistration.pcscfRestorationCallbackUriIsSet() ?
             ",pcscfRestorationCallbackUri='" +
                 amf3GppAccessRegistration.getPcscfRestorationCallbackUri() +
                 "'" :
             "") +
        (amf3GppAccessRegistration.initialRegistrationIndIsSet() ?
             (amf3GppAccessRegistration.isInitialRegistrationInd() ?
                  ",initialRegistrationInd=1" :
                  ",initialRegistrationInd=0") :
             "") +
        (amf3GppAccessRegistration.drFlagIsSet() ?
             (amf3GppAccessRegistration.isDrFlag() ? ",drFlag=1" :
                                                     ",drFlag=0") :
             "") +
        (amf3GppAccessRegistration.urrpIndicatorIsSet() ?
             (amf3GppAccessRegistration.isUrrpIndicator() ?
                  ",urrpIndicator=1" :
                  ",urrpIndicator=0") :
             "") +
        (amf3GppAccessRegistration.amfEeSubscriptionIdIsSet() ?
             ",amfEeSubscriptionId='" +
                 amf3GppAccessRegistration.getAmfEeSubscriptionId() + "'" :
             "") +
        (amf3GppAccessRegistration.ueSrvccCapabilityIsSet() ?
             (amf3GppAccessRegistration.isUeSrvccCapability() ?
                  ",ueSrvccCapability=1" :
                  ",ueSrvccCapability=0") :
             "") +
        (amf3GppAccessRegistration.registrationTimeIsSet() ?
             ",registrationTime='" +
                 amf3GppAccessRegistration.getRegistrationTime() + "'" :
             "") +
        (amf3GppAccessRegistration.noEeSubscriptionIndIsSet() ?
             (amf3GppAccessRegistration.isNoEeSubscriptionInd() ?
                  ",noEeSubscriptionInd=1" :
                  ",noEeSubscriptionInd=0") :
             "");

    if (amf3GppAccessRegistration.imsVoPsIsSet()) {
      to_json(j, amf3GppAccessRegistration.getImsVoPs());
      query += ",imsVoPs='" + j.dump() + "'";
    }
    if (amf3GppAccessRegistration.amfServiceNameDeregIsSet()) {
      to_json(j, amf3GppAccessRegistration.getAmfServiceNameDereg());
      query += ",amfServiceNameDereg='" + j.dump() + "'";
    }
    if (amf3GppAccessRegistration.amfServiceNamePcscfRestIsSet()) {
      to_json(j, amf3GppAccessRegistration.getAmfServiceNamePcscfRest());
      query += ",amfServiceNamePcscfRest='" + j.dump() + "'";
    }
    if (amf3GppAccessRegistration.backupAmfInfoIsSet()) {
      nlohmann::json tmp;
      j.clear();
      std::vector<BackupAmfInfo> backupamfinfo =
          amf3GppAccessRegistration.getBackupAmfInfo();
      for (int i = 0; i < backupamfinfo.size(); i++) {
        to_json(tmp, backupamfinfo[i]);
        j += tmp;
      }
      query += ",backupAmfInfo='" + j.dump() + "'";
    }
    if (amf3GppAccessRegistration.epsInterworkingInfoIsSet()) {
      to_json(j, amf3GppAccessRegistration.getEpsInterworkingInfo());
      query += ",epsInterworkingInfo='" + j.dump() + "'";
    }
    if (amf3GppAccessRegistration.vgmlcAddressIsSet()) {
      to_json(j, amf3GppAccessRegistration.getVgmlcAddress());
      query += ",vgmlcAddress='" + j.dump() + "'";
    }
    if (amf3GppAccessRegistration.contextInfoIsSet()) {
      to_json(j, amf3GppAccessRegistration.getContextInfo());
      query += ",contextInfo='" + j.dump() + "'";
    }

    to_json(j, amf3GppAccessRegistration.getGuami());
    query += ",guami='" + j.dump() + "'";
    to_json(j, amf3GppAccessRegistration.getRatType());
    query += ",ratType='" + j.dump() + "'";
    query += " WHERE ueid='" + ue_id + "'";
  } else {
    query =
        "INSERT INTO Amf3GppAccessRegistration SET ueid='" + ue_id + "'" +
        ",amfInstanceId='" + amf3GppAccessRegistration.getAmfInstanceId() +
        "'" +
        (amf3GppAccessRegistration.supportedFeaturesIsSet() ?
             ",supportedFeatures='" +
                 amf3GppAccessRegistration.getSupportedFeatures() + "'" :
             "") +
        (amf3GppAccessRegistration.purgeFlagIsSet() ?
             (amf3GppAccessRegistration.isPurgeFlag() ? ",purgeFlag=1" :
                                                        ",purgeFlag=0") :
             "") +
        (amf3GppAccessRegistration.peiIsSet() ?
             ",pei='" + amf3GppAccessRegistration.getPei() + "'" :
             "") +
        ",deregCallbackUri='" +
        amf3GppAccessRegistration.getDeregCallbackUri() + "'" +
        (amf3GppAccessRegistration.pcscfRestorationCallbackUriIsSet() ?
             ",pcscfRestorationCallbackUri='" +
                 amf3GppAccessRegistration.getPcscfRestorationCallbackUri() +
                 "'" :
             "") +
        (amf3GppAccessRegistration.initialRegistrationIndIsSet() ?
             (amf3GppAccessRegistration.isInitialRegistrationInd() ?
                  ",initialRegistrationInd=1" :
                  ",initialRegistrationInd=0") :
             "") +
        (amf3GppAccessRegistration.drFlagIsSet() ?
             (amf3GppAccessRegistration.isDrFlag() ? ",drFlag=1" :
                                                     ",drFlag=0") :
             "") +
        (amf3GppAccessRegistration.urrpIndicatorIsSet() ?
             (amf3GppAccessRegistration.isUrrpIndicator() ?
                  ",urrpIndicator=1" :
                  ",urrpIndicator=0") :
             "") +
        (amf3GppAccessRegistration.amfEeSubscriptionIdIsSet() ?
             ",amfEeSubscriptionId='" +
                 amf3GppAccessRegistration.getAmfEeSubscriptionId() + "'" :
             "") +
        (amf3GppAccessRegistration.ueSrvccCapabilityIsSet() ?
             (amf3GppAccessRegistration.isUeSrvccCapability() ?
                  ",ueSrvccCapability=1" :
                  ",ueSrvccCapability=0") :
             "") +
        (amf3GppAccessRegistration.registrationTimeIsSet() ?
             ",registrationTime='" +
                 amf3GppAccessRegistration.getRegistrationTime() + "'" :
             "") +
        (amf3GppAccessRegistration.noEeSubscriptionIndIsSet() ?
             (amf3GppAccessRegistration.isNoEeSubscriptionInd() ?
                  ",noEeSubscriptionInd=1" :
                  ",noEeSubscriptionInd=0") :
             "");

    if (amf3GppAccessRegistration.imsVoPsIsSet()) {
      to_json(j, amf3GppAccessRegistration.getImsVoPs());
      query += ",imsVoPs='" + j.dump() + "'";
    }
    if (amf3GppAccessRegistration.amfServiceNameDeregIsSet()) {
      to_json(j, amf3GppAccessRegistration.getAmfServiceNameDereg());
      query += ",amfServiceNameDereg='" + j.dump() + "'";
    }
    if (amf3GppAccessRegistration.amfServiceNamePcscfRestIsSet()) {
      to_json(j, amf3GppAccessRegistration.getAmfServiceNamePcscfRest());
      query += ",amfServiceNamePcscfRest='" + j.dump() + "'";
    }
    if (amf3GppAccessRegistration.backupAmfInfoIsSet()) {
      nlohmann::json tmp = {};
      j.clear();
      std::vector<BackupAmfInfo> backupamfinfo =
          amf3GppAccessRegistration.getBackupAmfInfo();
      for (int i = 0; i < backupamfinfo.size(); i++) {
        to_json(tmp, backupamfinfo[i]);
        j += tmp;
      }
      query += ",backupAmfInfo='" + j.dump() + "'";
    }
    if (amf3GppAccessRegistration.epsInterworkingInfoIsSet()) {
      to_json(j, amf3GppAccessRegistration.getEpsInterworkingInfo());
      query += ",epsInterworkingInfo='" + j.dump() + "'";
    }
    if (amf3GppAccessRegistration.vgmlcAddressIsSet()) {
      to_json(j, amf3GppAccessRegistration.getVgmlcAddress());
      query += ",vgmlcAddress='" + j.dump() + "'";
    }
    if (amf3GppAccessRegistration.contextInfoIsSet()) {
      to_json(j, amf3GppAccessRegistration.getContextInfo());
      query += ",contextInfo='" + j.dump() + "'";
    }

    to_json(j, amf3GppAccessRegistration.getGuami());
    query += ",guami='" + j.dump() + "'";
    to_json(j, amf3GppAccessRegistration.getRatType());
    query += ",ratType='" + j.dump() + "'";
  }

  mysql_free_result(res);
  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query %s", query.c_str());
    return false;
  }

  to_json(json_data, amf3GppAccessRegistration);

  Logger::udr_mysql().debug(
      "Amf3GppAccessRegistration PUT: %s", json_data.dump().c_str());

  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::query_amf_context_3gpp(
    const std::string& ue_id, nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res                                      = nullptr;
  MYSQL_ROW row                                       = {};
  MYSQL_FIELD* field                                  = nullptr;
  Amf3GppAccessRegistration amf3gppaccessregistration = {};
  const std::string query =
      "SELECT * FROM Amf3GppAccessRegistration WHERE ueid='" + ue_id + "'";

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query %s", query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query %s", query.c_str());
    return false;
  }

  row = mysql_fetch_row(res);

  if (row != NULL) {
    for (int i = 0; field = mysql_fetch_field(res); i++) {
      if (!strcmp("amfInstanceId", field->name)) {
        amf3gppaccessregistration.setAmfInstanceId(row[i]);
      } else if (!strcmp("supportedFeatures", field->name) && row[i] != NULL) {
        amf3gppaccessregistration.setSupportedFeatures(row[i]);
      } else if (!strcmp("purgeFlag", field->name) && row[i] != NULL) {
        if (strcmp(row[i], "0"))
          amf3gppaccessregistration.setPurgeFlag(true);
        else
          amf3gppaccessregistration.setPurgeFlag(false);
      } else if (!strcmp("pei", field->name) && row[i] != NULL) {
        amf3gppaccessregistration.setPei(row[i]);
      } else if (!strcmp("imsVoPs", field->name) && row[i] != NULL) {
        ImsVoPs imsvops;
        nlohmann::json::parse(row[i]).get_to(imsvops);
        amf3gppaccessregistration.setImsVoPs(imsvops);
      } else if (!strcmp("deregCallbackUri", field->name)) {
        amf3gppaccessregistration.setDeregCallbackUri(row[i]);
      } else if (
          !strcmp("amfServiceNameDereg", field->name) && row[i] != NULL) {
        ServiceName amfservicenamedereg;
        nlohmann::json::parse(row[i]).get_to(amfservicenamedereg);
        amf3gppaccessregistration.setAmfServiceNameDereg(amfservicenamedereg);
      } else if (
          !strcmp("pcscfRestorationCallbackUri", field->name) &&
          row[i] != NULL) {
        amf3gppaccessregistration.setPcscfRestorationCallbackUri(row[i]);
      } else if (
          !strcmp("amfServiceNamePcscfRest", field->name) && row[i] != NULL) {
        ServiceName amfservicenamepcscfrest;
        nlohmann::json::parse(row[i]).get_to(amfservicenamepcscfrest);
        amf3gppaccessregistration.setAmfServiceNamePcscfRest(
            amfservicenamepcscfrest);
      } else if (
          !strcmp("initialRegistrationInd", field->name) && row[i] != NULL) {
        if (strcmp(row[i], "0"))
          amf3gppaccessregistration.setInitialRegistrationInd(true);
        else
          amf3gppaccessregistration.setInitialRegistrationInd(false);
      } else if (!strcmp("guami", field->name)) {
        Guami guami;
        nlohmann::json::parse(row[i]).get_to(guami);
        amf3gppaccessregistration.setGuami(guami);
      } else if (!strcmp("backupAmfInfo", field->name) && row[i] != NULL) {
        std ::vector<BackupAmfInfo> backupamfinfo;
        nlohmann::json::parse(row[i]).get_to(backupamfinfo);
        amf3gppaccessregistration.setBackupAmfInfo(backupamfinfo);
      } else if (!strcmp("drFlag", field->name) && row[i] != NULL) {
        if (strcmp(row[i], "0"))
          amf3gppaccessregistration.setDrFlag(true);
        else
          amf3gppaccessregistration.setDrFlag(false);
      } else if (!strcmp("ratType", field->name)) {
        RatType rattype;
        nlohmann::json::parse(row[i]).get_to(rattype);
        amf3gppaccessregistration.setRatType(rattype);
      } else if (!strcmp("urrpIndicator", field->name) && row[i] != NULL) {
        if (strcmp(row[i], "0"))
          amf3gppaccessregistration.setUrrpIndicator(true);
        else
          amf3gppaccessregistration.setUrrpIndicator(false);
      } else if (
          !strcmp("amfEeSubscriptionId", field->name) && row[i] != NULL) {
        amf3gppaccessregistration.setAmfEeSubscriptionId(row[i]);
      } else if (
          !strcmp("epsInterworkingInfo", field->name) && row[i] != NULL) {
        EpsInterworkingInfo epsinterworkinginfo;
        nlohmann::json::parse(row[i]).get_to(epsinterworkinginfo);
        amf3gppaccessregistration.setEpsInterworkingInfo(epsinterworkinginfo);
      } else if (!strcmp("ueSrvccCapability", field->name) && row[i] != NULL) {
        if (strcmp(row[i], "0"))
          amf3gppaccessregistration.setUeSrvccCapability(true);
        else
          amf3gppaccessregistration.setUeSrvccCapability(false);
      } else if (!strcmp("registrationTime", field->name) && row[i] != NULL) {
        amf3gppaccessregistration.setRegistrationTime(row[i]);
      } else if (!strcmp("vgmlcAddress", field->name) && row[i] != NULL) {
        VgmlcAddress vgmlcaddress;
        nlohmann::json::parse(row[i]).get_to(vgmlcaddress);
        amf3gppaccessregistration.setVgmlcAddress(vgmlcaddress);
      } else if (!strcmp("contextInfo", field->name) && row[i] != NULL) {
        ContextInfo contextinfo;
        nlohmann::json::parse(row[i]).get_to(contextinfo);
        amf3gppaccessregistration.setContextInfo(contextinfo);
      } else if (
          !strcmp("noEeSubscriptionInd", field->name) && row[i] != NULL) {
        if (strcmp(row[i], "0"))
          amf3gppaccessregistration.setNoEeSubscriptionInd(true);
        else
          amf3gppaccessregistration.setNoEeSubscriptionInd(false);
      }
    }
    to_json(json_data, amf3gppaccessregistration);

    Logger::udr_mysql().debug(
        "Amf3GppAccessRegistration GET %s", json_data.dump().c_str());
  } else {
    Logger::udr_mysql().error(
        "Amf3GppAccessRegistration no data！ SQL Query %s", query.c_str());
  }

  mysql_free_result(res);
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::mysql_db::insert_authentication_status(
    const std::string& ue_id, const oai::udr::model::AuthEvent& authEvent,
    nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res = nullptr;
  MYSQL_ROW row  = {};

  const std::string select_AuthenticationStatus =
      "SELECT * FROM AuthenticationStatus WHERE ueid='" + ue_id + "'";
  std::string query = {};

  Logger::udr_mysql().info(
      "MySQL query: %s", select_AuthenticationStatus.c_str());

  if (mysql_real_query(
          &mysql_connector, select_AuthenticationStatus.c_str(),
          (unsigned long) select_AuthenticationStatus.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query %s",
        select_AuthenticationStatus.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error(
        "mysql_store_result failure！ SQL Query %s",
        select_AuthenticationStatus.c_str());
    return false;
  }

  if (mysql_num_rows(res)) {
    query = "UPDATE AuthenticationStatus SET nfInstanceId='" +
            authEvent.getNfInstanceId() + "'" +
            ",success=" + (authEvent.isSuccess() ? "1" : "0") + ",timeStamp='" +
            authEvent.getTimeStamp() + "'" + ",authType='" +
            authEvent.getAuthType() + "'" + ",servingNetworkName='" +
            authEvent.getServingNetworkName() + "'" +
            (authEvent.authRemovalIndIsSet() ?
                 (authEvent.isAuthRemovalInd() ? ",authRemovalInd=1" :
                                                 ",authRemovalInd=0") :
                 "");
    //        to_json(j,authEvent.getAuthType());
    //        query += ",authType='"+j.dump()+"'";
    query += " WHERE ueid='" + ue_id + "'";
  } else {
    query = "INSERT INTO AuthenticationStatus SET ueid='" + ue_id + "'" +
            ",nfInstanceId='" + authEvent.getNfInstanceId() + "'" +
            ",success=" + (authEvent.isSuccess() ? "1" : "0") + ",timeStamp='" +
            authEvent.getTimeStamp() + "'" + ",authType='" +
            authEvent.getAuthType() + "'" + ",servingNetworkName='" +
            authEvent.getServingNetworkName() + "'" +
            (authEvent.authRemovalIndIsSet() ?
                 (authEvent.isAuthRemovalInd() ? ",authRemovalInd=1" :
                                                 ",authRemovalInd=0") :
                 "");
    //        to_json(j,authEvent.getAuthType());
    //        query += ",authType='"+j.dump()+"'";
  }

  Logger::udr_mysql().info("MySQL query: %s", query.c_str());

  mysql_free_result(res);
  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql create failure！ SQL Query %s", query.c_str());
    return false;
  }

  nlohmann::json tmp = {};
  to_json(tmp, authEvent);
  Logger::udr_mysql().info("AuthenticationStatus PUT: %s", tmp.dump().c_str());
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::mysql_db::delete_authentication_status(
    const std::string& ue_id) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  const std::string query =
      "DELETE FROM AuthenticationStatus WHERE ueid='" + ue_id + "'";

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query %s", query.c_str());
    return false;
  }

  Logger::udr_mysql().debug("AuthenticationStatus DELETE - successful");
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::mysql_db::query_authentication_status(
    const std::string& ue_id, nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res                 = nullptr;
  MYSQL_ROW row                  = {};
  MYSQL_FIELD* field             = nullptr;
  AuthEvent authenticationstatus = {};
  const std::string query =
      "SELECT * FROM AuthenticationStatus WHERE ueid='" + ue_id + "'";

  Logger::udr_mysql().info("MySQL query: %s", query.c_str());
  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query %s", query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error("mysql_store_result failure！");
    return false;
  }

  row = mysql_fetch_row(res);
  if (row != NULL) {
    for (int i = 0; field = mysql_fetch_field(res); i++) {
      if (!strcmp("nfInstanceId", field->name)) {
        authenticationstatus.setNfInstanceId(row[i]);
      } else if (!strcmp("success", field->name)) {
        if (strcmp(row[i], "0"))
          authenticationstatus.setSuccess(true);
        else
          authenticationstatus.setSuccess(false);
      } else if (!strcmp("timeStamp", field->name)) {
        authenticationstatus.setTimeStamp(row[i]);
      } else if (!strcmp("authType", field->name)) {
        //                AuthType authtype;
        //                nlohmann::json::parse(row[i]).get_to(authtype);
        authenticationstatus.setAuthType(row[i]);
      } else if (!strcmp("servingNetworkName", field->name)) {
        authenticationstatus.setServingNetworkName(row[i]);
      } else if (!strcmp("authRemovalInd", field->name) && row[i] != NULL) {
        if (strcmp(row[i], "0"))
          authenticationstatus.setAuthRemovalInd(true);
        else
          authenticationstatus.setAuthRemovalInd(false);
      }
    }

    to_json(json_data, authenticationstatus);
    Logger::udr_mysql().info(
        "AuthenticationStatus GET: %s", json_data.dump().c_str());
  } else {
    Logger::udr_mysql().error(
        "AuthenticationStatus no data！ SQL Query %s", query.c_str());
  }

  mysql_free_result(res);
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::mysql_db::query_sdm_subscription(
    const std::string& ue_id, const std::string& subs_id,
    nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res                                    = nullptr;
  MYSQL_ROW row                                     = {};
  MYSQL_FIELD* field                                = nullptr;
  oai::udr::model::SdmSubscription SdmSubscriptions = {};
  const std::string query = "SELECT * FROM SdmSubscriptions WHERE ueid='" +
                            ue_id + "' AND subsId=" + subs_id;

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query: %s", query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error(
        "mysql_store_result failure！ SQL Query: %s", query.c_str());
    return false;
  }

  row = mysql_fetch_row(res);
  if (row != NULL) {
    for (int i = 0; field = mysql_fetch_field(res); i++) {
      if (!strcmp("nfInstanceId", field->name)) {
        SdmSubscriptions.setNfInstanceId(row[i]);
      } else if (
          !strcmp("implicitUnsubscribe", field->name) && row[i] != NULL) {
        if (strcmp(row[i], "0"))
          SdmSubscriptions.setImplicitUnsubscribe(true);
        else
          SdmSubscriptions.setImplicitUnsubscribe(false);
      } else if (!strcmp("expires", field->name) && row[i] != NULL) {
        SdmSubscriptions.setExpires(row[i]);
      } else if (!strcmp("callbackReference", field->name)) {
        SdmSubscriptions.setCallbackReference(row[i]);
      } else if (!strcmp("amfServiceName", field->name) && row[i] != NULL) {
        ServiceName amfservicename;
        nlohmann::json::parse(row[i]).get_to(amfservicename);
        SdmSubscriptions.setAmfServiceName(amfservicename);
      } else if (!strcmp("monitoredResourceUris", field->name)) {
        std::vector<std::string> monitoredresourceuris;
        nlohmann::json::parse(row[i]).get_to(monitoredresourceuris);
        SdmSubscriptions.setMonitoredResourceUris(monitoredresourceuris);
      } else if (!strcmp("singleNssai", field->name) && row[i] != NULL) {
        Snssai singlenssai;
        nlohmann::json::parse(row[i]).get_to(singlenssai);
        SdmSubscriptions.setSingleNssai(singlenssai);
      } else if (!strcmp("dnn", field->name) && row[i] != NULL) {
        SdmSubscriptions.setDnn(row[i]);
      } else if (!strcmp("subscriptionId", field->name) && row[i] != NULL) {
        SdmSubscriptions.setSubscriptionId(row[i]);
      } else if (!strcmp("plmnId", field->name) && row[i] != NULL) {
        PlmnId plmnid;
        nlohmann::json::parse(row[i]).get_to(plmnid);
        SdmSubscriptions.setPlmnId(plmnid);
      } else if (!strcmp("immediateReport", field->name) && row[i] != NULL) {
        if (strcmp(row[i], "0"))
          SdmSubscriptions.setImmediateReport(true);
        else
          SdmSubscriptions.setImmediateReport(false);
      } else if (!strcmp("report", field->name) && row[i] != NULL) {
        SubscriptionDataSets report;
        nlohmann::json::parse(row[i]).get_to(report);
        SdmSubscriptions.setReport(report);
      } else if (!strcmp("supportedFeatures", field->name) && row[i] != NULL) {
        SdmSubscriptions.setSupportedFeatures(row[i]);
      } else if (!strcmp("contextInfo", field->name) && row[i] != NULL) {
        ContextInfo contextinfo;
        nlohmann::json::parse(row[i]).get_to(contextinfo);
        SdmSubscriptions.setContextInfo(contextinfo);
      }
    }
    to_json(json_data, SdmSubscriptions);
    Logger::udr_mysql().debug(
        "SdmSubscription GET: %s", json_data.dump().c_str());
  } else {
    Logger::udr_mysql().error(
        "SdmSubscription no data！ SQL Query: %s", query.c_str());
  }

  mysql_free_result(res);
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::mysql_db::delete_sdm_subscription(
    const std::string& ue_id, const std::string& subs_id) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res                                 = nullptr;
  nlohmann::json j                               = {};
  oai::udr::model::ProblemDetails problemdetails = {};

  const std::string select_query =
      "SELECT * FROM SdmSubscriptions WHERE ueid='" + ue_id +
      "' AND subsId=" + subs_id;

  const std::string query = "DELETE FROM SdmSubscriptions WHERE ueid='" +
                            ue_id + "' AND subsId=" + subs_id;

  if (mysql_real_query(
          &mysql_connector, select_query.c_str(),
          (unsigned long) select_query.size())) {
    problemdetails.setCause("USER_NOT_FOUND");
    to_json(j, problemdetails);
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query: %s", query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    problemdetails.setCause("USER_NOT_FOUND");
    to_json(j, problemdetails);
    Logger::udr_mysql().error(
        "mysql_store_result failure！ SQL Query: %s", query.c_str());
    return false;
  }

  if (!mysql_num_rows(res)) {
    problemdetails.setCause("DATA_NOT_FOUND");
    to_json(j, problemdetails);
    return false;
  }
  mysql_free_result(res);

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    problemdetails.setCause("USER_NOT_FOUND");
    to_json(j, problemdetails);
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query: %s", query.c_str());
    return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::update_sdm_subscription(
    const std::string& ue_id, const std::string& subs_id,
    oai::udr::model::SdmSubscription& sdmSubscription,
    nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res = nullptr;
  MYSQL_ROW row  = {};

  const std::string select_query =
      "SELECT * FROM SdmSubscriptions WHERE ueid='" + ue_id +
      "' AND subsId=" + subs_id;
  std::string query             = {};
  nlohmann::json j              = {};
  ProblemDetails problemdetails = {};

  if (mysql_real_query(
          &mysql_connector, select_query.c_str(),
          (unsigned long) select_query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query: %s", query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error(
        "mysql_store_result failure！SQL Query: %s", query.c_str());
    return false;
  }

  if (mysql_num_rows(res)) {
    nlohmann::json MonitoredResourceUris_json(
        sdmSubscription.getMonitoredResourceUris());

    query =
        "UPDATE SdmSubscriptions SET nfInstanceId='" +
        sdmSubscription.getNfInstanceId() + "'" +
        (sdmSubscription.implicitUnsubscribeIsSet() ?
             (sdmSubscription.isImplicitUnsubscribe() ?
                  ",implicitUnsubscribe=1" :
                  ",implicitUnsubscribe=0") :
             "") +
        (sdmSubscription.expiresIsSet() ?
             ",expires='" + sdmSubscription.getExpires() + "'" :
             "") +
        ",callbackReference='" + sdmSubscription.getCallbackReference() + "'" +
        (sdmSubscription.dnnIsSet() ?
             ",dnn='" + sdmSubscription.getDnn() + "'" :
             "") +
        (sdmSubscription.subscriptionIdIsSet() ?
             ",subscriptionId='" + sdmSubscription.getSubscriptionId() + "'" :
             "") +
        (sdmSubscription.immediateReportIsSet() ?
             (sdmSubscription.isImmediateReport() ? ",immediateReport=1" :
                                                    ",immediateReport=0") :
             "") +
        (sdmSubscription.supportedFeaturesIsSet() ?
             ",supportedFeatures='" + sdmSubscription.getSupportedFeatures() +
                 "'" :
             "");

    if (sdmSubscription.amfServiceNameIsSet()) {
      to_json(j, sdmSubscription.getAmfServiceName());
      query += ",amfServiceName='" + j.dump() + "'";
    }
    if (sdmSubscription.singleNssaiIsSet()) {
      to_json(j, sdmSubscription.getSingleNssai());
      query += ",singleNssai='" + j.dump() + "'";
    }
    if (sdmSubscription.plmnIdIsSet()) {
      to_json(j, sdmSubscription.getPlmnId());
      query += ",plmnId='" + j.dump() + "'";
    }
    if (sdmSubscription.reportIsSet()) {
      to_json(j, sdmSubscription.getReport());
      query += ",report='" + j.dump() + "'";
    }
    if (sdmSubscription.contextInfoIsSet()) {
      to_json(j, sdmSubscription.getContextInfo());
      query += ",contextInfo='" + j.dump() + "'";
    }

    query +=
        ",monitoredResourceUris='" + MonitoredResourceUris_json.dump() + "'";

    query += " WHERE ueid='" + ue_id + "' AND subsId=" + subs_id;
  } else {
    to_json(json_data, problemdetails);
    mysql_free_result(res);
    return false;
  }

  mysql_free_result(res);
  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query: %s", query.c_str());
    return false;
  }

  nlohmann::json json_tmp = {};
  to_json(json_tmp, sdmSubscription);

  Logger::udr_mysql().debug("SdmSubscription PUT: %s", json_tmp.dump().c_str());
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::create_sdm_subscriptions(
    const std::string& ue_id, oai::udr::model::SdmSubscription& sdmSubscription,
    nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res   = nullptr;
  MYSQL_ROW row    = {};
  nlohmann::json j = {};
  int32_t subs_id  = 0;
  int32_t count    = 0;
  std::string query =
      "SELECT subsId FROM SdmSubscriptions WHERE ueid='" + ue_id + "'";

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query %s", query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error(
        "mysql_store_result failure！ SQL Query %s", query.c_str());
    return false;
  }

  while (row = mysql_fetch_row(res)) {
    count++;
    if (strcmp(row[0], std::to_string(count).c_str())) {
      subs_id = count;
      break;
    }
  }
  mysql_free_result(res);

  query =
      "INSERT INTO SdmSubscriptions SET ueid='" + ue_id + "'" +
      ",nfInstanceId='" + sdmSubscription.getNfInstanceId() + "'" +
      (sdmSubscription.implicitUnsubscribeIsSet() ?
           (sdmSubscription.isImplicitUnsubscribe() ?
                ",implicitUnsubscribe=1" :
                ",implicitUnsubscribe=0") :
           "") +
      (sdmSubscription.expiresIsSet() ?
           ",expires='" + sdmSubscription.getExpires() + "'" :
           "") +
      ",callbackReference='" + sdmSubscription.getCallbackReference() + "'" +
      (sdmSubscription.dnnIsSet() ? ",dnn='" + sdmSubscription.getDnn() + "'" :
                                    "") +
      (sdmSubscription.subscriptionIdIsSet() ?
           ",subscriptionId='" + sdmSubscription.getSubscriptionId() + "'" :
           "") +
      (sdmSubscription.immediateReportIsSet() ?
           (sdmSubscription.isImmediateReport() ? ",immediateReport=1" :
                                                  ",immediateReport=0") :
           "") +
      (sdmSubscription.supportedFeaturesIsSet() ?
           ",supportedFeatures='" + sdmSubscription.getSupportedFeatures() +
               "'" :
           "");

  if (sdmSubscription.amfServiceNameIsSet()) {
    to_json(j, sdmSubscription.getAmfServiceName());
    query += ",amfServiceName='" + j.dump() + "'";
  }
  if (sdmSubscription.singleNssaiIsSet()) {
    to_json(j, sdmSubscription.getSingleNssai());
    query += ",singleNssai='" + j.dump() + "'";
  }
  if (sdmSubscription.plmnIdIsSet()) {
    to_json(j, sdmSubscription.getPlmnId());
    query += ",plmnId='" + j.dump() + "'";
  }
  if (sdmSubscription.reportIsSet()) {
    to_json(j, sdmSubscription.getReport());
    query += ",report='" + j.dump() + "'";
  }
  if (sdmSubscription.contextInfoIsSet()) {
    to_json(j, sdmSubscription.getContextInfo());
    query += ",contextInfo='" + j.dump() + "'";
  }

  nlohmann::json MonitoredResourceUris_json(
      sdmSubscription.getMonitoredResourceUris());
  query += ",monitoredResourceUris='" + MonitoredResourceUris_json.dump() + "'";

  if (subs_id && (subs_id == count)) {
    query += ",subsId=" + std::to_string(subs_id);
  }

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query %s", query.c_str());
    return false;
  }

  to_json(j, sdmSubscription);
  json_data = j;

  Logger::udr_mysql().debug(
      "SdmSubscriptions POST: %s", json_data.dump().c_str());
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::query_sdm_subscriptions(
    const std::string& ue_id, nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res     = nullptr;
  MYSQL_ROW row      = {};
  MYSQL_FIELD* field = nullptr;
  std::vector<std::string> fields;
  nlohmann::json j   = {};
  nlohmann::json tmp = {};

  const std::string query =
      "SELECT * FROM SdmSubscriptions WHERE ueid='" + ue_id + "'";

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query: %s", query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error(
        "mysql_store_result failure！ SQL Query: %s", query.c_str());
    return false;
  }

  while (field = mysql_fetch_field(res)) {
    fields.push_back(field->name);
  }

  j.clear();

  while (row = mysql_fetch_row(res)) {
    SdmSubscription sdmsubscriptions = {};
    tmp.clear();

    for (int i = 0; i < fields.size(); i++) {
      if (!strcmp("nfInstanceId", fields[i].c_str())) {
        sdmsubscriptions.setNfInstanceId(row[i]);
      } else if (
          !strcmp("implicitUnsubscribe", fields[i].c_str()) && row[i] != NULL) {
        if (strcmp(row[i], "0"))
          sdmsubscriptions.setImplicitUnsubscribe(true);
        else
          sdmsubscriptions.setImplicitUnsubscribe(false);
      } else if (!strcmp("expires", fields[i].c_str()) && row[i] != NULL) {
        sdmsubscriptions.setExpires(row[i]);
      } else if (!strcmp("callbackReference", fields[i].c_str())) {
        sdmsubscriptions.setCallbackReference(row[i]);
      } else if (
          !strcmp("amfServiceName", fields[i].c_str()) && row[i] != NULL) {
        ServiceName amfservicename;
        nlohmann::json::parse(row[i]).get_to(amfservicename);
        sdmsubscriptions.setAmfServiceName(amfservicename);
      } else if (!strcmp("monitoredResourceUris", fields[i].c_str())) {
        std::vector<std::string> monitoredresourceuris;
        nlohmann::json::parse(row[i]).get_to(monitoredresourceuris);
        sdmsubscriptions.setMonitoredResourceUris(monitoredresourceuris);
      } else if (!strcmp("singleNssai", fields[i].c_str()) && row[i] != NULL) {
        Snssai singlenssai;
        nlohmann::json::parse(row[i]).get_to(singlenssai);
        sdmsubscriptions.setSingleNssai(singlenssai);
      } else if (!strcmp("dnn", fields[i].c_str()) && row[i] != NULL) {
        sdmsubscriptions.setDnn(row[i]);
      } else if (
          !strcmp("subscriptionId", fields[i].c_str()) && row[i] != NULL) {
        sdmsubscriptions.setSubscriptionId(row[i]);
      } else if (!strcmp("plmnId", fields[i].c_str()) && row[i] != NULL) {
        PlmnId plmnid;
        nlohmann::json::parse(row[i]).get_to(plmnid);
        sdmsubscriptions.setPlmnId(plmnid);
      } else if (
          !strcmp("immediateReport", fields[i].c_str()) && row[i] != NULL) {
        if (strcmp(row[i], "0"))
          sdmsubscriptions.setImmediateReport(true);
        else
          sdmsubscriptions.setImmediateReport(false);
      } else if (!strcmp("report", fields[i].c_str()) && row[i] != NULL) {
        SubscriptionDataSets report;
        nlohmann::json::parse(row[i]).get_to(report);
        sdmsubscriptions.setReport(report);
      } else if (
          !strcmp("supportedFeatures", fields[i].c_str()) && row[i] != NULL) {
        sdmsubscriptions.setSupportedFeatures(row[i]);
      } else if (!strcmp("contextInfo", fields[i].c_str()) && row[i] != NULL) {
        ContextInfo contextinfo;
        nlohmann::json::parse(row[i]).get_to(contextinfo);
        sdmsubscriptions.setContextInfo(contextinfo);
      }
    }
    to_json(tmp, sdmsubscriptions);
    j += tmp;
  }

  mysql_free_result(res);
  json_data = j;

  Logger::udr_mysql().debug(
      "SdmSubscriptions GET: %s", json_data.dump().c_str());
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::query_sm_data(
    const std::string& ue_id, const std::string& serving_plmn_id,
    nlohmann::json& json_data, const oai::udr::model::Snssai& snssai,
    const std::string dnn) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res                                                      = nullptr;
  MYSQL_ROW row                                                       = {};
  MYSQL_FIELD* field                                                  = nullptr;
  nlohmann::json j                                                    = {};
  SessionManagementSubscriptionData sessionmanagementsubscriptiondata = {};
  std::string query =
      "SELECT * FROM SessionManagementSubscriptionData WHERE ueid='" + ue_id +
      "' AND servingPlmnid='" + serving_plmn_id + "' ";
  std::string option_str = {};

  if (snssai.getSst() > 0) {
    option_str += " AND JSON_EXTRACT(singleNssai, \"$.sst\")=" +
                  std::to_string(snssai.getSst());
  }
  if (!dnn.empty()) {
    option_str +=
        " AND JSON_EXTRACT(dnnConfigurations, \"$." + dnn + "\") IS NOT NULL";
  }

  query += option_str;
  Logger::udr_mysql().debug("MySQL query: %s", query.c_str());

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure, SQL Query: %s", query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error(
        "mysql_store_result failure, SQL Query: %s", query.c_str());
    return false;
  }

  row = mysql_fetch_row(res);

  if (row != NULL) {
    for (int i = 0; field = mysql_fetch_field(res); i++) {
      if (!strcmp("singleNssai", field->name)) {
        Snssai singlenssai;
        nlohmann::json::parse(row[i]).get_to(singlenssai);
        sessionmanagementsubscriptiondata.setSingleNssai(singlenssai);
      } else if (!strcmp("dnnConfigurations", field->name) && row[i] != NULL) {
        std ::map<std ::string, DnnConfiguration> dnnconfigurations;
        nlohmann::json::parse(row[i]).get_to(dnnconfigurations);
        sessionmanagementsubscriptiondata.setDnnConfigurations(
            dnnconfigurations);
        Logger::udr_mysql().debug("DNN configurations (row %d): %s", i, row[i]);
        for (auto d : dnnconfigurations) {
          nlohmann::json temp = {};
          to_json(temp, d.second);
          Logger::udr_mysql().debug(
              "DNN configurations: %s", temp.dump().c_str());
        }
      } else if (!strcmp("internalGroupIds", field->name) && row[i] != NULL) {
        std ::vector<std ::string> internalgroupIds;
        nlohmann::json::parse(row[i]).get_to(internalgroupIds);
        sessionmanagementsubscriptiondata.setInternalGroupIds(internalgroupIds);
      } else if (
          !strcmp("sharedVnGroupDataIds", field->name) && row[i] != NULL) {
        std ::map<std ::string, std ::string> sharedvngroupdataids;
        nlohmann::json::parse(row[i]).get_to(sharedvngroupdataids);
        sessionmanagementsubscriptiondata.setSharedVnGroupDataIds(
            sharedvngroupdataids);
      } else if (
          !strcmp("sharedDnnConfigurationsId", field->name) && row[i] != NULL) {
        sessionmanagementsubscriptiondata.setSharedDnnConfigurationsId(row[i]);
      } else if (!strcmp("odbPacketServices", field->name) && row[i] != NULL) {
        OdbPacketServices odbpacketservices;
        nlohmann::json::parse(row[i]).get_to(odbpacketservices);
        sessionmanagementsubscriptiondata.setOdbPacketServices(
            odbpacketservices);
      } else if (!strcmp("traceData", field->name) && row[i] != NULL) {
        TraceData tracedata;
        nlohmann::json::parse(row[i]).get_to(tracedata);
        sessionmanagementsubscriptiondata.setTraceData(tracedata);
      } else if (!strcmp("sharedTraceDataId", field->name) && row[i] != NULL) {
        sessionmanagementsubscriptiondata.setSharedTraceDataId(row[i]);
      } else if (
          !strcmp("expectedUeBehavioursList", field->name) && row[i] != NULL) {
        std ::map<std ::string, ExpectedUeBehaviourData>
            expecteduebehaviourslist;
        nlohmann::json::parse(row[i]).get_to(expecteduebehaviourslist);
        sessionmanagementsubscriptiondata.setExpectedUeBehavioursList(
            expecteduebehaviourslist);
      } else if (
          !strcmp("suggestedPacketNumDlList", field->name) && row[i] != NULL) {
        std ::map<std ::string, SuggestedPacketNumDl> suggestedpacketnumdllist;
        nlohmann::json::parse(row[i]).get_to(suggestedpacketnumdllist);
        sessionmanagementsubscriptiondata.setSuggestedPacketNumDlList(
            suggestedpacketnumdllist);
      } else if (
          !strcmp("3gppChargingCharacteristics", field->name) &&
          row[i] != NULL) {
        sessionmanagementsubscriptiondata.setR3gppChargingCharacteristics(
            row[i]);
      }
    }
    to_json(j, sessionmanagementsubscriptiondata);
    json_data = j;

    Logger::udr_mysql().debug(
        "SessionManagementSubscriptionData: %s", j.dump().c_str());
  } else {
    Logger::udr_mysql().error(
        "SessionManagementSubscriptionData no data found, SQL query: %s",
        query.c_str());
  }
  mysql_free_result(res);

  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::insert_smf_context_non_3gpp(
    const std::string& ue_id, const int32_t& pdu_session_id,
    const oai::udr::model::SmfRegistration& smfRegistration,
    nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res    = nullptr;
  MYSQL_ROW row     = {};
  std::string query = {};
  nlohmann::json j  = {};

  const std::string select_SmfRegistration =
      "SELECT * FROM SmfRegistrations WHERE ueid='" + ue_id +
      "' AND subpduSessionId=" + std::to_string(pdu_session_id);

  if (mysql_real_query(
          &mysql_connector, select_SmfRegistration.c_str(),
          (unsigned long) select_SmfRegistration.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query: %s",
        select_SmfRegistration.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error(
        "mysql_store_result failure！ SQL Query: %s",
        select_SmfRegistration.c_str());
    return false;
  }

  if (mysql_num_rows(res)) {
    query =
        "UPDATE SmfRegistrations SET smfInstanceId='" +
        smfRegistration.getSmfInstanceId() + "'" +
        ",pduSessionId=" + std::to_string(smfRegistration.getPduSessionId()) +
        (smfRegistration.smfSetIdIsSet() ?
             ",smfSetId='" + smfRegistration.getSmfSetId() + "'" :
             "") +
        (smfRegistration.supportedFeaturesIsSet() ?
             ",supportedFeatures='" + smfRegistration.getSupportedFeatures() +
                 "'" :
             "") +
        (smfRegistration.dnnIsSet() ?
             ",dnn='" + smfRegistration.getDnn() + "'" :
             "") +
        (smfRegistration.emergencyServicesIsSet() ?
             (smfRegistration.isEmergencyServices() ? ",emergencyServices=1" :
                                                      ",emergencyServices=0") :
             "") +
        (smfRegistration.pcscfRestorationCallbackUriIsSet() ?
             ",pcscfRestorationCallbackUri='" +
                 smfRegistration.getPcscfRestorationCallbackUri() + "'" :
             "") +
        (smfRegistration.pgwFqdnIsSet() ?
             ",pgwFqdn='" + smfRegistration.getPgwFqdn() + "'" :
             "") +
        (smfRegistration.epdgIndIsSet() ?
             (smfRegistration.isEpdgInd() ? ",epdgInd=1" : ",epdgInd=0") :
             "") +
        (smfRegistration.deregCallbackUriIsSet() ?
             ",deregCallbackUri='" + smfRegistration.getDeregCallbackUri() +
                 "'" :
             "") +
        (smfRegistration.registrationTimeIsSet() ?
             ",registrationTime='" + smfRegistration.getRegistrationTime() +
                 "'" :
             "");

    if (smfRegistration.registrationReasonIsSet()) {
      to_json(j, smfRegistration.getRegistrationReason());
      query += ",registrationReason='" + j.dump() + "'";
    }
    if (smfRegistration.contextInfoIsSet()) {
      to_json(j, smfRegistration.getContextInfo());
      query += ",contextInfo='" + j.dump() + "'";
    }

    to_json(j, smfRegistration.getSingleNssai());
    query += ",singleNssai='" + j.dump() + "'";
    to_json(j, smfRegistration.getPlmnId());
    query += ",plmnId='" + j.dump() + "'";
    query += " WHERE ueid='" + ue_id +
             "' AND subpduSessionId=" + std::to_string(pdu_session_id);
  } else {
    query =
        "INSERT INTO SmfRegistrations SET ueid='" + ue_id + "'" +
        ",subpduSessionId=" + std::to_string(pdu_session_id) +
        ",pduSessionId=" + std::to_string(smfRegistration.getPduSessionId()) +
        ",smfInstanceId='" + smfRegistration.getSmfInstanceId() + "'" +
        (smfRegistration.smfSetIdIsSet() ?
             ",smfSetId='" + smfRegistration.getSmfSetId() + "'" :
             "") +
        (smfRegistration.supportedFeaturesIsSet() ?
             ",supportedFeatures='" + smfRegistration.getSupportedFeatures() +
                 "'" :
             "") +
        (smfRegistration.dnnIsSet() ?
             ",dnn='" + smfRegistration.getDnn() + "'" :
             "") +
        (smfRegistration.emergencyServicesIsSet() ?
             (smfRegistration.isEmergencyServices() ? ",emergencyServices=1" :
                                                      ",emergencyServices=0") :
             "") +
        (smfRegistration.pcscfRestorationCallbackUriIsSet() ?
             ",pcscfRestorationCallbackUri='" +
                 smfRegistration.getPcscfRestorationCallbackUri() + "'" :
             "") +
        (smfRegistration.pgwFqdnIsSet() ?
             ",pgwFqdn='" + smfRegistration.getPgwFqdn() + "'" :
             "") +
        (smfRegistration.epdgIndIsSet() ?
             (smfRegistration.isEpdgInd() ? ",epdgInd=1" : ",epdgInd=0") :
             "") +
        (smfRegistration.deregCallbackUriIsSet() ?
             ",deregCallbackUri='" + smfRegistration.getDeregCallbackUri() +
                 "'" :
             "") +
        (smfRegistration.registrationTimeIsSet() ?
             ",registrationTime='" + smfRegistration.getRegistrationTime() +
                 "'" :
             "");

    if (smfRegistration.registrationReasonIsSet()) {
      to_json(j, smfRegistration.getRegistrationReason());
      query += ",registrationReason='" + j.dump() + "'";
    }
    if (smfRegistration.contextInfoIsSet()) {
      to_json(j, smfRegistration.getContextInfo());
      query += ",contextInfo='" + j.dump() + "'";
    }

    to_json(j, smfRegistration.getSingleNssai());
    query += ",singleNssai='" + j.dump() + "'";
    to_json(j, smfRegistration.getPlmnId());
    query += ",plmnId='" + j.dump() + "'";
  }

  mysql_free_result(res);
  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query: %s", query.c_str());
    return false;
  }

  to_json(j, smfRegistration);
  json_data = j;

  Logger::udr_mysql().debug("SmfRegistration PUT: %s", j.dump().c_str());
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::delete_smf_context(
    const std::string& ue_id, const int32_t& pdu_session_id) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  const std::string query =
      "DELETE FROM SmfRegistrations WHERE ueid='" + ue_id +
      "' AND subpduSessionId=" + std::to_string(pdu_session_id);

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query: %s", query.c_str());
    return false;
  }

  // r_data = {};
  // code          = HTTP_STATUS_CODE_204_NO_CONTENT;
  Logger::udr_mysql().debug("SmfRegistration DELETE - successful");
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::query_smf_registration(
    const std::string& ue_id, const int32_t& pdu_session_id,
    nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res                  = nullptr;
  MYSQL_ROW row                   = {};
  MYSQL_FIELD* field              = nullptr;
  nlohmann::json j                = {};
  SmfRegistration smfregistration = {};
  const std::string query =
      "SELECT * FROM SmfRegistrations WHERE ueid='" + ue_id +
      "' AND subpduSessionId=" + std::to_string(pdu_session_id);

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query: %s", query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error(
        "mysql_store_result failure！SQL Query: %s", query.c_str());
    return false;
  }

  row = mysql_fetch_row(res);
  if (row != NULL) {
    for (int i = 0; field = mysql_fetch_field(res); i++) {
      try {
        if (!strcmp("smfInstanceId", field->name)) {
          smfregistration.setSmfInstanceId(row[i]);
        } else if (!strcmp("smfSetId", field->name) && row[i] != NULL) {
          smfregistration.setSmfSetId(row[i]);
        } else if (
            !strcmp("supportedFeatures", field->name) && row[i] != NULL) {
          smfregistration.setSupportedFeatures(row[i]);
        } else if (!strcmp("pduSessionId", field->name)) {
          int32_t a = std::stoi(row[i]);
          smfregistration.setPduSessionId(a);
        } else if (!strcmp("singleNssai", field->name)) {
          Snssai singlenssai;
          nlohmann::json::parse(row[i]).get_to(singlenssai);
          smfregistration.setSingleNssai(singlenssai);
        } else if (!strcmp("dnn", field->name) && row[i] != NULL) {
          smfregistration.setDnn(row[i]);
        } else if (
            !strcmp("emergencyServices", field->name) && row[i] != NULL) {
          if (strcmp(row[i], "0"))
            smfregistration.setEmergencyServices(true);
          else
            smfregistration.setEmergencyServices(false);
        } else if (
            !strcmp("pcscfRestorationCallbackUri", field->name) &&
            row[i] != NULL) {
          smfregistration.setPcscfRestorationCallbackUri(row[i]);
        } else if (!strcmp("plmnId", field->name)) {
          PlmnId plmnid;
          nlohmann::json::parse(row[i]).get_to(plmnid);
          smfregistration.setPlmnId(plmnid);
        } else if (!strcmp("pgwFqdn", field->name) && row[i] != NULL) {
          smfregistration.setPgwFqdn(row[i]);
        } else if (!strcmp("epdgInd", field->name) && row[i] != NULL) {
          if (strcmp(row[i], "0"))
            smfregistration.setEpdgInd(true);
          else
            smfregistration.setEpdgInd(false);
        } else if (!strcmp("deregCallbackUri", field->name) && row[i] != NULL) {
          smfregistration.setDeregCallbackUri(row[i]);
        } else if (
            !strcmp("registrationReason", field->name) && row[i] != NULL) {
          RegistrationReason registrationreason;
          nlohmann::json::parse(row[i]).get_to(registrationreason);
          smfregistration.setRegistrationReason(registrationreason);
        } else if (!strcmp("registrationTime", field->name) && row[i] != NULL) {
          smfregistration.setRegistrationTime(row[i]);
        } else if (!strcmp("contextInfo", field->name) && row[i] != NULL) {
          ContextInfo contextinfo;
          nlohmann::json::parse(row[i]).get_to(contextinfo);
          smfregistration.setContextInfo(contextinfo);
        }
      } catch (std::exception e) {
        Logger::udr_mysql().error(
            " Cannot set values for SMF Registration: %s", e.what());
      }
    }
    to_json(j, smfregistration);
    json_data = j;

    Logger::udr_mysql().debug("SmfRegistration GET: %s", j.dump().c_str());
  } else {
    Logger::udr_mysql().error(
        "SmfRegistration no data！ SQL Query: %s", query.c_str());
  }
  mysql_free_result(res);

  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::query_smf_reg_list(
    const std::string& ue_id, nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res     = nullptr;
  MYSQL_ROW row      = {};
  MYSQL_FIELD* field = nullptr;
  std::vector<std::string> fields;
  nlohmann::json j   = {};
  nlohmann::json tmp = {};

  const std::string query =
      "SELECT * FROM SmfRegistrations WHERE ueid='" + ue_id + "'";

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！SQL Query: %s", query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error(
        "mysql_store_result failure！SQL Query: %s", query.c_str());
    return false;
  }

  while (field = mysql_fetch_field(res)) {
    fields.push_back(field->name);
  }

  j.clear();
  while (row = mysql_fetch_row(res)) {
    SmfRegistration smfregistration = {};

    tmp.clear();

    for (int i = 0; i < fields.size(); i++) {
      try {
        if (!strcmp("smfInstanceId", fields[i].c_str())) {
          smfregistration.setSmfInstanceId(row[i]);
        } else if (!strcmp("smfSetId", fields[i].c_str()) && row[i] != NULL) {
          smfregistration.setSmfSetId(row[i]);
        } else if (
            !strcmp("supportedFeatures", fields[i].c_str()) && row[i] != NULL) {
          smfregistration.setSupportedFeatures(row[i]);
        } else if (!strcmp("pduSessionId", fields[i].c_str())) {
          int32_t a = std::stoi(row[i]);
          smfregistration.setPduSessionId(a);
        } else if (!strcmp("singleNssai", fields[i].c_str())) {
          Snssai singlenssai;
          nlohmann::json::parse(row[i]).get_to(singlenssai);
          smfregistration.setSingleNssai(singlenssai);
        } else if (!strcmp("dnn", fields[i].c_str()) && row[i] != NULL) {
          smfregistration.setDnn(row[i]);
        } else if (
            !strcmp("emergencyServices", fields[i].c_str()) && row[i] != NULL) {
          if (strcmp(row[i], "0"))
            smfregistration.setEmergencyServices(true);
          else
            smfregistration.setEmergencyServices(false);
        } else if (
            !strcmp("pcscfRestorationCallbackUri", fields[i].c_str()) &&
            row[i] != NULL) {
          smfregistration.setPcscfRestorationCallbackUri(row[i]);
        } else if (!strcmp("plmnId", fields[i].c_str())) {
          PlmnId plmnid;
          nlohmann::json::parse(row[i]).get_to(plmnid);
          smfregistration.setPlmnId(plmnid);
        } else if (!strcmp("pgwFqdn", fields[i].c_str()) && row[i] != NULL) {
          smfregistration.setPgwFqdn(row[i]);
        } else if (!strcmp("epdgInd", fields[i].c_str()) && row[i] != NULL) {
          if (strcmp(row[i], "0"))
            smfregistration.setEpdgInd(true);
          else
            smfregistration.setEpdgInd(false);
        } else if (
            !strcmp("deregCallbackUri", fields[i].c_str()) && row[i] != NULL) {
          smfregistration.setDeregCallbackUri(row[i]);
        } else if (
            !strcmp("registrationReason", fields[i].c_str()) &&
            row[i] != NULL) {
          RegistrationReason registrationreason;
          nlohmann::json::parse(row[i]).get_to(registrationreason);
          smfregistration.setRegistrationReason(registrationreason);
        } else if (
            !strcmp("registrationTime", fields[i].c_str()) && row[i] != NULL) {
          smfregistration.setRegistrationTime(row[i]);
        } else if (
            !strcmp("contextInfo", fields[i].c_str()) && row[i] != NULL) {
          ContextInfo contextinfo;
          nlohmann::json::parse(row[i]).get_to(contextinfo);
          smfregistration.setContextInfo(contextinfo);
        }
      } catch (std::exception e) {
        Logger::udr_mysql().error(
            " Cannot set values for SMF Registration: %s", e.what());
      }
    }
    to_json(tmp, smfregistration);
    j += tmp;
  }

  mysql_free_result(res);

  json_data = j;
  // code          = HTTP_STATUS_CODE_200_OK;

  Logger::udr_mysql().debug("SmfRegistrations GET: %s", j.dump().c_str());
  return true;
}

//------------------------------------------------------------------------------
bool mysql_db::query_smf_select_data(
    const std::string& ue_id, const std::string& serving_plmn_id,
    nlohmann::json& json_data) {
  // Check the connection with DB first
  if (!get_db_connection_status()) {
    Logger::udr_mysql().info(
        "The connection to the MySQL is currently inactive");
    return false;
  }

  MYSQL_RES* res                                            = nullptr;
  MYSQL_ROW row                                             = {};
  MYSQL_FIELD* field                                        = nullptr;
  nlohmann::json j                                          = {};
  SmfSelectionSubscriptionData smfselectionsubscriptiondata = {};
  const std::string query =
      "SELECT * FROM SmfSelectionSubscriptionData WHERE ueid='" + ue_id +
      "' AND servingPlmnid='" + serving_plmn_id + "'";

  if (mysql_real_query(
          &mysql_connector, query.c_str(), (unsigned long) query.size())) {
    Logger::udr_mysql().error(
        "mysql_real_query failure！ SQL Query: %s", query.c_str());
    return false;
  }

  res = mysql_store_result(&mysql_connector);
  if (res == NULL) {
    Logger::udr_mysql().error(
        "mysql_store_result failure！SQL Query: %s", query.c_str());
    return false;
  }

  row = mysql_fetch_row(res);

  if (row != NULL) {
    for (int i = 0; field = mysql_fetch_field(res); i++) {
      if (!strcmp("supportedFeatures", field->name) && row[i] != NULL) {
        smfselectionsubscriptiondata.setSupportedFeatures(row[i]);
      } else if (
          !strcmp("subscribedSnssaiInfos", field->name) && row[i] != NULL) {
        std ::map<std ::string, SnssaiInfo> subscribedsnssaiinfos;
        nlohmann::json::parse(row[i]).get_to(subscribedsnssaiinfos);
        smfselectionsubscriptiondata.setSubscribedSnssaiInfos(
            subscribedsnssaiinfos);
      } else if (
          !strcmp("sharedSnssaiInfosId", field->name) && row[i] != NULL) {
        smfselectionsubscriptiondata.setSharedSnssaiInfosId(row[i]);
      }
    }
    to_json(j, smfselectionsubscriptiondata);
    json_data = j;

    Logger::udr_mysql().debug(
        "SmfSelectionSubscriptionData GET: %s", j.dump().c_str());
  } else {
    Logger::udr_mysql().error(
        "SmfSelectionSubscriptionData no data！SQL Query: %s", query.c_str());
  }

  mysql_free_result(res);
  return true;
}
