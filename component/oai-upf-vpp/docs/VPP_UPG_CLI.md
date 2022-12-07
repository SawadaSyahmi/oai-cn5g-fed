# VPP-UPF CLI

## Available cli options for UPF

```bash
$ docker exec -it vpp-upf ./bin/vppctl show upf help
  adf                            show upf adf commands
  application                    show upf application <name>
  applications                   show upf applications [verbose]
  association                    show upf association
  bihash                         show upf bihash <v4-tunnel-by-key | v6-tunnel-by-key | qer-by-id | peer-index-by-ip> [detail|verbose]
  flows                          show upf flows
  flow                           show upf flow commands
  gtpu                           show upf gtpu commands
  node-id                        show upf node-id
  nwi                            show upf nwi
  pfcp                           show upf pfcp commands
  policy                         show upf policy
  proxy                          show upf proxy
  session                        show upf session
  specification                  show upf specification commands
  tdf                            show upf tdf commands
```

## Show UPF associations

```bash
$ docker exec -it vpp-upf ./bin/vppctl show upf association
Node: oai-smf-svc
  Recovery Time Stamp: 2022/09/02 07:26:31:000
  Sessions: 1
```

## Show PFCP session at UPF

```bash
$ docker exec -it vpp-upf ./bin/vppctl show upf session
CP F-SEID: 0x0000000000000001 (1) @ 192.168.70.133
UP F-SEID: 0x0000000000000001 (1) @ 192.168.70.202
  PFCP Association: 0
  TEID assignment per choose ID
PDR: 1 @ 0x7f6cef01cd00
  Precedence: 0
  PDI:
    Fields: 0000000d
    Source Interface: Access
    Network Instance: access.oai.org
    Local F-TEID: 218138074 (0x0d0085da)
            IPv4: 192.168.72.202
    UE IP address (source):
      IPv4 address: 12.1.1.2
    SDF Filter [1]:
      permit out ip from any to assigned 
  Outer Header Removal: GTP-U/UDP/IPv4
  FAR Id: 1
  URR Ids: [1] @ 0x7f6ceefde850
  QER Ids: [] @ 0x0
PDR: 2 @ 0x7f6cef01cd80
  Precedence: 0
  PDI:
    Fields: 0000000c
    Source Interface: Core
    Network Instance: core.oai.org
    UE IP address (destination):
      IPv4 address: 12.1.1.2
    SDF Filter [1]:
      permit out ip from any to assigned 
  Outer Header Removal: no
  FAR Id: 2
  URR Ids: [1] @ 0x7f6ceefde8e0
  QER Ids: [] @ 0x0
FAR: 1
  Apply Action: 00000002 == [FORWARD]
  Forward:
    Network Instance: core.oai.org
    Destination Interface: 1
FAR: 2
  Apply Action: 00000002 == [FORWARD]
  Forward:
    Network Instance: access.oai.org
    Destination Interface: 0
    Outer Header Creation: [GTP-U/UDP/IPv4],TEID:9acb0442,IP:192.168.72.141
URR: 1
  Measurement Method: 0002 == [VOLUME]
  Reporting Triggers: 0001 == [PERIODIC REPORTING]
  Status: 0 == []
  Start Time: 2022/09/02 07:29:30:476
  vTime of First Usage:       0.0000 
  vTime of Last Usage:        0.0000 
  Volume
    Up:    Measured:                    0, Theshold:                    0, Pkts:          0
           Consumed:                    0, Quota:                       0
    Down:  Measured:                    0, Theshold:                    0, Pkts:          0
           Consumed:                    0, Quota:                       0
    Total: Measured:                    0, Theshold:                    0, Pkts:          0
           Consumed:                    0, Quota:                       0
  Measurement Period:                   10 secs @ 2022/09/02 07:29:40:476, in     5.597 secs, handle 0x00000c01
```

## Get packet trace
* Add packet trace
```bash
$ docker exec -it vpp-upf ./bin/vppctl trace add af-packet-input 100
```
* Generate traffic
 ```bash
 $ docker exec -it oai-ext-dn ping -c4 12.1.1.3
PING 12.1.1.3 (12.1.1.3) 56(84) bytes of data.
64 bytes from 12.1.1.3: icmp_seq=1 ttl=63 time=0.661 ms
64 bytes from 12.1.1.3: icmp_seq=2 ttl=63 time=0.497 ms
64 bytes from 12.1.1.3: icmp_seq=3 ttl=63 time=0.533 ms
64 bytes from 12.1.1.3: icmp_seq=4 ttl=63 time=0.538 ms

--- 12.1.1.3 ping statistics ---
4 packets transmitted, 4 received, 0% packet loss, time 3056ms
rtt min/avg/max/mdev = 0.497/0.557/0.661/0.064 ms
 ``` 
* Show trace
```bash
$ docker exec -it vpp-upf ./bin/vppctl show trace
```
* Downlink packet in trace
```bash
Packet 2

02:00:14:156491: af-packet-input
  af_packet: hw_if_index 1 next-index 4
    tpacket2_hdr:
      status 0x20000081 len 98 snaplen 98 mac 66 net 80
      sec 0x6311cc53 nsec 0xaef062e vlan 0 vlan_tpid 0
02:00:14:156502: ethernet-input
  IP4: 02:42:c0:a8:49:87 -> 02:fe:b3:cb:dd:5f
02:00:14:156507: ip4-input
  ICMP: 192.168.73.135 -> 12.1.1.3
    tos 0x00, ttl 64, length 84, checksum 0xfb98 dscp CS0 ecn NON_ECN
    fragment id 0x27dd, flags DONT_FRAGMENT
  ICMP echo_request checksum 0x9de1 id 79
02:00:14:156512: ip4-lookup
  fib 1 dpo-idx 1 flow hash: 0x00000000
  ICMP: 192.168.73.135 -> 12.1.1.3
    tos 0x00, ttl 64, length 84, checksum 0xfb98 dscp CS0 ecn NON_ECN
    fragment id 0x27dd, flags DONT_FRAGMENT
  ICMP echo_request checksum 0x9de1 id 79
02:00:14:156519: upf-ip4-session-dpo
  upf_session1 seid 3 
  ICMP: 192.168.73.135 -> 12.1.1.3
    tos 0x00, ttl 63, length 84, checksum 0xfc98 dscp CS0 ecn NON_ECN
    fragment id 0x27dd, flags DONT_FRAGMENT
  ICMP echo_request checksum 0x9de1 id 79
02:00:14:156535: upf-ip4-flow-process
  upf_session1 cp-seid 0x0000000000000003
  FlowInfo - sw_if_index 1, next_index = 2
  proto 0x1, 12.1.1.3:0 <-> 192.168.73.135:0, seid 0x0000000000000003
  00000000: 03000000000000000000000000000000000000000c0101030000000000000000
  00000020: 00000000c0a849870000000001000000
  ICMP: 192.168.73.135 -> 12.1.1.3
    tos 0x00, ttl 63, length 84, checksum 0xfc98 dscp CS0 ecn NON_ECN
    fragment id 0x27dd, flags DONT_FRAGMENT
  ICMP echo_request checksum 0x9de1 id 79
02:00:14:156544: upf-ip4-input
  upf_session1 cp-seid 0x0000000000000003 pdr 2 far 2
  ICMP: 192.168.73.135 -> 12.1.1.3
    tos 0x00, ttl 63, length 84, checksum 0xfc98 dscp CS0 ecn NON_ECN
    fragment id 0x27dd, flags DONT_FRAGMENT
  ICMP echo_request checksum 0x9de1 id 79
02:00:14:156547: upf-ip4-forward
  upf_session1 cp-seid 0x0000000000000003 pdr 2 far 2
  ICMP: 192.168.73.135 -> 12.1.1.3
    tos 0x00, ttl 63, length 84, checksum 0xfc98 dscp CS0 ecn NON_ECN
    fragment id 0x27dd, flags DONT_FRAGMENT
  ICMP echo_request checksum 0x9de1 id 79
02:00:14:156552: upf4-encap
  GTPU encap to upf_session1 teid 0x9acb0442
02:00:14:156555: ip4-load-balance
  fib 1 dpo-idx 10 flow hash: 0x88bedf3d
  UDP: 192.168.72.202 -> 192.168.72.141
    tos 0x00, ttl 254, length 128, checksum 0xa9c4 dscp CS0 ecn NON_ECN
    fragment id 0x0000
  UDP: 15839 -> 2152
    length 108, checksum 0x0000
02:00:14:156558: ip4-rewrite
  tx_sw_if_index 3 dpo-idx 10 : ipv4 via 192.168.72.141 host-n3: mtu:1500 next:5 0242c0a8488d02fe9c90fdf90800 flow hash: 0x88bedf3d
  00000000: 0242c0a8488d02fe9c90fdf908004500008000000000fd11aac4c0a848cac0a8
  00000020: 488d3ddf0868006c000034ff00649acb044200000085010006004500
02:00:14:156563: host-n3-output
  host-n3 
  IP4: 02:fe:9c:90:fd:f9 -> 02:42:c0:a8:48:8d
  UDP: 192.168.72.202 -> 192.168.72.141
    tos 0x00, ttl 253, length 128, checksum 0xaac4 dscp CS0 ecn NON_ECN
    fragment id 0x0000
  UDP: 15839 -> 2152
    length 108, checksum 0x0000
```
* Uplink packet in trace
```bash
Packet 4

02:00:14:156976: af-packet-input
  af_packet: hw_if_index 3 next-index 4
    tpacket2_hdr:
      status 0x20000009 len 142 snaplen 142 mac 66 net 80
      sec 0x6311cc53 nsec 0xaf63908 vlan 0 vlan_tpid 0
02:00:14:156980: ethernet-input
  IP4: 02:42:c0:a8:48:8d -> 02:fe:9c:90:fd:f9
02:00:14:156981: ip4-input
  UDP: 192.168.72.141 -> 192.168.72.202
    tos 0x00, ttl 64, length 128, checksum 0xbc41 dscp CS0 ecn NON_ECN
    fragment id 0x6b83, flags DONT_FRAGMENT
  UDP: 2152 -> 2152
    length 108, checksum 0x0000
02:00:14:156982: ip4-lookup
  fib 2 dpo-idx 13 flow hash: 0x00000000
  UDP: 192.168.72.141 -> 192.168.72.202
    tos 0x00, ttl 64, length 128, checksum 0xbc41 dscp CS0 ecn NON_ECN
    fragment id 0x6b83, flags DONT_FRAGMENT
  UDP: 2152 -> 2152
    length 108, checksum 0x0000
02:00:14:156986: ip4-local
    UDP: 192.168.72.141 -> 192.168.72.202
      tos 0x00, ttl 64, length 128, checksum 0xbc41 dscp CS0 ecn NON_ECN
      fragment id 0x6b83, flags DONT_FRAGMENT
    UDP: 2152 -> 2152
      length 108, checksum 0x0000
02:00:14:156988: ip4-udp-lookup
  UDP: src-port 2152 dst-port 2152
02:00:14:156989: upf-gtpu4-input
  GTPU decap from gtpu_session1 teid 0x2d149b71 next 1 error 0
02:00:14:156997: upf-ip4-flow-process
  upf_session1 cp-seid 0x0000000000000003
  FlowInfo - sw_if_index 3, next_index = 2
  proto 0x1, 12.1.1.3:0 <-> 192.168.73.135:0, seid 0x0000000000000003
  00000000: 03000000000000000000000000000000000000000c0101030000000000000000
  00000020: 00000000c0a849870000000001000000
  ICMP: 12.1.1.3 -> 192.168.73.135
    tos 0x00, ttl 64, length 84, checksum 0x31fd dscp CS0 ecn NON_ECN
    fragment id 0x3179
  ICMP echo_reply checksum 0xa5e1 id 79
02:00:14:156998: upf-ip4-input
  upf_session1 cp-seid 0x0000000000000003 pdr 1 far 1
  ICMP: 12.1.1.3 -> 192.168.73.135
    tos 0x00, ttl 64, length 84, checksum 0x31fd dscp CS0 ecn NON_ECN
    fragment id 0x3179
  ICMP echo_reply checksum 0xa5e1 id 79
02:00:14:156998: upf-ip4-forward
  upf_session1 cp-seid 0x0000000000000003 pdr 1 far 1
  ICMP: 12.1.1.3 -> 192.168.73.135
    tos 0x00, ttl 64, length 84, checksum 0x31fd dscp CS0 ecn NON_ECN
    fragment id 0x3179
  ICMP echo_reply checksum 0xa5e1 id 79
02:00:14:156999: ip4-input
  ICMP: 12.1.1.3 -> 192.168.73.135
    tos 0x00, ttl 64, length 84, checksum 0x31fd dscp CS0 ecn NON_ECN
    fragment id 0x3179
  ICMP echo_reply checksum 0xa5e1 id 79
02:00:14:157000: ip4-lookup
  fib 1 dpo-idx 8 flow hash: 0x00000000
  ICMP: 12.1.1.3 -> 192.168.73.135
    tos 0x00, ttl 64, length 84, checksum 0x31fd dscp CS0 ecn NON_ECN
    fragment id 0x3179
  ICMP echo_reply checksum 0xa5e1 id 79
02:00:14:157003: ip4-rewrite
  tx_sw_if_index 1 dpo-idx 8 : ipv4 via 192.168.73.135 host-n6: mtu:1500 next:3 0242c0a8498702feb3cbdd5f0800 flow hash: 0x00000000
  00000000: 0242c0a8498702feb3cbdd5f080045000054317900003f0132fd0c010103c0a8
  00000020: 49870000a5e1004f000153cc11630000000033cc0200000000001011
02:00:14:157004: host-n6-output
  host-n6 
  IP4: 02:fe:b3:cb:dd:5f -> 02:42:c0:a8:49:87
  ICMP: 12.1.1.3 -> 192.168.73.135
    tos 0x00, ttl 63, length 84, checksum 0x32fd dscp CS0 ecn NON_ECN
    fragment id 0x3179
  ICMP echo_reply checksum 0xa5e1 id 79
```
