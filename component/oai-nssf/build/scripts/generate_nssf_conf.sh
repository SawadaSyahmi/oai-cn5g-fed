# prompt has been removed for easier Ctrl+C Ctrl+V
# please update the following information according to your configuration

INSTANCE=1
PREFIX='/usr/local/etc/oai'
sudo mkdir -m 0777 -p $PREFIX
cp ../../etc/nssf.conf  $PREFIX
cp ../../etc/nssf_slice_config.yaml  $PREFIX

declare -A NSSF_CONF

NSSF_CONF[@INSTANCE@]=$INSTANCE
NSSF_CONF[@PREFIX@]=$PREFIX
NSSF_CONF[@PID_DIRECTORY@]='/var/run'

NSSF_CONF[@NSSF_NAME@]='OAI-NSSF'
NSSF_CONF[@NSSF_FQDN@]='oai-nssf'
NSSF_CONF[@NSSF_SLICE_CONFIG@]='/usr/local/etc/oai/nssf_slice_config.yaml'

NSSF_CONF[@SBI_IF_NAME@]='eth0'
NSSF_CONF[@SBI_PORT_HTTP1@]='80'
NSSF_CONF[@SBI_PORT_HTTP2@]='8080'
NSSF_CONF[@SBI_API_VERSION@]='v1'

for K in "${!NSSF_CONF[@]}"; do 
  egrep -lRZ "$K" $PREFIX | xargs -0 -l sed -i -e "s|$K|${NSSF_CONF[$K]}|g"
  ret=$?;[[ ret -ne 0 ]] && echo "Tried to replace $K with ${NSSF_CONF[$K]}"
done

