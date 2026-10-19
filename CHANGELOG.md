
## 2. `CHANGELOG.md`

```markdown
# Changelog

All notable changes to the PlayTradeX SDK are documented here.

This changelog records the Unity/shared SDK feature history from `v0.1.0-alpha` through `v0.9.0-alpha`, together with the Unreal Engine `v0.9.0` integration documentation and release preparation.

Unity-specific Editor tooling and C# APIs are not automatically available in Unreal Engine.

## [Unreal v0.9.0] — 2026-10-18

### Unreal Integration Documentation

- Added Unreal Engine-focused installation and integration guidance.
- Documented the native C++ SDK API surface.
- Documented Unreal plugin architecture and lifecycle considerations.
- Added guidance for asynchronous blockchain operations and safe game-thread interaction.
- Documented EVM operations, network configuration, wallet integration, and transaction lifecycle concepts.
- Added security recommendations and a release testing checklist.
- Clarified the difference between shared SDK capabilities and engine-specific APIs.
- Clarified that platform support must be confirmed through Unreal-specific builds and runtime tests.

### Validation

This entry records documentation and release preparation. It does not assert that all Unreal plugin features, platform targets, or release tests have been completed.

## Unity/Shared SDK Feature History

### [v0.9.0-alpha] — Transaction Recovery and Production Hardening

- Added deterministic local transaction hashing.
- Improved recovery from ambiguous transaction-submission outcomes.
- Improved nonce reconciliation.
- Hardened concurrency and shutdown handling.
- Improved sensitive-data cleanup.
- Improved wallet persistence and import/export recovery.

### [v0.8.0-alpha] — Transaction Events and Receipt Monitoring

- Added background receipt monitoring for submitted transactions.
- Added Unity mined-transaction events with transaction hash, receipt, and confirmed/reverted status.
- Separated initial submission results from later receipt outcomes.
- Improved request scheduling for independent operations and network-scoped sequential work.
- Improved transaction lifecycle and signer/network coordination.
- Continued adaptive RPC scoring and multi-network isolation.

### [v0.7.0-alpha] — Transaction Lifecycle and Nonce Coordination

- Added transaction lifecycle coordination across preparation, consent, signing, and submission.
- Added nonce reservations for concurrent approved transactions.
- Improved signer/network coordination.

### [v0.6.0-alpha] — Request Pipeline and Adaptive RPC Scoring

- Improved the request pipeline.
- Added adaptive RPC scoring based on latency and reliability history.

### [v0.5.0-alpha] — Latency-Aware RPC Routing

- Added latency-aware endpoint preference.
- Added successful-operation latency measurement.
- Improved endpoint scoring and preference.
- Added initial sampling of eligible endpoints before latency-based routing.

### [v0.4.0-alpha] — RPC Load Balancing and Concurrency Safety

- Added the round-robin/load-balancing foundation.
- Added runtime endpoint health cooldown and recovery.
- Improved concurrent endpoint validation and in-flight reservations.
- Added generation-safe load-balancer cursor handling.
- Isolated state for separate RPC pools.
- Centralized RPC resolution and improved transaction endpoint consistency.

### [v0.3.0-alpha] — RPC Validation and Failover

- Added validation for multiple RPC endpoints.
- Added chain-ID verification and wrong-network endpoint rejection.
- Added validation caching and temporary-failure cooldown/re-entry.
- Added automatic failover across configured endpoints.
- Improved consistency between transaction preparation and submission endpoints.
- Hardened Android integration.
- Optimized release binaries.

**Note:** This milestone focused on RPC validation and reliability, not latency-aware load balancing.

### [v0.2.0-alpha] — Multi-Network and Multi-Wallet

- Added configurable EVM networks with IDs, names, chain IDs, symbols, RPC endpoints, and explorer URLs.
- Added multiple application-managed wallets and wallet identifiers.
- Added network-aware execution.
- Added EVM testnet presets.
- Added generated network and wallet identifiers.
- Added Unity Project Settings and updater/about tooling.
- Expanded the Unity sample.

### [v0.1.0-alpha] — Initial EVM Integration

- Added native balance queries and native currency transfers.
- Added smart contract reads and writes.
- Added transaction approval flow.
- Added human-readable ABI utility.
- Added wallet management and encrypted backup/import foundation.
- Added exact string-based blockchain unit conversion.
- Added asynchronous operations and Unity main-thread callback dispatch.
- Added initial Unity package integration for Windows x64 and Android arm64-v8a.

## Unreal Engine Notes

- Unreal C++ wrappers, Blueprint nodes, delegates, Editor tools, and sample assets must be verified against the actual Unreal plugin.
- Unity-specific APIs, including Unity Project Settings and generated C# classes, are not Unreal APIs.
- Platform support in the Unity package does not automatically establish Unreal platform support.
- Record Unreal-specific API and feature changes in future release entries.

## Versioning Notes

- The `alpha` suffix identifies pre-release software.
- Shared native SDK capabilities and engine-specific functionality should be tracked separately where their implementations differ.
- Release notes should distinguish implemented and tested features from planned work.