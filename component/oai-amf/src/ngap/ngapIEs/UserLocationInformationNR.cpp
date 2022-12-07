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

#include "UserLocationInformationNR.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UserLocationInformationNR::UserLocationInformationNR() {}

//------------------------------------------------------------------------------
UserLocationInformationNR::~UserLocationInformationNR() {}

//------------------------------------------------------------------------------
void UserLocationInformationNR::setInformationNR(
    const NR_CGI& m_nR_CGI, const TAI& m_tAI) {
  nR_CGI = m_nR_CGI;
  tAI    = m_tAI;
}

//------------------------------------------------------------------------------
bool UserLocationInformationNR::encode2UserLocationInformationNR(
    Ngap_UserLocationInformationNR_t* userLocationInformationNR) {
  if (!nR_CGI.encode2NR_CGI(&userLocationInformationNR->nR_CGI)) {
    return false;
  }
  if (!tAI.encode2TAI(&userLocationInformationNR->tAI)) {
    return false;
  }
#if 0
		if(istimeStampSet)
		{
			Ngap_TimeStamp_t *ieTimeStamp = (Ngap_TimeStamp_t *)calloc(1,sizeof(Ngap_TimeStamp_t));
			if(!timeStamp->encodefromTimeStamp(ieTimeStamp))
			{
				free(ieTimeStamp);
				return false;
			}
			userLocationInformationEUTRA->timeStamp = ieTimeStamp;
		}
#endif
  return true;
}

//------------------------------------------------------------------------------
bool UserLocationInformationNR::decodefromUserLocationInformationNR(
    Ngap_UserLocationInformationNR_t* userLocationInformationNR) {
  if (!nR_CGI.decodefromNR_CGI(&userLocationInformationNR->nR_CGI)) {
    return false;
  }

  if (!tAI.decodefromTAI(&userLocationInformationNR->tAI)) {
    return false;
  }
#if 0
		if(userLocationInformationEUTRA->timeStamp)
		{
			istimeStampSet = true;
			timeStamp = new TimeStamp();
			if(!timeStamp->decodefromTimeStamp(userLocationInformationEUTRA->timeStamp))
			{
				return false;
			}
		}
#endif
  return true;
}

//------------------------------------------------------------------------------
void UserLocationInformationNR::getInformationNR(NR_CGI& m_nR_CGI, TAI& m_tAI) {
  m_nR_CGI = nR_CGI;
  m_tAI    = tAI;
}
}  // namespace ngap
