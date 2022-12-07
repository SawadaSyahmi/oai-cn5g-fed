#!/bin/bash

set -euo pipefail

CONFIG_DIR="/openair-smf/etc"

# Default values
ENABLE_USAGE_REPORTING=${ENABLE_USAGE_REPORTING:-no}
USE_NETWORK_INSTANCE=${USE_NETWORK_INSTANCE:-no}
USE_LOCAL_SUBSCRIPTION_INFO=${USE_LOCAL_SUBSCRIPTION_INFO:-yes}
DOMAIN_ACCESS=${DOMAIN_ACCESS:-access.oai.org}
DOMAIN_CORE=${DOMAIN_CORE:-core.oai.org}
UE_MTU=${UE_MTU:-1358}
NSSAI_SST0=${NSSAI_SST0:-222}
NSSAI_SD0=${NSSAI_SD0:-123}
NSSAI_SST1=${NSSAI_SST1:-1}
NSSAI_SD1=${NSSAI_SD1:-1}
NSSAI_SST2=${NSSAI_SST2:-1}
NSSAI_SD2=${NSSAI_SD2:-1}
QOS_PROFILE_5QI0=${QOS_PROFILE_5QI0:-6}
QOS_PROFILE_5QI1=${QOS_PROFILE_5QI1:-7}
QOS_PROFILE_5QI2=${QOS_PROFILE_5QI2:-8}
SESSION_AMBR_UL0=${SESSION_AMBR_UL0:-20Mbps}
SESSION_AMBR_DL0=${SESSION_AMBR_DL0:-22Mbps}
SESSION_AMBR_UL1=${SESSION_AMBR_UL1:-20Mbps}
SESSION_AMBR_DL1=${SESSION_AMBR_DL1:-22Mbps}
SESSION_AMBR_UL2=${SESSION_AMBR_UL2:-20Mbps}
SESSION_AMBR_DL2=${SESSION_AMBR_DL2:-22Mbps}

if [[ ${USE_FQDN_DNS} == "yes" ]];then
    AMF_IPV4_ADDRESS=${AMF_IPV4_ADDRESS:-0.0.0.0}
    NRF_IPV4_ADDRESS=${NRF_IPV4_ADDRESS:-0.0.0.0}
    UPF_IPV4_ADDRESS=${UPF_IPV4_ADDRESS:-0.0.0.0}
    UDM_IPV4_ADDRESS=${UDM_IPV4_ADDRESS:-0.0.0.0}
fi

DEFAULT_CSCF_IPV4_ADDRESS=${DEFAULT_CSCF_IPV4_ADDRESS:-127.0.0.1}

HTTP_VERSION=${HTTP_VERSION:-1}

DNN_NI0=${DNN_NI0:-oai.ipv4}
TYPE0=${TYPE0:-IPv4}
DNN_RANGE0=${DNN_RANGE0:-'12.1.1.2 - 12.1.1.128'}
DNN_NI1=${DNN_NI1:-default}
TYPE1=${TYPE1:-IPv4}
DNN_RANGE1=${DNN_RANGE1:-'12.2.1.2 - 12.2.1.128'}
DNN_NI2=${DNN_NI2:-oai}
TYPE2=${TYPE2:-IPv4}
DNN_RANGE2=${DNN_RANGE2:-'12.1.1.129 - 12.1.1.224'}
DNN_NI3=${DNN_NI3:-ims}
TYPE3=${TYPE3:-IPv4v6}
DNN_RANGE3=${DNN_RANGE3:-'12.2.1.129 - 12.2.1.224'}

for c in ${CONFIG_DIR}/*.conf; do
    # grep variable names (format: ${VAR}) from template to be rendered
    if ! grep -oP '@[a-zA-Z0-9_]+@' ${c}; then
        echo "Configuration is already set"
        exec "$@"
    fi
    VARS=$(grep -oP '@[a-zA-Z0-9_]+@' ${c} | sort | uniq | xargs)
    echo "Now setting these variables '${VARS}'"

    # create sed expressions for substituting each occurrence of ${VAR}
    # with the value of the environment variable "VAR"
    EXPRESSIONS=""
    for v in ${VARS}; do
        NEW_VAR=`echo $v | sed -e "s#@##g"`
        if [[ -z ${!NEW_VAR+x} ]]; then
            echo "Error: Environment variable '${NEW_VAR}' is not set." \
                "Config file '$(basename $c)' requires all of $VARS."
            exit 1
        fi
        EXPRESSIONS="${EXPRESSIONS};s|${v}|${!NEW_VAR}|g"
    done
    EXPRESSIONS="${EXPRESSIONS#';'}"

    # render template and inline replace config file
    sed -i "${EXPRESSIONS}" ${c}
done
echo "Done setting the configuration"
exec "$@"
