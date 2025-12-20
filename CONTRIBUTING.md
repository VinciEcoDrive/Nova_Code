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

* **Automatic Build:** When you open a Pull Request, the server automatically attempts to compile your code.
* **Checks:** You will see a status at the bottom of your PR.
    * ✅ **Green Check:** The code compiles. It is safe to review and merge.
    * ❌ **Red X:** The build failed. You must fix the errors in your branch before merging.

**Do not merge a PR if the CI check has failed.**

## Versioning Strategy

We use **CalVer** (Calendar Versioning) to align with the competition year.

**Format:** `YY.MINOR.PATCH`
* `YY`: Competition Year (e.g., `26` for 2026).
* `MINOR`: Major feature additions.
* `PATCH`: Bug fixes.

## Releasing Firmware
To generate a "Production Ready" binary file (for example, before a race):

1. Update the version in `platformio.ini`.
2. Merge your code to main.
3. Tag the commit on GitHub or via CLI:
```bash
git tag v<YY>.<MINOR>.<PATCH>
git push origin v<YY>.<MINOR>.<PATCH>
```
  - e.g.
```bash
git tag v26.2.1
git push origin v26.2.1
```

1. The CI pipeline will detect the tag, compile the code, and name the file `firmware-v<MAJOR>.<MINOR>.<PATCH>.bin`. 
2. Download the binary from the GitHub Actions tab -> Artifacts

## Coding Standards 

- **Libraries**: Do not include libraries manually. Add them to lib_deps in platformio.ini. PlatformIO will handle the rest.
- **Formatting**: Try to keep indentation consistent (spaces vs tabs).