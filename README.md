# CyberCrawl

Clone of original Rogue v3.6.

Main goal is to produce a roguelike with classic cyberpunk-influenced setting, primarily based on "Ghost in the Shell" by Masamune Shirow, Sprawl trilogy by Willian Gibson, and Deus Ex game by Ion Software.

As for now, original fantasy setting is fixed, using name conversions (like guns instead of rods etc.) to look like it takes place in cyberpunk universe. No new features was added, only original features are present.

Next goals:

* Add unit tests and refactor.
* Make modular structure and add SDL2 rendering.
* Make Android version using NDK.
* Convert to C++.

## Installation

Just go to main dir and run `make`. Produced `rogue` file is the only game file.
Savefile is stored at `~.local/share/cybercrawl/rogue.sav`. If savefile is present in home dir `~/.rogue.sav`, it will be loaded instead.
