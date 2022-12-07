<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface 5G Core Network Deployment : Building Container Images</font></b>
    </td>
  </tr>
</table>

# 1.  Retrieve the correct network function branches #

**CAUTION: PLEASE READ THIS SECTION VERY CAREFULLY!**

This repository only has tutorials and Continuous Integration scripts.

Each 5G Network Function source code is managed in its own repository.

They are called as `git sub-modules` in the `component` folder.

Before doing anything, you SHALL retrieve the code for each git sub-module.

## 1.1. You are interested on a stable version. ##

We recommend to synchronize with the master branches on all git sub-modules.

We also recommend that you synchronize this "tutorial" repository with a provided tag. By doing so, the `docker-compose` files will be aligned with feature sets of each cNF.

**At the time of writing (30/06/2022), the release tag was `v1.4.0`.**

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
# Clone directly on the v1.4.0 release tag
$ git clone --branch v1.4.0 https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed.git
$ cd oai-cn5g-fed
# If you forgot to clone directly to the v1.4.0 release tag
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

or a little bit more dangerous

```bash
$ ./scripts/syncComponents.sh --nrf-branch v1.4.0 --amf-branch v1.4.0 \
                              --smf-branch v1.4.0 --spgwu-tiny-branch v1.4.0 \
                              --udr-branch v1.4.0 --udm-branch v1.4.0 \
                              --ausf-branch v1.4.0 --upf-vpp-branch v1.4.0 \
                              --nssf-branch v1.4.0
---------------------------------------------------------
OAI-NRF     component branch : v1.4.0
OAI-AMF     component branch : v1.4.0
OAI-SMF     component branch : v1.4.0
OAI-SPGW-U  component branch : v1.4.0
OAI-UDR     component branch : v1.4.0
OAI-UDM     component branch : v1.4.0
OAI-AUSF    component branch : v1.4.0
OAI-UPF-VPP component branch : v1.4.0
OAI-NSSF    component branch : v1.4.0
---------------------------------------------------------
git submodule deinit --all --force
git submodule init
git submodule update
```

## 1.2. You are interested on the latest features. ##

All the latest features are pushed to the `develop` branches of each NF repository.

It means that we/you are able to build and the Continuous Integration test suite makes sure it
does NOT break any existing tested feature.

So for example, at time of writing, N2 Handover support code is included in `v1.1.0` release.
But it is not tested yet. So it is not advertized in the `CHANGELOG.md` and the Release Notes.

Anyhow, the tutorials' docker-compose files on the latest commit of the `master` branch of
`oai-cn5g-fed` repository SHALL support any additional un-tested feature.

```bash
# Clone
$ git clone  https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed.git
$ cd oai-cn5g-fed
# On an existing repository, resync to the last `master` commit
$ git fetch --prune
$ git checkout master
$ git rebase origin/master

# Synchronize all git submodules
$ ./scripts/syncComponents.sh --nrf-branch develop --amf-branch develop \
                              --smf-branch develop --spgwu-tiny-branch develop \
                              --ausf-branch develop --udm-branch develop \
                              --udr-branch develop --upf-vpp-branch develop \
                              --nssf-branch develop
---------------------------------------------------------
OAI-NRF     component branch : develop
OAI-AMF     component branch : develop
OAI-SMF     component branch : develop
OAI-SPGW-U  component branch : develop
OAI-AUSF    component branch : develop
OAI-UDM     component branch : develop
OAI-UDR     component branch : develop
OAI-UPF-VPP component branch : develop
OAI-NSSF    component branch : develop
---------------------------------------------------------
git submodule deinit --all --force
git submodule init
git submodule update
```

# 2. Generic Parameters #

If you are re-building CN5G images, be careful that `docker` or `podman` may re-use `cached` blobs
to construct the intermediate layers.

We recommend to add the `--no-cache` option in that case.

## 2.1. On a Ubuntu Host ##

We are supporting the following releases:

* Ubuntu `18.04` or `bionic`
* Ubuntu `20.04` or `focal`
* Ubuntu `22.04` or `jammy`

When building, you can specify which base image to work on (by default it will be `ubuntu:bionic`).

You just add the `--build-arg BASE_IMAGE=ubuntu:xxxx` option.

# 3. Build AMF Image #

## 3.1 On a Ubuntu Host ##

For example, I am building using `ubuntu:focal` as base image:

```bash
$ docker build --target oai-amf --tag oai-amf:v1.4.0 \
               --file component/oai-amf/docker/Dockerfile.amf.ubuntu \
               --build-arg BASE_IMAGE=ubuntu:focal \
               component/oai-amf
$ docker image prune --force
$ docker image ls
oai-amf                 v1.4.0             f478bafd7a06        1 minute ago          179MB
...
```

## 3.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in your present working directory `pwd` before building the image, you can do it like below

```bash
$: mkdir -p ./etc-pki-entitlement ./rhsm-conf ./rhsm-ca
$: cp /etc/pki/entitlement/*pem ./etc-pki-entitlement
$: cp /etc/rhsm/ca/*pem ./rhsm-ca
$ sudo podman build --target oai-amf --tag oai-amf:v1.4.0 \
               --file component/oai-amf/docker/Dockerfile.amf.rhel8 \
               component/oai-amf
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.


# 4. Build SMF Image #

## 4.1 On a Ubuntu Host ##

For example, I am building using `ubuntu:22.04` as base image:

```bash
$ docker build --target oai-smf --tag oai-smf:v1.4.0 \
               --file component/oai-smf/docker/Dockerfile.smf.ubuntu \
               --build-arg BASE_IMAGE=ubuntu:22.04 \
               component/oai-smf
$ docker image prune --force
$ docker image ls
oai-smf                 v1.4.0             f478bafd7a06        1 minute ago          193MB
...
```

## 4.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in your present working directory `pwd` before building the image, you can do it like below

```bash
$: mkdir -p ./etc-pki-entitlement ./rhsm-conf ./rhsm-ca
$: cp /etc/pki/entitlement/*pem ./etc-pki-entitlement
$: cp /etc/rhsm/ca/*pem ./rhsm-ca
$ sudo podman build --target oai-smf --tag oai-smf:v1.4.0 \
               --file component/oai-smf/docker/Dockerfile.smf.rhel8 \
               component/oai-smf
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.

# 5. Build NRF Image #

## 5.1 On a Ubuntu Host ##

For example, I am building using `ubuntu:jammy` as base image:

```bash
$ docker build --target oai-nrf --tag oai-nrf:v1.4.0 \
               --file component/oai-nrf/docker/Dockerfile.nrf.ubuntu \
               --build-arg BASE_IMAGE=ubuntu:jammy \
               component/oai-nrf
$ docker image prune --force
$ docker image ls
oai-nrf                 v1.4.0             04334b29e103        1 minute ago          247MB
...
```

## 5.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in your present working directory `pwd` before building the image, you can do it like below

```bash
$: mkdir -p ./etc-pki-entitlement ./rhsm-conf ./rhsm-ca
$: cp /etc/pki/entitlement/*pem ./etc-pki-entitlement
$: cp /etc/rhsm/ca/*pem ./rhsm-ca
$ sudo podman build --target oai-nrf --tag oai-nrf:v1.4.0 \
               --file component/oai-nrf/docker/Dockerfile.nrf.rhel8 \
               component/oai-nrf
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.

# 6. Build SPGW-U Image #

## 6.1 On a Ubuntu Host ##

**CAUTION: SPGWU cannot be built using a ubuntu:22.04 or ubuntu:jammy base image.**

**It can only be `bionic` or `focal`.**

For example, I am building using `ubuntu:20.04` as base image:

```bash
$ docker build --target oai-spgwu-tiny --tag oai-spgwu-tiny:v1.4.0 \
               --file component/oai-upf-equivalent/docker/Dockerfile.ubuntu \
               --build-arg BASE_IMAGE=ubuntu:20.04 \
               component/oai-upf-equivalent
$ docker image prune --force
$ docker image ls
oai-spgwu-tiny          v1.4.0             dec6311cef3b        1 minute ago          155MB
...
```

## 6.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in your present working directory `pwd` before building the image, you can do it like below

```bash
$: mkdir -p ./etc-pki-entitlement ./rhsm-conf ./rhsm-ca
$: cp /etc/pki/entitlement/*pem ./etc-pki-entitlement
$: cp /etc/rhsm/ca/*pem ./rhsm-ca
$ sudo podman build --target oai-spgwu-tiny --tag oai-spgwu-tiny:v1.4.0 \
               --file component/oai-spgwu-tiny/docker/Dockerfile.rhel8 \
               component/oai-upf-equivalent
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.

# 7. Build AUSF Image #

## 7.1 On a Ubuntu Host ##

```bash
$ docker build --target oai-ausf --tag oai-ausf:v1.4.0 \
               --file component/oai-ausf/docker/Dockerfile.ausf.ubuntu \
               component/oai-ausf
$ docker image prune --force
$ docker image ls
oai-ausf          v1.4.0              77a96de94c23        1 minute ago        231MB
...
```

## 7.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in your present working directory `pwd` before building the image, you can do it like below

```bash
$: mkdir -p ./etc-pki-entitlement ./rhsm-conf ./rhsm-ca
$: cp /etc/pki/entitlement/*pem ./etc-pki-entitlement
$: cp /etc/rhsm/ca/*pem ./rhsm-ca
$ sudo podman build --target oai-ausf --tag oai-ausf:v1.4.0 \
               --file component/oai-ausf/docker/Dockerfile.ausf.rhel8 \
               component/oai-ausf
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.

# 8. Build UDM Image #

## 8.1 On a Ubuntu Host ##

```bash
$ docker build --target oai-udm --tag oai-udm:v1.4.0 \
               --file component/oai-udm/docker/Dockerfile.udm.ubuntu \
               component/oai-udm
$ docker image prune --force
$ docker image ls
oai-udm                 v1.4.0             10a4334e31be        1 minute ago          229MB
...
```

## 8.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in your present working directory `pwd` before building the image, you can do it like below

```bash
$: mkdir -p ./etc-pki-entitlement ./rhsm-conf ./rhsm-ca
$: cp /etc/pki/entitlement/*pem ./etc-pki-entitlement
$: cp /etc/rhsm/ca/*pem ./rhsm-ca
$ sudo podman build --target oai-udm --tag oai-udm:v1.4.0 \
               --file component/oai-udm/docker/Dockerfile.udm.rhel8 \
               component/oai-udm
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.

# 9. Build UDR Image #

## 9.1 On a Ubuntu Host ##

```bash
$ docker build --target oai-udr --tag oai-udr:v1.4.0 \
               --file component/oai-udr/docker/Dockerfile.udr.ubuntu \
               component/oai-udr
$ docker image prune --force
$ docker image ls
oai-udr                 v1.4.0             581e07d59ec3        1 minute ago          234MB
...
```

## 9.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in your present working directory `pwd` before building the image, you can do it like below

```bash
$: mkdir -p ./etc-pki-entitlement ./rhsm-conf ./rhsm-ca
$: cp /etc/pki/entitlement/*pem ./etc-pki-entitlement
$: cp /etc/rhsm/ca/*pem ./rhsm-ca
$ sudo podman build --target oai-udr --tag oai-udr:v1.4.0 \
               --file component/oai-udr/docker/Dockerfile.udr.rhel8 \
               component/oai-udr
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.


# 10. Build UPF-VPP Image #

## 10.1 On a Ubuntu Host ##

```bash
$ docker build --target oai-upf-vpp --tag oai-upf-vpp:v1.4.0 \
               --file component/oai-upf-vpp/docker/Dockerfile.upf-vpp.ubuntu \
               component/oai-upf-vpp
$ docker image prune --force
$ docker image ls
oai-upf-vpp             v1.4.0             581e07d59ec3        1 minute ago          937MB
...
```

## 10.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in your present working directory `pwd` before building the image, you can do it like below

```bash
$: mkdir -p ./etc-pki-entitlement ./rhsm-conf ./rhsm-ca
$: cp /etc/pki/entitlement/*pem ./etc-pki-entitlement
$: cp /etc/rhsm/ca/*pem ./rhsm-ca
$ sudo podman build --target oai-upf-vpp --tag oai-upf-vpp:v1.4.0 \
               --file component/oai-upf-vpp/docker/Dockerfile.upf-vpp.rhel7 \
               component/oai-upf-vpp
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.

# 11. Build NSSF Image #

## 11.1 On a Ubuntu Host ##

```bash
$ docker build --target oai-nssf --tag oai-nssf:v1.4.0 \
               --file component/oai-nssf/docker/Dockerfile.nssf.ubuntu \
               component/oai-nssf
$ docker image prune --force
$ docker image ls
oai-nssf          v1.4.0              77a96de94c23        1 minute ago        231MB
...
```

## 11.2 On a RHEL8 Host ##

RHEL base images generally needs a subscription to access the package repository.
For that the base image needs ca and entitlement .pem files.

Copy the ca and entitlement .pem files in your present working directory `pwd` before building the image, you can do it like below

```bash
$: mkdir -p ./etc-pki-entitlement ./rhsm-conf ./rhsm-ca
$: cp /etc/pki/entitlement/*pem ./etc-pki-entitlement
$: cp /etc/rhsm/ca/*pem ./rhsm-ca
$ sudo podman build --target oai-nssf --tag oai-nssf:v1.4.0 \
               --file component/oai-nssf/docker/Dockerfile.nssf.rhel8 \
               component/oai-nssf
...
```

The above command is with podman, in case of docker it can be changed with its docker equivalent.

# 12. Build Traffic-Generator Image #

This is just an utility image.

```bash
$ docker build --target trf-gen-cn5g --tag trf-gen-cn5g:latest \
               --file ci-scripts/Dockerfile.traffic.generator.ubuntu18.04 \
               .
```

You are ready to [Configure the Containers](./CONFIGURE_CONTAINERS.md) or deploying the images using [helm-charts](./DEPLOY_SA5G_HC.md)

You can also go [back](./DEPLOY_HOME.md) to the list of tutorials.
