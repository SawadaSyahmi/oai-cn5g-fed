# RELEASE NOTES: #

## v1.4.0 -- July 2022 ##

* All official images produced by CI are pushed to `oaisoftwarealliance` Docker-Hub Team account
* New tutorial:
  - My5gRAN Tester --> `docs/DEPLOY_SA5G_WITH_My5g-RANTester.md`
* Update the helm charts and helm chart tutorial
* Reduce image size for all network functions
* New wiki page to know more about variable which can be configured for each network function
* Fix HTTP/2 support for all network functions `docker-compose/docker-compose-basic-nrf-httpv2.yaml`
* `AMF` Changes:
  - Fix SCTP NG Shutdown issue
  - Fix 100% CPU consumption issue
  - Fix RAN UE ID display
  - Fix issue for TMSI and GUTI value
  - Fix for handling S-NSSAI 
  - Remove OPC value from AMF configuration
  - Support for Ubuntu 20.04 bare-metal installation
* `AUSF` Changes:
  - Remove unused packages
  - HTTP/2 support for SBI interface
  - NRF Registration and heartbeat exchange
* `NRF` Changes:
  - Remove unused packages
  - Add NF profile handling for PCF
  - HTTP/2 support for SBI interface
* `NSSF` Changes:
  - Network slice selection for UE registration 
  - NS-AVAIBALITY API support to CREATE new slice
  - Fix bare-metal setup configuration management
  - HTTP/2 support for SBI interface
* `SMF` Changes:
  - Fix for handling S-NSSAI 
  - Add support for IMS
  - HTTP/2 support for SBI interface
  - Add usage report feature
  - Flexiable MTU
  - Support for Ubuntu 20.04 bare-metal installation
* `UDM` Changes:
  - Fix HTTP/2 support
  - NRF Registration and heartbeat exchange
* `UDR` Changes:
  - Fix sync failure issue
  - Fix issue to support Slice Selection Subscription Data retrieval
  - NRF Registration and heartbeat exchange
* `SPGWU-TINY` Changes:
  - Fix for handling S-NSSAI 
  - Add support for multiple slices via config file
  - Allowed reusing GTPv1-U socket
  - Fix GTPU offset sequence number
  - Fix configurable number of DL threads for data path
* `UPF-VPP` Changes:
  - Standalone support validated for traffic steering and redirection
  - Added instructions for bare-metal installation
  - Support for Ubuntu 20.04 bare-metal/docker installation
* `NEF` It is ongoing so far it is not functional

## v1.3.0 -- January 2022 ##

* Incorporation of new public network functions:
  - NSSF
* New tutorials:
  - Slicing --> `docs/DEPLOY_SA5G_SLICING.md`
  - Static UE IP address to emulate private network --> `docs/DEPLOY_SA5G_BASIC_STATIC_UE_IP.md`
* `AMF` Changes:
  - Periodic Registration Update
  - Support for Event Exposure (Registration State Report)
  - Implicit Deregistration Timer
  - Mobile Reachable Timer
  - Mobility Registration Update
  - NRF Selection (via NSSF)
  - Fix for validation of SMF Info
  - Fix RAN UE NGAP Id
  - Fix SMF Selection SD
  - Fix NSSAI mismatch
  - Fix Service Request
  - Fix HTTP2
  - Fix MCC such as 001
  - Docker optimization
* `AUSF` Changes:
  - HTTP2 Support
  - Docker optimization
* `NRF` Changes:
  - NF profile update
  - Docker optimization
* `NSSF` Changes:
  - Initial Public Release
  - Docker Optimization
* `SMF` Changes:
  - Update NWI from NF profile
  - Update SMF profile
  - Added retry for PFCP association request
  - More flexible DNN/IP ranges when deploying container
  - Fix retrieving the list of NWI
  - Fix entreprise IE decoding
  - Fix issue for UPF selection
  - Fix for IE Measurement Period
  - Docker optimization
* `UDM` Changes:
  - Experimental support for Event Exposure
  - Added HTTP2 support
  - Fix static addr allocation
  - Fix build issue
  - Docker build optimizations
* `UDR` Changes:
  - Added HTTP2 support
  - Fix build issue
  - Fix static addr allocation
  - Docker build optimizations
* `SPGWU-TINY` Changes:
  - Disable association request if NF registration is enabled
* `UPF-VPP` Changes:
  - Upgrade to UPG plugin stable/1.2
  - Build fixes
  - Deployment fixes (wait launch of NRF Client until VPP is getting ready)

## v1.2.1 -- October 2021 ##

* Incorporation of new public network functions:
  - UPF-VPP
* New tutorial with this new function
* `AMF` Changes:
  - Fix build issue
  - Tentative fix for ngKSI already in use
  - Initial implementation for Mobility registration update
* `AUSF` Changes:
  - Fix build issue
* `NRF` Changes:
  - Fix build issue
* `SMF` Changes:
  - Timers handling: T3591/T3952
  - Fix build issue
  - Fix UPF selection
* `UDM` Changes:
  - Fix build issue
* `UDR` Changes:
  - Fix build issue
* `SPGWU-TINY` Changes:
  - Fix build issue
  - Fix GTPU DL encapsulation: 8 extraneous bytes
* `UPF-VPP` Changes:
  - Initial Public Release
  - Full support for Ubuntu18 and RHEL7
  - CI Build support
  - Tutorial validated

## v1.2.0 -- September 2021 ##

* Incorporation of new public network functions:
  - AUSF
  - UDM
  - UDR
* New tutorials with these new functions
* CI improvements
* `AMF` Changes:
  - AUSF connection
  - Support PDU Session Resource Modify procedure
  - Support HTTP2
  - Support AMF Event Exposure Services
  - Fix NIA0, NEA2
  - Fix potential AMF crash with UE/NGAP/NAS context
  - Fix N2 Handover
  - Fix Paging procedures
* `AUSF` Changes:
  - Initial public release
  - NRF registration
    - with FQDN DNS resolution
  - Full support for Ubuntu18 and RHEL8
* `NRF` Changes:
  - Added AUSF, UDR, UDM profiles
  - Updated UPF profile
* `SMF` Changes:
  - Event Exposure implemented
  - UPF profile update
  - Support for URR query
  - Bug fixes
* `UDM` Changes:
  - Initial public release
  - NRF registration
    - with FQDN DNS resolution
  - Full support for Ubuntu18 and RHEL8
* `UDR` Changes:
  - Initial public release
  - NRF registration
    - with FQDN DNS resolution
  - Proper mySQL DB deployment management
  - Full support for Ubuntu18 and RHEL8
* `SPGWU-TINY` Changes:
  - Adding 5G features
    - HTTP2 support

## v1.1.0 -- July 2021 ##

* Improvements on Continuous Integration:
  - DsTester validation on Ubuntu18 docker deployment
  - DsTester validation on RHEL8 / OpenShift deployment
  - Some components (such as AMF/NRF) have bracket-testing at unit level
* A lot of tutorials have been added
* `AMF` Changes:
  - Session Release
  - NRF registration
    - with FQDN DNS resolution
  - Multiple PDU support
  - Bug fixes
  - Full support for Ubuntu18 and RHEL8
* `NRF` Changes:
  - FQDN DNS resolution
  - Bug fixes
  - Full support for Ubuntu18 and RHEL8
* `SMF` Changes:
  - PFCP Release 16 support
  - NRF registration
    - with FQDN DNS resolution
  - Support for multiple UPF instances
  - Dotted DNN support
  - Use (SST, SD) to get the subscription information
  - Bug fixes
  - Full support for Ubuntu18 and RHEL8
* `SPGWU-TINY` Changes:
  - Adding 5G features
    - NRF discovery and FQDN support

## v1.0.0 -- September 2020 ##

* Initial release

