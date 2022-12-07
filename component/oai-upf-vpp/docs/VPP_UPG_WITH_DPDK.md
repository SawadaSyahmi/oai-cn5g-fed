# VPP-UPF With DPDK

## List Interfaces
```bash
$ sudo lshw -c network -businfo
Bus info          Device      Class          Description
========================================================
pci@0000:01:00.0              network        82599ES 10-Gigabit SFI/SFP+ Network Connection
pci@0000:01:00.1              network        82599ES 10-Gigabit SFI/SFP+ Network Connection
pci@0000:06:00.0  eno1        network        I350 Gigabit Network Connection
pci@0000:06:00.1              network        I350 Gigabit Network Connection
```

## Bind interfaces to DPDK compatible driver (e.g. uio_pci_generic here)
```bash
$ sudo dpdk-devbind.py -b uio_pci_generic 0000:01:00.0 --force
$ sudo dpdk-devbind.py -b uio_pci_generic 0000:01:00.1 --force
$ sudo dpdk-devbind.py -b uio_pci_generic 0000:06:00.1 --force
```

## Verify DPDK binding is good
```bash
$ sudo dpdk-devbind.py -s

Network devices using DPDK-compatible driver
============================================
0000:01:00.0 '82599ES 10-Gigabit SFI/SFP+ Network Connection 10fb' drv=uio_pci_generic unused=ixgbe,vfio-pci
0000:01:00.1 '82599ES 10-Gigabit SFI/SFP+ Network Connection 10fb' drv=uio_pci_generic unused=ixgbe,vfio-pci
0000:06:00.1 'I350 Gigabit Network Connection 1521' drv=uio_pci_generic unused=igb,vfio-pci

Network devices using kernel driver
===================================
0000:06:00.0 'I350 Gigabit Network Connection 1521' if=eno1 drv=igb unused=vfio-pci,uio_pci_generic *Active*

No 'Baseband' devices detected
==============================

No 'Crypto' devices detected
============================

No 'Eventdev' devices detected
==============================

No 'Mempool' devices detected
=============================

No 'Compress' devices detected
==============================

No 'Misc (rawdev)' devices detected
===================================

No 'Regex' devices detected
===========================
```

## Add dpdk pci address in startup.conf for VPP
```bash
dpdk {
 dev 0000:01:00.0 {name n3}
 dev 0000:06:00.1 {name n4}
 dev 0000:01:00.1 {name n6}
}
```
## Modify init.conf for UPF accordingly
e.g. for N3

```bash
set interface ip table n3 1
set interface mtu 9000 n3
set interface ip address n3 172.21.8.133/22
set interface state n3 up
```

## Verify interfaces at VPP
```bash
vpp# 
vpp# show hardware-interfaces 
              Name                Idx   Link  Hardware
local0                             0    down  local0
  Link speed: unknown
  local
n3                                 1     up   n3
  Link speed: 10 Gbps
  Ethernet address 90:e2:ba:c5:fc:04
  Intel 82599
    carrier up full duplex mtu 9000 
    flags: admin-up pmd maybe-multiseg tx-offload intel-phdr-cksum rx-ip4-cksum
    Devargs: 
    rx: queues 1 (max 128), desc 1024 (min 32 max 4096 align 8)
    tx: queues 2 (max 64), desc 1024 (min 32 max 4096 align 8)
    pci: device 8086:10fb subsystem 8086:0003 address 0000:01:00.00 numa 0
    max rx packet len: 15872
    promiscuous: unicast off all-multicast on
    vlan offload: strip off filter off qinq off
    rx offload avail:  vlan-strip ipv4-cksum udp-cksum tcp-cksum tcp-lro 
                       macsec-strip vlan-filter vlan-extend jumbo-frame scatter 
                       security keep-crc rss-hash 
    rx offload active: ipv4-cksum jumbo-frame scatter 
    tx offload avail:  vlan-insert ipv4-cksum udp-cksum tcp-cksum sctp-cksum 
                       tcp-tso macsec-insert multi-segs security 
    tx offload active: udp-cksum tcp-cksum multi-segs 
    rss avail:         ipv4-tcp ipv4-udp ipv4 ipv6-tcp-ex ipv6-udp-ex ipv6-tcp 
                       ipv6-udp ipv6-ex ipv6 
    rss active:        none
    tx burst function: ixgbe_xmit_pkts
    rx burst function: ixgbe_recv_scattered_pkts_vec

    rx frames ok                                          12
    rx bytes ok                                          740
    extended stats:
      rx_good_packets                                     12
      rx_good_bytes                                      740
      rx_q0_packets                                       12
      rx_q0_bytes                                        740
      mac_local_errors                                     3
      mac_remote_errors                                    1
      rx_size_64_packets                                   7
      rx_size_65_to_127_packets                            5
      rx_multicast_packets                                12
      rx_total_packets                                    12
      rx_total_bytes                                     740
n4                                 3    down  n4
  Link speed: unknown
  Ethernet address f0:1f:af:db:b9:c8
  Intel e1000
    carrier down 
    flags: admin-up pmd maybe-multiseg tx-offload intel-phdr-cksum rx-ip4-cksum
    Devargs: 
    rx: queues 1 (max 8), desc 1024 (min 32 max 4096 align 8)
    tx: queues 2 (max 8), desc 1024 (min 32 max 4096 align 8)
    pci: device 8086:1521 subsystem 1028:04cf address 0000:06:00.01 numa 0
    max rx packet len: 16383
    promiscuous: unicast off all-multicast on
    vlan offload: strip off filter off qinq off
    rx offload avail:  vlan-strip ipv4-cksum udp-cksum tcp-cksum vlan-filter 
                       vlan-extend jumbo-frame scatter keep-crc rss-hash 
    rx offload active: ipv4-cksum jumbo-frame scatter 
    tx offload avail:  vlan-insert ipv4-cksum udp-cksum tcp-cksum sctp-cksum 
                       tcp-tso multi-segs 
    tx offload active: udp-cksum tcp-cksum multi-segs 
    rss avail:         ipv4-tcp ipv4-udp ipv4 ipv6-tcp-ex ipv6-udp-ex ipv6-tcp 
                       ipv6-udp ipv6-ex ipv6 
    rss active:        none
    tx burst function: eth_igb_xmit_pkts
    rx burst function: eth_igb_recv_scattered_pkts

n6                                 2     up   n6
  Link speed: 10 Gbps
  Ethernet address 90:e2:ba:c5:fc:05
  Intel 82599
    carrier up full duplex mtu 9000 
    flags: admin-up pmd maybe-multiseg tx-offload intel-phdr-cksum rx-ip4-cksum
    Devargs: 
    rx: queues 1 (max 128), desc 1024 (min 32 max 4096 align 8)
    tx: queues 2 (max 64), desc 1024 (min 32 max 4096 align 8)
    pci: device 8086:10fb subsystem 8086:0003 address 0000:01:00.01 numa 0
    max rx packet len: 15872
    promiscuous: unicast off all-multicast on
    vlan offload: strip off filter off qinq off
    rx offload avail:  vlan-strip ipv4-cksum udp-cksum tcp-cksum tcp-lro 
                       macsec-strip vlan-filter vlan-extend jumbo-frame scatter 
                       security keep-crc rss-hash 
    rx offload active: ipv4-cksum jumbo-frame scatter 
    tx offload avail:  vlan-insert ipv4-cksum udp-cksum tcp-cksum sctp-cksum 
                       tcp-tso macsec-insert multi-segs security 
    tx offload active: udp-cksum tcp-cksum multi-segs 
    rss avail:         ipv4-tcp ipv4-udp ipv4 ipv6-tcp-ex ipv6-udp-ex ipv6-tcp 
                       ipv6-udp ipv6-ex ipv6 
    rss active:        none
    tx burst function: ixgbe_xmit_pkts
    rx burst function: ixgbe_recv_scattered_pkts_vec

    rx frames ok                                          16
    rx bytes ok                                         1068
    extended stats:
      rx_good_packets                                     16
      rx_good_bytes                                     1068
      rx_q0_packets                                       16
      rx_q0_bytes                                       1068
      mac_local_errors                                     2
      mac_remote_errors                                    2
      rx_size_64_packets                                   7
      rx_size_65_to_127_packets                            9
      rx_broadcast_packets                                 1
      rx_multicast_packets                                15
      rx_total_packets                                    16
      rx_total_bytes                                    1068
upf-nwi-access.oai.org             4     up   upf-nwi-access.oai.org
  Link speed: unknown
  GTPU
upf-nwi-core.oai.org               5     up   upf-nwi-core.oai.org
  Link speed: unknown
  GTPU
```

