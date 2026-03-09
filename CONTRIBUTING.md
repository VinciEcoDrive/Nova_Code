# Contributing to Nova_Code

Thank you for contributing to the Efficiency Prototype code! Please follow these guidelines to ensure our code remains stable and match-ready.

## Development Workflow

We use a standard Feature Branch workflow.

1.  **Branch:** Create a new branch for your task.
    * `git checkout -b feature/add-gps-sensor`
    * `git checkout -b fix/motor-pid`
2.  **Code:** Make your changes.
3.  **Commit:** Meaningful commit messages are appreciated.
4.  **Push:** Push your branch to GitHub.
5.  **Pull Request (PR):** Open a PR targeting the `main` branch.

### Continuous Integration (CI)

We use **GitHub Actions** to protect the main branch.

* **Automatic Build:** CI runs on every Pull Request and every push to `main`/`master`.
* **Checks:** You will see a status at the bottom of your PR.
    * ✅ **Green Check:** The code compiles. It is safe to review and merge.
    * ❌ **Red X:** The build failed. You must fix the errors in your branch before merging.

**Do not merge a PR if the CI check has failed.**

### Release Pipeline

Releases are intentionally separate from CI:

* A release is created **only** when a tag matching `v*` is pushed (example: `v26.2.1`).
* The release workflow builds firmware and publishes a `.bin` file as a GitHub Release asset.
* Regular PRs and merges never create a GitHub Release.

## Versioning Strategy

We use **CalVer** (Calendar Versioning) to align with the competition year.

**Format:** `YY.MINOR.PATCH`
* `YY`: Competition Year (e.g., `26` for 2026).
* `MINOR`: Major feature additions.
* `PATCH`: Bug fixes.

## Releasing Firmware
To generate a "Production Ready" binary file (for example, before a race):

1. Merge your code to `main`.
2. Tag the target commit on GitHub or via CLI:
```bash
git tag v<YY>.<MINOR>.<PATCH>
git push origin v<YY>.<MINOR>.<PATCH>
```
  - e.g.
```bash
git tag v26.2.1
git push origin v26.2.1
```

3. The release workflow detects the tag, builds the firmware, and publishes `firmware-v<YY>.<MINOR>.<PATCH>.bin`.
4. Download the binary from the GitHub **Releases** page.

## Coding Standards 

- **Libraries**: Do not include libraries manually. Add them to lib_deps in platformio.ini. PlatformIO will handle the rest.
- **Formatting**: Try to keep indentation consistent (spaces vs tabs).