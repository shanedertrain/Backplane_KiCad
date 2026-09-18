# Backplane KiCad

- This is a personal fork (`shanedertrain/Backplane_KiCad`) of upstream `i2cjak/Backplane_KiCad`. Commits here use the fork owner's own git identity, never upstream's. Before pushing, verify the destination is this fork, not upstream.
- Base changes on stable KiCad releases. Keep IPC backports focused, record their upstream revisions, and verify both schematic and PCB behavior. Never label an untested backport a working runtime.
- Preserve file compatibility with unmodified KiCad 10.0.6. Do not raise native file-format versions or emit newer unsupported tokens. Store fork-only metadata in adjacent `.backplane.json` companions, preserve those companions through save/copy/export staging, and verify native save/reopen with stock 10.0.6 before releasing.
- This repository and corresponding source for distributed binaries must remain public. Preserve upstream licenses, authorship, and third-party notices. Ship licenses and matching source with release binaries.
- Coordinate the runtime archive layout and supported platforms with the sibling Backplane app. Validate the packaged runtime on a host without a separate KiCad installation.
- Never add OpenAI copyright or ownership notices.
