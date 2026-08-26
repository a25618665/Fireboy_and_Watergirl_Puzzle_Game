# Third-Party Notices and Asset Provenance

This file records provenance that can be verified from the repository and the
2023 university project report. It does not grant permission beyond the
applicable licenses.

## Project authorship

- The project README identifies Fu Lien and Yen-Te Liu as the project authors
  (2023).
- The course report attributes asset preprocessing, architecture design, and
  implementation to both team members. It also records audio production as a
  team task.
- These statements document project work, but they do not establish ownership
  or redistribution rights for every underlying image, audio file, font, name,
  or character design.

## Leistungsstarkes Game Framework

The application is based on the Leistungsstarkes Game Framework, as identified
in `Source/Core/game.rc` and the project README.

Twelve source files contain copyright notices for Woei-Kae Chen
(`<wkc@csie.ntut.edu.tw>`) and declare the GNU General Public License, version 2
or any later version (`GPL-2.0-or-later`):

- `Source/Core/Game.h`
- `Source/Core/MainFrm.cpp`
- `Source/Core/MainFrm.h`
- `Source/Core/game.cpp`
- `Source/Core/gameDoc.cpp`
- `Source/Core/gameDoc.h`
- `Source/Core/gameView.cpp`
- `Source/Core/gameView.h`
- `Source/Game/mygame.h`
- `Source/Library/audio.cpp`
- `Source/Library/audio.h`
- `Source/Library/gameutil.h`

The notices show copyright years 2002-2008 in most files and 2002-2012 in
`audio.cpp` and `gameView.cpp`. The original notices in those files remain the
authoritative record. The license named in the headers is available from the
[GNU Project](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).

No top-level license currently states how the project authors license their
separate original contributions. That question should be resolved with both
authors before a binary or a new repository-wide license is published.

## Original game reference

This is an educational replica inspired by *Fireboy & Watergirl: The Forest
Temple*. The project is not affiliated with the original game's publisher. The
repository does not contain a permission record for the original game's name,
characters, visual design, or other recognizable material.

## Asset inventory and evidence status

The repository contains 178 resource files totaling approximately 23.25 MiB:

| Location | Files | Contents | Provenance status |
|---|---:|---|---|
| `Resources/game_run/` | 147 | Gameplay bitmaps, 11 collision maps, and two checkpoint-data files | Unverified: the report describes preprocessing and level construction but does not map each source asset to a creator or license. |
| `Resources/sounds/` | 12 | Menu, level, completion, failure, death, button, collectible, door, jump, platform, and switch audio | Unverified: the report lists these filenames and records audio production, but supplies no per-file source or license. |
| `Resources/game_menu/` | 11 | Menu/interface bitmaps and four additional audio files | Unverified: no per-file source or license is recorded. |
| `Resources/font/` | 2 | `Cinzel[wght].ttf` and `OFL.txt` | Verified: Copyright 2020 The Cinzel Project Authors; licensed under the SIL Open Font License 1.1 and obtained from the official [Google Fonts repository](https://github.com/google/fonts/tree/main/ofl/cinzel). |
| `Resources/` root | 6 | MFC/UI icons, cursor, bitmaps, and `GAME.RC2` | Unverified: no per-file source or license is recorded. |

The inventory contains 143 BMP files, 11 MAP files, 11 MP3 files, 5 WAV files,
2 ICO files, 3 TXT files, 1 CUR file, 1 RC2 file, and 1 TTF file. Cinzel's
copyright notice and complete OFL license are included beside the font. No
license, credits file, source URL, purchase record, or written permission was
found for the remaining media assets. An educational or non-commercial
statement is not a substitute for provenance or a redistribution license.

## Binary-release decision

**Status: font cleared; hold public binary publication pending clearance of the
remaining media assets.**

A playable release must ship the external `Resources` directory beside the
executable, so publishing the binary would also redistribute the 177 files
listed above. The same unresolved provenance applies to copies already stored
in the public repository; this notice documents the gap but does not resolve
it.

Before publication, record one of the following for every asset group:

1. the original creator and written permission from all applicable authors;
2. an open-source or Creative Commons license and its required attribution;
3. a commercial license that explicitly permits redistribution; or
4. replacement with original or clearly licensed media.

For each cleared item, record the relative path, title, creator, source URL,
license, modifications, and the location of supporting evidence. If clearance
cannot be established, keep the executable private and present the engineering
work through source code and documentation that do not redistribute the
uncleared assets.
