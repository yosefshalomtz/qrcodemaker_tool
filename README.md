
# qrcodemaker_tool

A simple Qt-based QR code maker GUI built in C++ using the Project Nayuki QR code generator library.

This repository contains a small application that renders QR codes as SVG and displays them in a window. It uses Qt (core/gui/widgets/svg) and the included `qrcodegen` C++ implementation.

## Key features

- Live QR preview: enter text and see the QR code update in real time.
- Bit style toggle: rectangle or circle rendering of QR modules.
- Error correction level selector (HIGH, QUARTILE, MEDIUM, LOW).
- Border size selector.
- Renders QR as SVG via `QSvgRenderer` (displayed in the app).

## Files of interest

- `qrcodemaker.cpp` — Main Qt GUI application: menu, controls, and SVG preview widget.
- `qrcodegen.cpp` / `qrcodegen.hpp` — QR code generator library (Project Nayuki, MIT). This library creates the QR data and SVG output.
- `resources.qrc` — Qt resource file (embeds `icon.png`).
- `win_rc.rc`, `icon.ico`, `icon.png` — Windows resources & icons.
- `qttest.pro` — Qt qmake project file (build configuration).
- `compile.bat` — Convenience build script (Windows / cmd) which runs qmake and build/deploy steps.
- `release/` — Prebuilt `qrcodemaker.exe` and required DLLs (packaged application).

## Quick start — Windows (cmd.exe)

The project is configured with `qmake` and a `compile.bat` helper that performs the typical steps used here. The `compile.bat` included in the repo runs the following sequence:

1. `qmake`
2. `make release`
3. `cd release` and `windeployqt .`
4. cleanup of intermediate files

To build and run manually in a Windows cmd shell (assumes Qt 6 is installed and `qmake` & `windeployqt` are on PATH):

```bat
qmake
make release
cd release
windeployqt .
cd ..
del Makefile.Release Makefile.Debug Makefile .qmake.stash
rmdir /S /Q debug
```

Notes and common alternatives:
- If you use the Qt command prompt provided by the Qt installer, `qmake` and `windeployqt` are already in PATH.
- The `make` command assumes an environment with a Unix-like make (MinGW/msys). If you build with MSVC, use the appropriate generator (for example, use `nmake` or generate a Visual Studio solution instead).
- After a successful build, the produced executable is `release\qrcodemaker.exe`.

## Running the application

Open `release\qrcodemaker.exe` (or run the built binary from your build directory). The window contains:

- A text input at the top — type the text or URL to encode into the QR code (default is the repository URL).
- The QR preview in the center (rendered from SVG data produced by the `qrcodegen` library).
- Menu (Options) where you can toggle module style (rectangles/circles), change ECC level, and adjust border size.

There is no dedicated export/save button in the current UI; the app renders SVG internally using `QSvgRenderer`. To capture an SVG output you can extend the code to write `QrCode::toSvgString()` output to a file, or take a screenshot of the preview as a quick workaround.

## Development notes

- The QR generator code (`qrcodegen.cpp`/`.hpp`) is taken from Project Nayuki and is distributed under the MIT License (see Credits & License below).
- The GUI is implemented in modern C++ (C++17) and Qt widgets; the project file (`qttest.pro`) sets `CONFIG += c++17` and links `core gui widgets svg`.

If you want to add a save/export feature, a simple change is to call `qrcodegen::QrCode::toSvgString()` with the current parameters and write the result to a `.svg` file.

## Troubleshooting

- `qmake` not found: open a Qt command prompt or add the Qt bin folder to PATH.
- `make` fails on Windows: ensure you have a compatible build tool (MinGW `make` or use Visual Studio toolchain with the matching qmake/kit).
- Missing runtime DLLs when running `release\qrcodemaker.exe`: run `windeployqt` on the executable folder to collect required Qt DLLs (the `compile.bat` already calls `windeployqt`).

## License & credits

- The project code that generates QR codes (`qrcodegen.cpp` / `qrcodegen.hpp`) is from Project Nayuki and is licensed under the MIT License. The original project page: https://www.nayuki.io/page/qr-code-generator-library
- The rest of this repository (GUI, resources, and build files) should be considered under the repository owner's preferred license — check `LICENCE` in the repo root for the project's chosen license.

## Attribution

- Author / repository owner: `yosefshalomtz` (based on the repo layout and default QR path).
- QR library: Project Nayuki — MIT.

## Next steps (suggested)

- Add a small "Save SVG" menu action that writes `QrCode::toSvgString()` to disk.
- Add a small automated build script for MSVC users (msbuild or Visual Studio solution) if you want official support for multiple toolchains.
- Add unit tests around `qrcodegen` usage if you plan to modify the generator.

---

Requirements coverage:

- Create an automatic README.md based on project files: Done — this file was generated from `qrcodemaker.cpp`, `qrcodegen.*`, `qttest.pro`, `resources.qrc`, and `compile.bat`.



--- this file created by chatGPT-5 mini :) ---