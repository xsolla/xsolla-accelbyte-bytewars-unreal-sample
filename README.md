# Byte Wars - Unreal Engine

## Overview

Byte Wars is the official tutorial game for Xsolla and AccelByte Gaming Services (AGS). It is intended to act as a sample project that can be used as a reference on the best practices to integrate our services into your game. We created Byte Wars from scratch as a fully functional offline game. This offline game was then brought online with the power of AccelByte’s platform by adding different services from each of our service areas like access, play and engagement. Every tutorial module walks you through a step by step guide to add a specific feature to Byte Wars which you can then translate into your own game.

## Prerequisites

* Use **Unreal Engine** version 5.7 source build from [Unreal Engine on GitHub](https://www.unrealengine.com/en-US/ue-on-github).

## Clone Byte Wars

This repository ships with the AccelByte and Xsolla plugins already wired in as submodules:
* Xsolla Store Unreal SDK under `Plugins/store-ue4-sdk`.
* Xsolla Backend Unreal SDK under `Plugins/xsolla-backend-unreal-sdk`, which bundles the AccelByte Unreal SDK, AccelByte Unreal Online Subsystem, and AccelByte Network Utilities plugins.

To clone the repository and checkout the submodules at the same time, run the following command:

```batch
git clone --recursive git@github.com:accelbyte-sdk/xsolla-bytewars-sample.git
```

## Stage the CA Certificate Bundle

Packaged builds need a CA root certificate bundle so libwebsockets/OpenSSL can verify the TLS
handshake when connecting to the AccelByte lobby websocket. Without it, packaged builds fail to
connect with `SSL error: unable to get local issuer certificate`.

Copy it from your engine's `Engine/Content/Certificates/ThirdParty/cacert.pem` to the game's
`Content/Certificates/cacert.pem` by running this command (replace `$UE_ROOT` with your engine install path):

```bash
mkdir -p Content/Certificates
cp "$UE_ROOT/Engine/Content/Certificates/ThirdParty/cacert.pem" Content/Certificates/cacert.pem
```

## Compile Byte Wars (Windows)

1. Right click on AccelByteWars.uproject, select **Switch Unreal Engine version**, then choose Unreal Engine version 5.7 that you already installed.
2. Open `AccelByteWars.sln` generated from the previous step using your preferred IDE.
3. Compile the game project using the **Development Editor - Win64**.

## Compile Byte Wars (Mac Apple Silicon)

Byte Wars supports native **Apple Silicon (`arm64`)** builds. Intel Macs are not supported.

1. Install full **Xcode.app** from the Mac App Store (Command Line Tools alone are not sufficient). Accept the license when prompted:
   ```bash
   sudo xcode-select -s /Applications/Xcode.app/Contents/Developer
   sudo xcodebuild -license accept
   ```
2. Install the **Xcode Metal Toolchain**:
   ```bash
   xcodebuild -downloadComponent MetalToolchain
   ```
3. Install **.NET 8 SDK** from [Microsoft](https://dotnet.microsoft.com/download/dotnet/8.0), required by UnrealBuildTool.
4. Generate the Xcode project files (replace `$UE_ROOT` with your UE 5.7 install path):
   ```bash
   "$UE_ROOT/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh" \
     -project="$(pwd)/AccelByteWars.uproject" -game -engine
   ```
5. Open `AccelByteWars.xcworkspace` in Xcode and build using the **Development Editor - Mac** configuration.

> **Note:** To run a locally packaged `.app` without a Developer ID certificate, set `bMacSignToRunLocally=True` in `Config/DefaultEngine.ini` under `[/Script/MacTargetPlatform.XcodeProjectSettings]`. Keep it `False` for distribution builds.

## Run Byte Wars Offline

### Game Client

#### Run via PIE

1. Open the Unreal Engine editor by double-clicking `AccelByteWars.uproject`, or run it via your IDE (Development Editor - Win64).
2. Click on PIE button to run the game.

#### Run via Editor Standalone

**Windows** (adjust path to your UE 5.7 install):
```batch
"C:\Path\To\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe" AccelByteWars.uproject -game
```

**macOS** (set `UE_ROOT` to the directory containing `Engine/`):
```bash
export UE_ROOT="$HOME/EpicGames/UE_5.7"
"$UE_ROOT/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor" \
  "$(pwd)/AccelByteWars.uproject" -game
```

#### Run via Packaged Game Client

1. Open the Unreal Engine editor.
2. Package the game for your desired platform, Development config, with AccelByteWars as the target.
3. Once the build completes, run the packaged game client.


### Game Server

#### Run via Packaged Game Server

1. Open the Unreal Engine editor.
2. Package the game for your desired platform, Development config, with AccelByteWarsServer as the target.
3. Once the build completes, run the packaged game server. Example (Windows):
   ```batch
    AccelByteWarsServer.exe -server -log
   ```

### Connect Game Client to Game Server Locally

1. Run both game client and game server.
2. On game client, make sure it's in main menu, open command prompt using ` (tilde key on keyboard)
3. Then run the following command to connect to game server.
   ```batch
   open 127.0.0.1:7777/Game/ByteWars/Maps/MainMenu/MainMenu
   ```

## Run Byte Wars Online

Follow along Byte Wars [Learning Paths](https://docs.accelbyte.io/gaming-services/tutorials/byte-wars/unreal-engine/learning-paths/). We suggest starting with the [Login with Device ID and Single Platform Authentication](https://docs.accelbyte.io/gaming-services/tutorials/byte-wars/unreal-engine/learning-paths/authentication/unreal-path-login-device-id-and-single-platform-auth/) if you're unsure where to start.

### Build and Run Steam or EOS

Steam and EOS config are separated in different `.Target.cs` and `DefaultEngine.ini`.

To target the platform when running from the Editor, use the launch parameter below. It switches the `DefaultEngine.ini` based on the platform.

```batch
// Switch Engine.ini to Steam
-customconfig=Steam

// Switch Engine.ini to EOS
-customconfig=EOS
```

To target the platform when packaging the game, use the launch parameter below. It switches the `Target.cs` and `DefaultEngine.ini` based on the platform.

```batch
// Retarget to Steam when packaging
-target=AccelByteWarsSteam

// Retarget to EOS when packaging
-target=AccelByteWarsEOS
```