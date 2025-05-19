<img src="https://static.wixstatic.com/media/799fd4_0512b6edaeea4017a35613b4c0e9fc0b~mv2.jpg/v1/fill/w_1200,h_140,al_c,q_80,usm_0.66_1.00_0.01/networknext_logo_colour_black_RGB_tightc.jpg" alt="Network Next" width="600"/>

# Unreal Engine 5 Plugin

This repository contains the UE5 plugin for Network Next.

It's tested working with Unreal Engine 5.3.2

The plugin supports Win64, Linux, Mac, PS4, PS5 and GDK (XBoxOne, SeriesX and PC)

# Usage

1. Copy the entire **NetworkNext** folder into your **Plugins** directory.

2. Add the following to the bottom of **DefaultEngine.ini**
  
```[/Script/Engine.Engine]
!NetDriverDefinitions=ClearArray
+NetDriverDefinitions=(DefName="GameNetDriver",DriverClassName="/Script/NetworkNext.NetworkNextNetDriver",DriverClassNameFallback="/Script/NetworkNext.NetworkNextNetDriver")

[/Script/NetworkNext.NetworkNextNetDriver]
NetConnectionClassName="/Script/NetworkNext.NetworkNextConnection"

[NetworkNext]
Datacenter="local"
BuyerPublicKey="zkaPRGcAuThG6poXMJ8di/yKzgZEbqyQ6Ky951reRq4sgCm83lV24g=="
BuyerPrivateKey="zkaPRGcAuTiYwqkwWEmWSrsxpcJzErC1mkBz3W0PlWdSynr/uuS4jUbqmhcwnx2L/IrOBkRurJDorL3nWt5GriyAKbzeVXbi"
```
          
3. Scan all ini files in your game project in case somewhere else is clobbering the NetDriver setting. If this is the case the Network Next plugin will not work. This is a common failure point during integration for our Unreal Engine customers.

4. Replace the keypair values in **DefaultEngine.ini** with your own keys.

5. Edit your game mode blueprint to exec **UpgradePlayer** in response to the **OnPostLogin**

6. Set environment variables on the server, so Network Next knows where your server is running.

```
export NEXT_SERVER_ADDRESS=10.2.100.23:7777        # change to the public IP:port of your server
export NEXT_DATACENTER=cloud                       # autodetects datacenter in GCP or AWS
```

7. If setting environment variables is not possible, you can pass the values in using command line parameters

```
-serverAddress=10.2.100.23:7777
-datacenter=cloud
```

8. Secure yourself in production. Be absolutely sure not to ship the BuyerPrivateKey value to players with your game. It is a private value, and should only be accessible to your game server. The BuyerPublicKey value is OK to ship with your game.
