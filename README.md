# BRED-EDITOR

![The banner](cia/banner_upscaled.png)

## What is it?
### The **bred-editor** (standing for "*blue/red save editor*") is a simple save editor for Pokémon Red/Blue on the 3DS, it's still W.I.P but many features are planned to be added later.

The current version is the `beta0.3.0`, having the following features :

    - Reading information about the save (including Player/Rival name, playtime, money, playerID, checksum, badges, etc....)
    - Reading many stats about the Pokemons in the party
    - Editing information such as the Player/Rival names, the money amount, the playtime, etc...
    - Properly saving the file and fixing the checksum
    - Viewing Pokémon Team stats in detail
    - Changing obtained badges
    - A GameBoy emulator **embed into the app** to test changes
    - + A few bugfixes


I fixed many bugs and every feature should work, but it's not perfect and there is probably still bugs so please keep a backup of your savefile before using the tool. 
New features will be added in the future versions.

## Usage :
Put your savefile in `SD:/3ds/bedit/SAVE.sav`

Install either the CIA of 3DSX executable and run it, then the menu is pretty user-friendly so you can just modify the data as you want. 
Finally you can press SELECT on the main menu to **write all changes to the file**.

## About the emulator
This save editor have an embed emulator made from [deltabeard's gameboy-c](https://github.com/deltabeard/gameboy-c/) for quick testing, for obvious reasons the ROM file isn't included so you need to place a Pokémon Red/Blue ROM file at `SD:/3ds/bedit/ROM.gb` (versions of any language should work).

## Screenshots
![Main Menu](pictures/mainmenu.bmp)
---------------------------------
![Save Information Menu](pictures/saveinfo.bmp)
---------------------------------
![Gameboy emulator](pictures/gb_emu.bmp)
---------------------------------
![Pokemon Stats Menu](pictures/pokestats.bmp)
---------------------------------
![Badges Editing Menu](pictures/badgesmenu.bmp)

## Yellow compatibility?
This save editor is made for the Red/Blue versions, but many offsets for data are the same on Yellow, so it may work too, i haven't tested but if you want to give it a try, go for it!

## Future updates
I'm still working on the project and in the future i will add the following features :

- Editing data such as PlayerID, Bag Items, and various flags
- Editing the data of the Pokémons themselves (that will be funny mmhhhh)
- Making a system to save/load multiple savefiles
- More features when i will have more ideas

###### Fun fact: the program is called **bred-editor** because `bred` is a mix between blue and red, but i thought it sounded like "bread" so i decided to use bread for the icon/banner theme :3
--------------------------------------------
Thanks to [DevKitPro](https://devkitpro.org/wiki/Getting_Started) for the toolchain and some code, [this GBATemp thread](https://gbatemp.net/threads/cxitool-convert-3dsx-to-cia-directly.440385/) for the tools to make the CIA app, the [libctru examples](https://github.com/devkitPro/3ds-examples) for the uses of some functions, [gameboy-c](https://github.com/deltabeard/gameboy-c/) for the embed emulator code, and [Bulbapedia](https://bulbapedia.bulbagarden.net/wiki/Save_data_structure_(Generation_I)) for the savedata structure.

License GPLv3. By [AtexBg](https://github.com/AtexBg). May 9 2026.
