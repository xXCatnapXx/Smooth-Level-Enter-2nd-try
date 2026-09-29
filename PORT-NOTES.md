# Smooth Level Enter - GD 2.2074 port (unofficial)

Original mod: https://github.com/undefined06855/Smooth-Level-Enter (v1.0.9, GD 2.2081, Geode 5.6.1)
All credit for the mod goes to undefined0.

## What changed vs. upstream
1. mod.json: geode 5.6.1 -> 4.10.2, gd (all platforms) 2.2081 -> 2.2074, geode.node-ids v1.23.3 -> v1.21.0
2. src/CCTransitionPlayLayer.cpp: geode::utils::random::generate(0, 4) does not exist in Geode 4.x,
   replaced with a small std::mt19937 helper (same range: 0..3)
3. CMakeLists.txt: C++23 -> C++20 (what Geode 4.x is built with; the code uses no C++23-only features)

## Build (Windows)
    geode sdk update v4.10.2
    geode sdk install-binaries
    geode build
The .geode file ends up in build/. Copy it to %LOCALAPPDATA%\GeometryDash\geode\mods\

NOTE: not compiled or tested by the person who wrote this port. If the build shows errors,
send them over and they can be fixed.
