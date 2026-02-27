# Hollyhock V2 to V3 Migration Notes

## Build System
*   **Makefile**: Update toolchain to `sh4a_nofpueb-elf-gcc/g++`.
*   **Flags**: Use `-flto=auto`, `-ffat-lto-objects`, `-gdwarf-5`, `-O2`.
*   **Sources**: Ensure Makefile picks up sources correctly (e.g., recursive search or explicit list). Be careful with "unity builds" (files including other .cpp files); only compile the entry point file to avoid duplicate symbols.
*   **Linker**: Remove custom `linker.ld`. The SDK handles linking.

## Entry Point & Init
*   **main()**: Signature is `int main() { ... return 0; }`. Remove `extern "C"` if using C++.
*   **calcInit()**: Removed. Initialization is automatic.
*   **calcEnd()**: Removed. Cleanup is automatic.
*   **VRAM**: `width`, `height`, and `vram` pointer are compile-time constants in `<sdk/calc/calc.h>`. Do not redefine them. For PC ports, wrap custom definitions in `#ifdef PC`.

## Headers & API
*   **Headers**: Use C-style headers (e.g., `<sdk/os/file.h>` instead of `.hpp`).
*   **Prefixes**: APIs now use prefixes. `open` -> `File_Open`, `memset` -> `Mem_Memset`.
*   **Constants**: Constants are often enums. `OPEN_READ` -> `FILE_OPEN_READ`. Check SDK headers for exact names.
*   **Standard Lib**: Use `<string.h>`, `<stdlib.h>` instead of `<cstring>`, `<cstdlib>` if working with global namespace C functions. Avoid renaming standard macros like `try`/`catch`.

## File I/O
*   **Paths**: Paths are `const char_const16_t*` (UTF-16).
*   **Strings**: Use `char16_t` arrays and `u"path"` literals. Cast to `(const char_const16_t*)` when calling SDK functions.
*   **Alignment**: File path buffers passed to SDK functions (like `File_FindFirst`) MUST be 4-byte aligned on SuperH. Use `__attribute__((aligned(4)))`.
*   **Structs**: `File_FindInfo` uses scoped enums like `File_FindInfo::EntryTypeDirectory` (or just `EntryTypeDirectory` depending on SDK version/macros). Check scoping.

## Input
*   **Polling**: Use `GetInput` instead of `GetKey`.
*   **Struct**: `struct Input_Event` MUST be 4-byte aligned. Use `__attribute__((aligned(4)))`. Misalignment causes crashes (address error) at runtime.
*   **Keycodes**: Use `KEYCODE_*` enums.
*   **Touch**: Coordinates in `Input_Event` are `int32_t`. Cast to `uint32_t` if comparing with unsigned bounds to avoid compiler warnings.

## Troubleshooting
*   **Crashes**: If it crashes in an SDK function (e.g., `GetInput`, `File_Find...`), check ALIGNMENT of the structs/buffers passed to it.
*   **Linker Errors**: "Multiple definition of ..." usually means a conflict with SDK symbols (e.g., `fillScreen`). Rename your function or wrap in `#ifdef PC`.
*   **Compile Errors**: "Did you mean...?" implies the symbol exists but maybe signature mismatch or missing namespace. Check SDK headers.
*   **Warnings**: Suppress `warn_unused_result` by checking return value `if (func() < 0) {}` rather than `(void)func()`.
