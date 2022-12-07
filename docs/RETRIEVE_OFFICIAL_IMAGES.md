<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface 5G Core Network Deployment : Pulling Container Images</font></b>
    </td>
  </tr>
</table>

# This page is only valid for an `Ubuntu` host.

If you are using any other distributions, please refer to [Build your own images](./BUILD_IMAGES.md).

If you want to use a specific branch or commit, please refer to [Build your own images](./BUILD_IMAGES.md).

# Pulling the images from Docker Hub #

The images are hosted under the oai account `oaisoftwarealliance`. 

**All images that are currently pushed to Docker-Hub have an `Ubuntu-18.04` base image.**

**But they should be working on any Ubuntu newer hosts (such as `20.04` or `22.04`).**

Once again you may need to log on [docker-hub](https://hub.docker.com/) if your organization has reached pulling limit as `anonymous`.

```bash
$ docker login
Login with your Docker ID to push and pull images from Docker Hub. If you don't have a Docker ID, head over to https://hub.docker.com to create one.
Username:
Password:
```

The OAI CI/CD team has automated more frequent pushes to Docker-Hub on `oaisoftwarealliance` account. Two important things to be noted:
  - We will keep pushing to the `latest` tag when a milestone is reached current `latest` tag correspond to v1.4.0 release.
  - We are making pushes on the `develop` tag whenever a contribution has been accepted. These images are **EXPERIMENTAL**.
  - Release tag `vx.x.x` contains the release code

Now pull images according to your requirement,

```bash
#!/bin/bash
docker pull oaisoftwarealliance/oai-amf:v1.4.0
docker pull oaisoftwarealliance/oai-nrf:v1.4.0
docker pull oaisoftwarealliance/oai-spgwu-tiny:v1.4.0
docker pull oaisoftwarealliance/oai-smf:v1.4.0
docker pull oaisoftwarealliance/oai-udr:v1.4.0
docker pull oaisoftwarealliance/oai-udm:v1.4.0
docker pull oaisoftwarealliance/oai-ausf:v1.4.0
docker pull oaisoftwarealliance/oai-upf-vpp:v1.4.0
docker pull oaisoftwarealliance/oai-nssf:v1.4.0
# Utility image to generate traffic
docker pull oaisoftwarealliance/trf-gen-cn5g:latest
```

And **re-tag** them for tutorials' docker-compose file to work.

```bash
#!/bin/bash
docker image tag oaisoftwarealliance/oai-amf:v1.4.0 oai-amf:v1.4.0
docker image tag oaisoftwarealliance/oai-nrf:v1.4.0 oai-nrf:v1.4.0
docker image tag oaisoftwarealliance/oai-smf:v1.4.0 oai-smf:v1.4.0
docker image tag oaisoftwarealliance/oai-spgwu-tiny:v1.4.0 oai-spgwu-tiny:v1.4.0
docker image tag oaisoftwarealliance/oai-udr:v1.4.0 oai-udr:v1.4.0
docker image tag oaisoftwarealliance/oai-udm:v1.4.0 oai-udm:v1.4.0
docker image tag oaisoftwarealliance/oai-ausf:v1.4.0 oai-ausf:v1.4.0
docker image tag oaisoftwarealliance/oai-upf-vpp:v1.4.0 oai-upf-vpp:v1.4.0
docker image tag oaisoftwarealliance/oai-nssf:v1.4.0 oai-nssf:v1.4.0
docker image tag oaisoftwarealliance/trf-gen-cn5g:latest trf-gen-cn5g:latest
```

Finally you may logoff --> your token is stored in plain text..

```bash
$ docker logout
```

We will push new versions when new features are validated.

# Synchronizing the tutorials #

**CAUTION: PLEASE READ THIS SECTION VERY CAREFULLY!**

This repository only has tutorials and Continuous Integration scripts.

**At the time of writing (2022/07), the release tag is `v1.4.0`.**

| CNF Name    | Branch Name | Tag      | Ubuntu 18.04 | RHEL8 (UBI8)    |
| ----------- | ----------- | -------- | ------------ | ----------------|
| FED REPO    | N/A         | `v1.4.0` |              |                 |
| AMF         | `master`    | `v1.4.0` | X            | X               |
| SMF         | `master`    | `v1.4.0` | X            | X               |
| NRF         | `master`    | `v1.4.0` | X            | X               |
| SPGW-U-TINY | `master`    | `v1.4.0` | X            | X               |
| UDR         | `master`    | `v1.4.0` | X            | X               |
| UDM         | `master`    | `v1.4.0` | X            | X               |
| AUSF        | `master`    | `v1.4.0` | X            | X               |
| UPF-VPP     | `master`    | `v1.4.0` | X            | X               |
| NSSF        | `master`    | `v1.4.0` | X            | X               |

```bash
# Clone directly on the latest release tag
$ git clone --branch v1.4.0 https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed.git
$ cd oai-cn5g-fed
# If you forgot to clone directly to the latest release tag
$ git checkout -f v1.4.0

# Synchronize all git submodules
$ ./scripts/syncComponents.sh 
---------------------------------------------------------
OAI-NRF     component branch : master
OAI-AMF     component branch : master
OAI-SMF     component branch : master
OAI-SPGW-U  component branch : master
OAI-AUSF    component branch : master
OAI-UDM     component branch : master
OAI-UDR     component branch : master
OAI-UPF-VPP component branch : master
OAI-NSSF    component branch : master
---------------------------------------------------------
git submodule deinit --all --force
git submodule init
git submodule update
```

Later versions of the `master` branch may not work with the pulled images.

You are ready to [Configure the Containers](./CONFIGURE_CONTAINERS.md).

You can also go [back](./DEPLOY_HOME.md) to the list of tutorials.
