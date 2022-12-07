#!/usr/bin/env bash
NWI_N3=access.oai.org
NWI_N6=core.oai.org
GW_ID=1
MNC03=95
MCC=208
REALM=3gppnetwork.org
NETWORK_UE_IP=12.2.1.0/24
INIT_CONF_PATH=
VPP_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/vpp_plugins/ # Ubntu18.04
INTERFACE_ACCESS=cn5g-access   # N3
INTERFACE_CORE=cn5g-core       # N6
INTERFACE_CP=demo-oai          # N4
NSSAI_SD_0=123
SST=222
DNN=default
REGISTER_NRF=yes
NRF_IP_ADDR=192.168.70.130
NRF_PORT=8080
UUID=0beafa60-8211-4774-b54f-2a8a35a721fe #$(cat /proc/sys/kernel/random/uuid)
HTTP_VERSION=2

################ Gateway for N3(Gnb), N4(SMF), N6(DN/NAT Gateway) ################
#N3_IPV4_ADDRESS_REMOTE=${N3_IPV4_ADDRESS_REMOTE:-$(ifconfig $INTERFACE_ACCESS | grep "inet " | awk '{print $2}' | cut -d"." -f1-3)".1"}
N3_IPV4_ADDRESS_REMOTE=192.168.70.1

#N4_IPV4_ADDRESS_REMOTE=${N4_IPV4_ADDRESS_REMOTE:-$(ifconfig $INTERFACE_CP | grep "inet " | awk '{print $2}' | cut -d"." -f1-3)".1"}
N4_IPV4_ADDRESS_REMOTE=192.168.72.1

#N6_IPV4_ADDRESS_REMOTE=${N6_IPV4_ADDRESS_REMOTE:-$(ifconfig $INTERFACE_CORE | grep "inet " | awk '{print $2}' | cut -d"." -f1-3)".1"}
N6_IPV4_ADDRESS_REMOTE=192.168.73.135



################ Veth Pair IP in the VPP switch ################
#N3_IPV4_ADDRESS_LOCAL=${N3_IPV4_ADDRESS_LOCAL:-$(ifconfig $INTERFACE_ACCESS | grep "inet " | awk '{print $2}' | cut -d"." -f1-3)".202"}
N3_IPV4_ADDRESS_LOCAL=192.168.72.202

#N4_IPV4_ADDRESS_LOCAL=${N4_IPV4_ADDRESS_LOCAL:-$(ifconfig $INTERFACE_CP | grep "inet " | awk '{print $2}' | cut -d"." -f1-3)".202"}
N4_IPV4_ADDRESS_LOCAL=192.168.70.202

#N6_IPV4_ADDRESS_LOCAL=${N6_IPV4_ADDRESS_LOCAL:-$(ifconfig $INTERFACE_CORE | grep "inet " | awk '{print $2}' | cut -d"." -f1-3)".202"}
N6_IPV4_ADDRESS_LOCAL=192.168.73.202
