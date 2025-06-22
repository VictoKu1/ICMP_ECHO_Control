# ICMP\_ECHO\_Control

A C-language project for learning and demonstrating the fundamentals of ICMP communication and network analysis, built as a university communications assignment.
The repository contains **two independent parts**:

1. ICMP Echo Request/Reply implementation (like a simple `ping`)
2. ICMP packet sniffer and analyzer
   
---

## Overview

This project covers **ICMP packet manipulation and network analysis** on Linux.
**Part A** demonstrates how to construct and send ICMP Echo Request packets using raw sockets, receive replies, and measure round-trip time.
**Part B** implements a simple ICMP packet sniffer in promiscuous mode, capturing and displaying ICMP packet details in real time.

---

## Project Structure

```
ICMP_ECHO_Control/
│
├── Part_A/
│   ├── myPing.c
│   └── makefile
│
└── Part_B/
    ├── main.c
    ├── header.h
    └── Makefile
```

---

## Getting Started

### Prerequisites

* **Linux OS**
* **C Compiler** (`gcc`)
* **Root privileges** (`sudo` required for raw sockets)
* **Wireshark** (recommended for packet verification)

---

## Part A: ICMP Echo (Ping) Implementation

Implements a simple ping-like utility in C.

### Features

* Sends a single ICMP Echo Request to `8.8.8.8` (Google DNS).
* Waits for the Echo Reply.
* Measures and prints round-trip time (RTT) in milliseconds and microseconds.

### Usage

```sh
cd Part_A
make
sudo ./myPing
```

**Expected Output:**
Message sent/received confirmation and RTT.

---

## Part B: ICMP Packet Sniffer & Analyzer

A raw socket-based sniffer for all network traffic, focusing on ICMP.

### Features

* Runs in promiscuous mode and captures all packets.
* Filters for ICMP packets, extracts, and displays:

  * Timestamp
  * Source/Destination IP
  * ICMP type & code
* Counts and displays every detected ICMP packet.

### Usage

```sh
cd Part_B
make
sudo ./main
```

**Expected Output:**
Real-time, detailed output for each captured ICMP packet.

---

## Testing & Verification

* Use `ping 8.8.8.8` in another terminal to generate ICMP traffic.
* Use **Wireshark** to cross-check captured packets.
* Part A: Confirm that your Echo Request is sent and a reply is received, with accurate RTT reporting.
* Part B: Confirm that the sniffer displays all ICMP packets, matching the number and details with Wireshark (e.g., sending 5 pings yields 10 ICMP packets: 5 requests, 5 replies).

---

**Educational use only. Built for network protocol learning and experimentation.**
If you have questions or want to contribute, feel free to open an issue or pull request!







