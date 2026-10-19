# PlayTradeX SDK for Unreal Engine

**Native blockchain infrastructure for Unreal Engine games and interactive applications.**

PlayTradeX is a C++-based blockchain SDK designed to help game developers integrate EVM-compatible blockchain functionality without implementing wallet management, RPC networking, cryptography, secure storage, transaction execution, and platform-specific native code directly in gameplay systems.

The native PlayTradeX SDK provides the underlying blockchain functionality. The Unreal Engine plugin is the engine-facing integration layer for Unreal C++ and, where implemented, Blueprint workflows.

> **Release:** `v0.9.0`
>
> **Engine:** Unreal Engine
>
> **Core:** Native C++
>
> **Status:** Pre-release / integration validation
>
> **Platforms:** Must be confirmed against successful Unreal plugin builds and runtime tests
>
> **Stable target:** `1.0.0` planned for December

---

## Why PlayTradeX?

Blockchain integration in a game should not require every Unreal Engine team to build wallet handling, native cryptography, RPC networking, transaction signing, secure storage, ABI tooling, native bridges, and transaction workflows from scratch.

PlayTradeX puts these responsibilities behind a reusable native SDK and an Unreal-facing integration layer so game code can work with configured networks, wallet identifiers, and asynchronous operations.

The SDK feature history through Unity `v0.9.0-alpha` includes:

- A request pipeline for independent concurrent work and network-scoped sequential operations.
- Adaptive RPC scoring based on latency and reliability.
- Transaction lifecycle coordination.
- Nonce reservation for concurrent transactions.
- Transaction consent, signing, submission, and subsequent confirmation tracking.
- Background transaction receipt monitoring.
- Deterministic local transaction-hash calculation before submission.
- Recovery handling for uncertain transaction-submission outcomes.
- Nonce reconciliation.
- Concurrency and shutdown hardening.
- Sensitive-data cleanup.
- Multi-network EVM configuration.
- Multiple RPC endpoints per network.
- RPC validation, failover, cooldown, and endpoint recovery.
- Multiple application-managed wallets.
- Identity-wallet and external-wallet execution.
- Exact string-based blockchain unit conversion.

These capabilities are part of the documented native/shared SDK feature history. Their availability through the Unreal plugin must be confirmed against the actual Unreal-facing implementation.

# Features

## Blockchain

- Native EVM blockchain integration.
- Multiple configured EVM networks.
- Multiple RPC endpoints per network.
- RPC endpoint validation and chain-ID verification.
- Automatic failover between eligible RPC endpoints.
- Endpoint health tracking and cooldown/recovery.
- Latency-aware RPC routing where implemented.
- Native currency balance queries.
- Native currency transfers.
- Read-only smart contract calls.
- Transaction-producing smart contract writes.
- Payable contract writes where exposed by the integration.
- Network-aware execution.
- Application-controlled transaction consent where implemented.
- Asynchronous blockchain operations.
- Exact string-based blockchain unit conversion.

## Wallets

- Native PlayTradeX identity wallet.
- Multiple application-managed external wallets.
- Developer-defined wallet identifiers.
- Wallet generation and persistence where supported.
- Encrypted wallet export and import where implemented.
- Platform-specific secure storage where supported.
- Wallet recovery and import/export error handling.

Wallet generation, persistence, encryption, import/export, and secure storage must be verified for each target platform and the actual Unreal plugin release.

## Unreal Engine Developer Experience

The Unreal integration should provide an engine-appropriate interface for accessing native SDK operations.

Depending on the implementation included in the release, this may include:

- Unreal C++ wrapper classes.
- Blueprint-callable functions.
- Asynchronous Blueprint nodes or delegates.
- Unreal-friendly response structures.
- Game-thread callback dispatch.
- Plugin configuration and initialization support.
- Example C++ usage.
- An Unreal sample project or sample module.

Only interfaces actually present in the distributed plugin should be considered supported APIs. Unity Editor tools, Unity Project Settings, and generated C# classes are not Unreal features.

## Native Platform Integration

The native SDK is written in C++. Platform support depends on the native dependencies and Unreal plugin build configuration.

Potential target platforms must be validated individually:

- Windows
- Android
- macOS
- Linux
- iOS

Support for a platform in the Unity package does not automatically establish support for that platform in Unreal Engine.

---

# Platform Support

Do not advertise a platform as supported until the Unreal plugin has been compiled and tested on that platform.

| Platform | Status |
|---|---|
| Windows | Confirm with an Unreal build and runtime test |
| Android | Confirm native ABI, packaging, and device tests |
| macOS | Requires Unreal-specific validation |
| Linux | Requires Unreal-specific validation |
| iOS | Requires Unreal-specific build, signing, and device validation |

The exact supported Unreal Engine versions and target architectures should be documented after testing the corresponding plugin builds.

---

# Installation

## Install the Unreal Engine Plugin

1. Close Unreal Editor.
2. Copy the PlayTradeX plugin directory into your project's `Plugins/` directory, unless your distribution provides another installation method.
3. Verify that the plugin descriptor (`.uplugin`) and module `Build.cs` files are included.
4. Regenerate project files if required by your development environment.
5. Open the `.uproject` file.
6. Enable the PlayTradeX plugin if it is not already enabled.
7. Compile the project.
8. Open Unreal Editor and verify that the plugin loads successfully.
9. Configure the required storage location, network settings, and wallet settings using the configuration mechanism provided by the plugin.
10. Test against an EVM test network before using production assets.

The exact installation steps depend on the layout and packaging of the published Unreal plugin.

## Plugin Structure

A typical Unreal Engine plugin may use a structure similar to:

```text
YourProject/
└── Plugins/
    └── PlayTradeX/
        ├── PlayTradeX.uplugin
        ├── Source/
        │   └── PlayTradeX/
        │       ├── PlayTradeX.Build.cs
        │       ├── Public/
        │       └── Private/
        ├── Resources/
        └── Content/