
# WLAN Cheatsheet

WLANs can operate in 3 different frequency bands:

  * 2.4GHz (802.11b/g/n)
  * 3.6GHz (802.11y)
  * 4.9/5.0GHz (802.11a/h/j/n)

Each of these ranges is divided into multiple **channels**. 

Every country has allowed channels and maximum power levels.

## 802.11b/g/n Channels

There are 14 channels. **1st channel** frequency is **2412 MHz**. Each
next level -- **+5 MHz** up to 13rd channel (2472 MHz). 

14th channel is 2484 MHz.

Wireless Card can only be on **one channel at a time**. 
So it can not sniff on all channels and bands at the same time.

Wireless Card needs to be capable of operating in the given range:
**a?b?g?n?h?**. Alfa Network can operates in b/g.

To change channel:

```
iwconfig <iw_device> channel 1
```

## Understanding WLAN Packet Types

3 types of packets:

  * Management
  * Control
  * Data

**Subtypes** exist for each of the above. Full details available
in IEEE Specification.

## Understanding the Access Point

  * Access Point is configured with an SSID (Service set identifier)

  * This SSID acts as a network name for discovery

  * Clients search for this access point or network 
  (which can consists of multiple access points) using this SSID

  * Access Point sends out **broadcast frames** called **Beacon 
  Frames** to announce its presence


Anyone can create and transmit beacon frames.

All clients will list that as a new access point. No protection
mechanism available.

Client Connection flow:

  1. Client sends **IEEE 802.11 Probe Request**
  2. Client gets **IEEE 802.11 Probe Response**
  3. Client sends **IEEE 802.11 Open System Authentication Request**
  4. Cleint gets **IEEE 802.11 Open System Authentication Response**
  5. Client sends **IEEE 802.11 Association Request**
  6. Client gets **IEEE 802.11 Association Response**

Terminology:

  * **STA** -- STAtion (Wireless Client)
  * **BSS** -- Basic Service Set (set of nodes communicating with each 
  other)

    * Infrastructure BSS (AP and Clients)
    * Independent BSS (Ad-Hoc Clients)

  * **ESS** -- Extended Service Set (set of connected BSSs)

  * **BSSID** -- Basic Service Set Identifier

    * Infrastructure BSS (MAC address of AP)
    * IBSS (Randomly Chosen MAC address by First Client in Ad-Hoc Mode
    which creates Ad-Hoc network)

  * **DS** -- Distribution System (connected BSSs in an ESS)

## WLAN Packet Header

  1. **Frame Control** -- 2 bytes
  2. **Duration/ID** -- 2 bytes
  3. **Address 1** -- 6 bytes
  4. **Address 2** -- 6 bytes
  5. **Address 3** -- 6 bytes
  6. **Sequence Control** -- 2 bytes
  7. **Address 4** -- 6 bytes
  8. **QoS Control** -- 2 bytes
  9. **Frame Body** -- 0 to 23124 bytes
  10. **FCS** -- 4 bytes

### Frame Control

So let's first look at **Frame Control Field**:

  1. **Protocol** -- 2 bits
  
  **Protocol** has its default value **0** and it may change when a
  major revision happens incompatible with the previous version.
  So for all practical purposes currently the **protocol field**
  will always remain zero in every 802.11 packet.

  2. **Type** -- 2 bits

  There are 3 types: Management, Control and Data. Beacon Frame
  is Management frame.

  3. **SubType** -- 4 bits

  Each type has multiple **subtypes**. Subtype **8** is Beacon Frame.

  4. **To DS** -- 1 bit
  5. **From DS** -- 1 bit


 To DS | From DS | Interpretation
--------|---------|-----------------
0 | 0 | STA to STA in same IBSS (Ad-Hoc) or Management or Control Frames
0 | 1 | Exiting the Distribution System (DS)
1 | 0 | Entering the DS
1 | 1 | Used in Wireless Distribution Systems (WDS)


  6. **More Frag** -- 1 bit

  Indicates if **more fragments** of the current frame are to follow;
  only application to Data and Management frames

  7. **Retry** -- 1 bit

  Indicates if the current frame is an original frame or it is a 
  **retransmission**; if it is a retransmission the **retry bit** is
  set to **1**.

  It is applicable only to Management and Data Frames.
  
  8. **Power Mgmt.** -- 1 bit

  Indicates if the STA in Power Save Mode or Active Mode
  **0** means Active Mode (will stay up in Wireshark)

  9. **More Data** -- 1 bit

  Indicates if more data to be followed; Data is queued up on the AP

  10. **Protected Frame** -- 1 bit

  **1** indicates that the Frame Body is encrypted:

    * Data frames
    * Management frames of Type Auth

  **0** indicates no encryption

  11. **Order** -- 1 bit

  Indicates that all received frames must be processed in order
 
### Duration / ID

It can be used either as Duration field or ID field in different
circumstances.

Duration / ID field is actually used to set Network Allocation
Vector (NAV). NAV is the minimum amount of time a STA
needs to wait before attempting transmission (it is used as **Duration
field**).

### Address fields

  * Value and Presence depends on Type/Sub-Type
  * Destination Address
  * Source Address
  * BSSID


### Sequence Control

Sequence Control field is broken up in two fields:

  * Sequence number of the packet
  * Fragment number of the packet

### QoS Control

  * Qualilty of Service Related
  * In Data Frames

### Frame Body

Contains the data payload:

  * Management frame details
  * Actual data

### FCS

  Frame check sequence (FCS) is basically **CRC** check over the 
  header and Frame Body.

## Hidden SSID

  * Turn SSID Broadcasting off in Beacon Frames
  * Just monitoring Beacon Frames will not give you the SSID
 
### Pwning Hidden SSIDs 

Multiple Techniques:

  * Monitor Air for a new Client trying to associate with the
  access point (passive)

  * De-authenticate one or all clients and monitor reconnections 
  (active)

Basic idea is to force the network to send Probe / Association
packets.

These packets **contain the SSID** even if not present in the Beacon 
frame from the access point.

```
airodump-ng mon0 --channel 1 --bssid <MAC-address-here>
```

Wireshark filter:

```
wlan.addr == <MAC-here>
```

Probe Request and Probe Response have SSID of the network.

simple way to send DeAuth packets:

```
aireplay-ng --deauth 0 -a <BSSID> mon0
```

spoof MAC

```
ip link set dev <interface> down
ip link set dev <interface> address XX:XX:XX:XX:XX:XX
ip link set dev <interface> up
```

## WLAN Authentication

2 types:

  * Open Authentication
  * Shared Authentication

**Open Authentication:** No "Actual" Authentication mechanism at all,
only 2 packets exchanged between Client and AP, and authentication 
ends 

Cases where authentication may fail is **MAC Filtering**.

**Shared Authentication:**

  * **WEP**

  **WEP encryption** requires two things: 

    * the first is 3 byte value called an **initialization vector** 
    which typically changing per packet

    * **WEP key**

  **WEP encryption algorithm** uses **RC4** and passes **IV** and 
  **WEP key** to RC4 algorithm which creates a **key string**

  Now this **key string** is actually XORed with plain text and put
  inside the packet and send across

Ok, now about **shared authentication**:

  * Client sends **Authentication request**
  * AP generates random string to challenge station (128 bytes of 
  plain text) and sends it to Client

  * The Client now uses Encryption Algorithm (RC4) to encrypt this 
  text by choosing some random **IV** along with it and then send this
  encrypted text and the **IV** back to the AP

  * the AP uses this **IV** and the WEP key which it has, puts it
  to the RC4 Algorithm, gets the **key string**, XORs the key string
  with the encrypted packet and that gives back the original plain
  text packet

  * Depending on if the encryption succeeded and the AP got back the
  original plain text it will send **authentication success** or
  **authentication failure**
  
**key string** as also called **WEP Keystream**.

## Hotspot Attacks

Hotspot Basics: 

  * Free / Paid WiFi based internet offered in public places

    * Coffee shops
    * Airport

  * Typically uses

    * Open Authentication
    * MAC Filtering at times
    * No Encryption
    * Can use captive portals for application layer authentication

Hotspot Attacks:

  * Create an Evil Twin:

    * Same ESSID
    * Same BSSID (optional)

  * Use De-Authentication attacks to break Client AP Connection
  * if Evil Twin has higher signal strength, then Client will
  connect to it

**Soft AP** is AP created entirely in software.

Client typically will request **DHCP request**, so attacker machine
should run **DHCP server** to provide an IP address to the client.

Now you has three options:

  1. PWN client machine
  2. Relay it to the access point (MITM)
  3. Relay to Internet Directly 

We can put Evil Twin on **different channel**.

Simple Way:

```
iwconfig mon0 channel 11
airbase-ng -a AA:AA:AA:AA:AA:AA -e MyAP_Name_Here mon0
```

**airbase-ng** creates soft AP on configured channel 11. 

Typical AP has wired and wireless interfaces which has different
MAC addresses.

So airbase-ng creates new tap interface **at0** so this is equivalent
of the wired side interface of the soft AP created by airbase-ng.

```
ip link set dev at0 up
```

If you don't supply DHCP address then client set **auto-configuration
IP address** that can actually be used to connect to the client 
in the LAN. And we can simply monitor the air and figure out 
what packets client is sending. One of these packet will be 
**gratuitous ARP request** packet where the client declares its
presence to the AP.

A gratuitous ARP request is an AddressResolutionProtocol request 
packet where the source and destination IP are both set to the IP of 
the machine issuing the packet and the destination MAC is the 
broadcast address ff:ff:ff:ff:ff:ff. 

# Isolated Clients

Clients usually have **Preffered Network List**. Preffered Networks
are networks which clients can automatically connect to in the future
wherenever the network is available.

So the next time client is anywhere near SSID in Preffered Network 
List it would automatically initiate a connection without prompting
the user.

It is typically a huge list with SSID and Credentials.

So let's assume this client is actually in an airport. Where non of
the networks he has previously connected to. The client starts
sending **Probe request** packets for networks in **Preffered Network
List**. As we know **Probe request** packet is a way to find whether a
given network is available in vicinity.

Now there are multiple cases possible for any given network that
stored in the client PNL i.e. ESSID stored in the PNL have either
of 3 configurations:

  * No Encryption
  * WEP
  * WPA/WPA2

We will deal with each of them separately.

## Case 1: Open Authentication, No Encryption

So what this means is that victim is going to send broadcast probe
request for SSID, the attacker will be sniffing and respond back
with the **Probe Response** for given SSID.

The Victim sends **Authentication Request** to which attacker simply
sends **Authentication Response Success**, then **Association 
Request** is send by the victim and it gets back **Association 
Response**; and after that **Data Exchange** happens

**Fundamental Problem:**

  * In Wi-Fi currently the client cannot authenticate the access point
  in any way

  * The SSID all alone is used to decide whom to connect to

  * Anyone can set a similar SSID and force a client to connect to
  their access point

  * This is especially true with **Hotspot SSIDs** as they by 
  definition are Open Authentication with no Encryption

simple cool stuff:

```
airbase-ng -P -C 10 -a AA:AA:AA:AA:AA:AA mon0 -v
```

**-P** option -- respond to all probes, even when specifying ESSIDs

**-C seconds** options -- enables beaconing of probed ESSID values

We see **SSID dancing**.

Case 2 and 3: WEP and WPA/WPA2 which use **Shared Key Authentication**

We will talk about these once later.

## Understanding Transmit Power

**dBm** is a logarithmic scale.

```
dBm = log10(mW) * 10
mW = 10^(dBm/10)
```

**mW** is **milliwatt**

so 30dBm is 1.00 watt


What is **EIRP**? **EIRP** stands for **Effective Isotropic Radiated
Power**

The whole idea is to calculate actual signal power

```
EIRP = Transmitter Power (in dBm)
+ Antenna Gain (in dBi)
- Cable loss (in dBm)
```

So max eirp is maximum power device is allowed to send out.

to change transmitter power:

```
iwconfig wlan0 txpower 30
```

get current **Regulatory Domain**:

```
iw reg get
```

set **Regulatory Domain**:

```
iw reg set BO
```

you can view different country regulatory domains in 
**wireless-regdb** repository

```
https://git.kernel.org/cgit/linux/kernel/git/sforshee/wireless-regdb.git/tree/db.txt
```

cool stuff: **Network bridge** 

```
# ip link add name bridge_name type bridge
# ip link set bridge_name up
```

## WEP Basics

The first encryption scheme made available for Wi-Fi; Uses RC4
encryption algorithm (Symmetric Key Encryption).

WEP Internals: **Frame Body**

Frame Body would contain 3 parts:

  * IV 
  * Data
  * ICV (Integrity Check Value), CRC32 algorithm

Data and ICV are **encrypted**, IV is not encrypted; it is prepended
with data and ICV:

```
-------------------
| IV | Data | ICV |
-------------------
``` 

Now let's come to the IV:

  * Init. Vector (3 bytes)
  * Pad (6 bits)
  * Key ID (2 bits)

```
-------------------------------
| Init. Vector | Pad | Key ID |
-------------------------------
```

**Key ID** represents number of 4 possible WEP keys that currently 
used.

WEP Step 1: Generating the Keystream

**IV** is generated randomly **per packet** basics. So AP generates
random IV (24 bits), appends WEP Key (which can be 40 or 104 bits),
so total is 64 or 128 bits

And this all goes to RC4 Algorithm which consists of two parts:
KSA (Key Scheduling Algorithm) + PRGA (Pseudo-Random Generation 
Algorithm)

Its output is **Random Keystream** which is used in encryption.


WEP Step 2: Generate Integrity Check Value

Let's assume we have **Data** which can be variable size. To this
data we apply CRC-32 Algorithm

```
DATA ========> ICV (32 bits)
      CRC-32
```

After that we concatenate **Data** and **ICV** together into one block

```
--------------
| DATA | ICV |
--------------
```

WEP Step 3: Cipher Text Generation

Now we **XOR Random Keystream with DATA + ICV**. And the output
is stored in **Cipher Text**

After that we prepend **IV** to the **Cipher Text**:

```
--------------------
| IV | Cipher Text |
--------------------
```

And this **IV** means **IV header** which consists of 24 bits IV +
1 byte (Pad 6 bits + Key ID 2 bits).

So **IV header  + Cipher Text** is actually **Frame Body** 

Decrypt Packets in Wireshark:

Go to 

```
Edit -> Preferences -> Protocols -> IEEE 802.11 -> Decryption Keys -> Edit
```

enter your key and tick **Enable decryption**

And after that all encrypted packets will be decrypted.

Also you can use **airdecap-ng** utility to decrypt packets.

Attacks:

  * Capture ARP packets

  How to figure out that it's the ARP packets? Aren't they all 
  encrypted? ARP packets are of a fixed unique size, easy to identify
  even if encrypted

  Capture ARP Request packets using encrypted packet size and 
  Destination, Replay them blindly, and see if the network responds
  back!

  So Attacker will send **Encrypted ARP Request Packet** and get 
  **Encrypted ARP Response** from AP and repeat this steps.

  * Caffe Latte Attack
  * Hirte Attack

## Understanding WPA/WPA2

WPA (Wi-Fi Protected Access):

  * Intermediate solution by Wi-Fi Alliance
  * Uses TKIP which is Based on WEP
  * Hardware changes not required (to upgrade from WEP)
  * Only Firmware upgrade

WPA2 :

  * Long Term solution (802.11i draft)
  * Uses CCMP which is Based on AES
  * Hardware changes required

Both of them have **Personal** and **Enterprise** edition

**Personal** edition uses **PSK** or **Pre-Shared Key**.

And **Enterprise** edition is based on the **802.1x + Radius server**,
Radius server is basically an authentication server.

In **WEP** we have static keys with which we apply encryption;
with **WPA** -- **no static** keys. This means we won't use
static keys for any kind of encryption which is going to happen
for data transfer.

Instead we are going to create **Dynamic Keys** on the 
**per connection** basics.

How are Dynamic Keys created? This is where WPA PSK or Pre-Shared Key
comes in.

  1. First you enter **Passphrase** which is 8 to 63 characters long
  2. This **Passphrase** is used as input to the function called
  **PBKDF2** it's a hashing algorithm which outputs 
  **Pre-Shared Key 256 bit**
  3. This process also going on the **AP** (when you enter passphrase
  in the web form)

### PBKDF2

  * Password Based Key Derivation Function
  * RFC 2898
  * It actually takes in 4 inputs: **Passphrase** which the user 
  enters, **SSID** of the AP, then **ssidLen** which is **length**
  of SSID, and then **4096**, and **256**; **4096** -- Number of times
  the passphrase is hashed by this function, **256** -- Intended Key
  Length of PSK
   
Now we've learnt how to derive **Pre-Shared Key** from the passphrase.
Now here comes the second part: How is the **dynamic key** derived?
This is what we are coming to **4-Way Handshake**. So how does it
work?

1. AP and Client should be configured to use PSK
2. After that Probe Request, Probe Response happens
3. Authentication Request, Authentication Response using Open 
Authentication
4. And finally Association Request, Association Response happens

In Probe Request and Probe Response packets there is **Security Info**
which contains types of encryption WPA, WPA2, Personal, 
Enterprise, ...

Now basically after step 4 we've finished association and we are about
to derive the key. At this point we will call the Client -- 
**Supplicant**, and the AP -- **Authenticator**.

So till now what we know is both of these have derived the same 
**Pre-Shared Key 256 bit** from the passphrase entered by the user.


After that **Authenticator** or AP first generates a long random 
value called **ANounce** and it sends this value as part of 
**Message 1** of the **4-Way Handshake** to the client.
This message contains a lot of fields.

The most important fields:
 
  * Key Nonce == ANounce
  * Key Replay Counter == n where n is some value based on previous
  history etc.

And client generates long random value called **Snounce**.
After that the Client generates **PTK (Pairwise Transient Key)** 
base on **ANounce** and **Snounce**.

Also to mention that Pre-Shared Key is also known as **PMK (Pairwise
Master Key)**

Airodump indicates if it captured **4-Way Handshake**.
**EAPOL** in wireshark.

How is **Pairwise Transient Key** created?

To keep it simple here 

```
PTK = Function(PMK, ANounce, SNounce, Authenticator MAC, Supplicant MAC)
```

  * **PMK** -- Pre-Shared Key (Pairwise Master Key)
  * **ANounce** -- Random by AP
  * **SNounce** -- Random by Client
  * **Authenticator MAC** -- AP MAC
  * **Supplicant MAC** -- Client MAC

Now the client sends **Message 2** which contains **SNounce**

Message 2: Key Nonce == SNounce, Key Replay Counter is the same
as in **Message 1**

Also the most important field is **MIC Key**, MIC -- is actually
message intergrity check and MIC computed over the body of this
EAPOL frame, **Key MIC** field first initialized to 0 in packet.

This **Message 2** also contains **Key Data** field which is
RSN information element -- the sending STA's RSN information
element. This information tells Client capabilities itself.

After the AP receives **Message 2** it can calculate valid PTK
and MIC. If both MIC: **received** and **calculated** are **matched**
it actually means the client derived the same PTK which would mean
the client also knows correct **Pre-Shared Key** and hence 
the next step AP sends **Message 3** which is **Key Installation 
Message** (**Key INstallation Request**).

However, at this stage if the MIC calculation does not match with
what the client has sent the AP would send back the **DeAuthentication
Message**.

Message 3 important fields: Key Install flag == 1 which means AP
tells the client to use PTK for the current session, 
Key Replay Counter == n+1

The Client after that sends **Key Install Acknowledgement**

Message 4 important fields: Key Ack == 0 -- this is the last message,
Key Replay Counter == n+1 (it's the matching message for the 
**Message 3**)

Only differences between WPA and WPA2 that WPA uses HMAC-MD5
for PTK and TKIP (RC4 for MIC), WPA2 uses HMAC-SHA1 for PTK and
CCMP (AES for MIC)

So **Key Descriptor Version == 1** in WPA, and **Key Descriptor
Version == 2** in WPA2

## WPA-Enterprise

  * Use a RADIUS server for authentication
  * Different supported EAP types -- EAP-MD5, PEAP, EAP-TLS etc.
  * **FreeRadius** -- De facto server
  * Depending on EAP type used Client and Server will need to be 
  configured

What is **FreeRadius-WPE** ? 

  * FreeRadius Wireless Pwnage Edition
  * A patch to the FreeRadius code 
  * It converts FreeRadius to Attack Server

Setting up the RADIUS Server (EAP-MD5, easy to setup but insecure):

  * Add a username / password in **users** file
  * Make eap-md5 the default EAP type in **eap.conf**
  * Ensure the shared secret is correct for the AP-RADIUS server
  in **clients.conf**

To sniff the packets for demonstration use **eap** keyword as filter
in wireshark. 

MD5 Mathematics:

```
Hash = MD5(EAP Response ID + Password + RADIUS Challenge)
```

Available to attacker:

  * Hash
  * Response ID
  * Challenge

## WPA/WPA2 Enterprise

| EAP Type | Real World Usage |
|----------|------------------|
| PEAP | Highest |
| EAP-TTLS | High |
| EAP-TLS | Medium |
| LEAP | Low |
| EAP-FAST | Low |




