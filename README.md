# ft_ping
ft_ping project at 42 School Paris
## Documentation
### ICMP
**ICMP** (Internet Network Message Protocol) is a OSI Network layer protocol that is built on the IP protocol.
It is used to send network errors and operational information when communicating with another IP address.
#### PDU
The **PDU** (Protocol Data Unit) of ICMP is a **packet**.
It's composed of the ICMP header followed by data.
#### Header
ICMP packet header is composed of:
- Type (1 byte): ICMP type.
- Coce (1 byte): ICMP subtype.
- Checksum (2 bytes): Computed from header and data size for error checking.
- Rest of header (4 bytes): Content vary based on the ICMP type and code.
