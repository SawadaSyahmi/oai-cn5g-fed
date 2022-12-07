<a href="https://openairinterface.org/">
    <img src="./images/oai_final_logo.png" alt="Openairinterface logo" title="Openairinterface" align="right" height="60" />
</a>

# OpenAirInterface 5G Core Network Deployment

Welcome to the tutorial home page of OAI 5g Core project. Here you can find lot of tutorials and help manuals. We regularly update these documents depending on the new feature set. 

## Table of content

- [Pre-requisites](./DEPLOY_PRE_REQUESITES.md)
- How to get the container images
    - [Pull the container images](./RETRIEVE_OFFICIAL_IMAGES.md)
    - [Build the container images](./BUILD_IMAGES.md)
- [Configuring the Containers](./CONFIGURE_CONTAINERS.md)
- 5G Core Network Deployment
    - [Using Docker-Compose, perform a `minimalist` deployment](./DEPLOY_SA5G_MINI_DEPLOYMENT.md)
    - [Using Docker-Compose, perform a `basic` deployment](./DEPLOY_SA5G_BASIC_DEPLOYMENT.md)
    - [Using Docker-Compose, perform a `basic-vpp` deployment with VPP implementation of UPF](./DEPLOY_SA5G_WITH_VPP_UPF.md)
    - [Using Docker-Compose, perform a `basic` deployment with Static UE IP address allocation](./DEPLOY_SA5G_BASIC_STATIC_UE_IP.md)
    - [Using Helm Chart, perform a `basic` deployment](./DEPLOY_SA5G_HC.md)
    - [Using Docker-Compose, doing network slicing](./DEPLOY_SA5G_SLICING.md)
- 5G Core Network Deployment and Testing with Ran Emulators
    - [Using Docker-Compose, perform a `basic` deployment and test with `OAI RF simulator`](https://gitlab.eurecom.fr/oai/openairinterface5g/-/tree/develop/ci-scripts/yaml_files/5g_rfsimulator)
    - [Using Docker-Compose, perform a `minimalist` deployment and test with `gnbsim`](./DEPLOY_SA5G_WITH_GNBSIM.md)
    - [Using Docker-Compose, perform a `basic` deployment and test with `UERANSIM`](./DEPLOY_SA5G_WITH_UERANSIM.md)
    - [Using Docker-Compose, perform a `basic` deployment and test with `My5g-RANTester`](./DEPLOY_SA5G_WITH_My5g-RANTester.md)
    - [Using Docker-Compose, when testing with Commercial UE, troubleshoot traffic issues](./TROUBLESHOOT_COTS_UE_TRAFFIC.md)
- The Developers Corner
    - [How to Deploy Developers Core Network and Basic Debugging](./DEBUG_5G_CORE.md)
    - [Using Virtual Machine, deploy and test with a Commercial UE](./DEPLOY_SA5G_VM_COTSUE.md)
- [Report an Issue or bug for Core Network Functions](./DEPLOY_SA5G_BASIC_DEPLOYMENT.md#8-report-an-issue)