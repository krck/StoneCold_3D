StoneCold (2D Game and Engine)
==============================

# Table of Contents  
1. [About](#about)  
2. [Game Design Document](#gdd)  
3. [Technical Description](#technical)  
4. [Copyright and Attribution Notices](#copyr)
5. [Workitems](#work)


<a name="about"/>

# 1. About

This game is a combination of my (quite old) attempt to write a 3D Graphics Engine [arealGL](https://github.com/krck/arealGL) and my (not that old) attempt to write a 2D Game [StoneCold](https://github.com/krck/StoneCold).

<a name="gdd"/>

# 2. Game Design Document

### 2.1 Project Description
 
StoneCold will be a 3D, top-down, "tile-based" RPG and comes with its own OpenGL based Engine. The concept is heavily inspired by Diablo (Gameplay aspects), Action-RPGs like Dark Souls (Gameplay aspects), Nuclear Throne (procedural map generation) and many more.

### 2.2 Story, Characters and Theme

The main character is a dwarf called Henry and lives in the small village Boulder, by the Mountain. Many years ago, a strange comet impacted nearby and shattered its core all over the dwarven kingdom. A special metal was found in those fragments, mined and forged by the dwarves. This special metal has great abilities once forged, but all the raw fragments that are still out there, not found for many years, started corrupting the land around them and spawning strange creatures that now threaten the dwarven kingdom.

### 2.3 Progression

The player starts out in a town-hub, to which he can return to at any moment and progresses through a series of zones (grasslands, highlands, desert, arctic, castles, ...). Each zone has a bunch of levels and ends in a boss fight. Each level of a zone needs a specific quest/task to be completed, before moving on to the next level (find all X, kill all Y, ...). Once all zones are traversed Henry reaches the biggest of the raw comet-fragments and fights the final boss, to cleanse the lands for good.

After that an "endless-mode" will be unlocked in town, to level/quest/grind further.

### 2.4 Gameplay and User Skills

- Keyboard / Controller input
- Navigating different map types and dungeons
- Tactical action combat, learning AI behavior and terrain to maximize the chances of survival
- Stat and skill management - improve your chances of survival for different threats
- Long and short-term resource management
- Ideally there is enough depth for actuals "builds" and "combos"

### 2.5 Game Mechanics

- Items and Powerups
- Special Moves (on cooldown)
- Healing potion (on cooldown)
- ...

### 2.6 Progression, Challenge and Losing

- Defeat enemies, get XP, Level up
- Each level allows to advance a specific skill or stat
- Special "corruptions" (More life, damage, def, crit, ...) that will be inflicted
- Gather corrupted Stones (currency) to modify/reforge special abilites and "corruption" states
- Enemy difficulty increases slow during "story" and fast during "endless" mode
- ...
If all health is lost, the level will be reset and has to be started from the beginning.
All Zone and Level progression will be lost, but "corruptions" and skills/stats will stay.

### 2.7 Art Style and Music

3D, but still Pixel-Art-ish using the dawnbringer color palette. Most object in game will be "low-poly" with pixelated Textures. Some will be simple Blender creations done by myself but most of the characters, enemies, etc. will be most likley be open source artworks by various artists (see below).

<a name="technical"/>

# 3. Technical Description

### 3.1 Language and libraries

- C++ 17 (Compiler: MSVC 14.24, IDE: VS2019)
- [GLFW](https://github.com/glfw/glfw) : Window and IO manager
- [GLEW](http://glew.sourceforge.net/): OpenGL extension library
- [Assimp](https://github.com/assimp/assimp) : Model loader
- [glm](https://github.com/g-truc/glm) :  Mathematics library (until the custom stuff is working)
- [STB lib](https://github.com/nothings/stb) : File “stb_image.h” to load texture and image files
- All the SDL/SDL_image/SDL_ttf dependencies

### 3.2 Architecture and Code Structure

The code is written in modern C++ and in a object oriented manner. Simplicity (or rather: avoiding complexity) is a main goal. Well. Fingers crossed. At the highes level the structure is as follows:
- <b>StoneCold.Base:</b> Basic Settings, common Types and Enums, Math and Exception classes
- <b>StoneCold.Engine:</b>OpenGL Engine, Entity-Component-System, Game-State-Management and Event handling
- <b>StoneCold.Game:</b> Window creation, Levels/NPCs/Menu Setup, Manager init and main loop
- <b>StoneCold.Resources:</b> Resource management with Lifetime checks (loading and unloading)
- <b>StoneCold.Tests:</b> Unit Tests

<a name="copyr"/>

# 4. Copyright and Attribution Notices

Some of the Textures are hand drawn (thanks MS Paint and Gimp) but all the nice looking ones are created by actual Artists:
- DawnBringer Color-Palette by Richard Fhager [link to his pixeljoint](http://pixeljoint.com/p/23821.htm)
- Dwarf Sprites by Elthen (Ahmet Avci) [link to his patreon](https://www.patreon.com/elthen)
- Skeleton Sprites by Jesse Munguia [link to his homepage](https://jessemunguia.com/)
- Mountain Background by NotTandy [link to his pixelart](https://www.pixilart.com/nottandy)
- UI Textures (RPG Pack) by Kenney Vleugels [link to his art](https://www.kenney.nl)
- Font Crom by russh (Russell Herschler) [link to his homepage](http://www.dragonfang.com/)
- Font OldeDethek by Pixel Sagas [link to the homepage](http://www.pixelsagas.com/)

<a name="work"/>

# 5. Workitems

### 5.1 Current tasks

- [ ] ...

### 5.2 Backlog, cleanup and housekeeping

- [ ] ...

### 5.3 Known Bugs

- [ ] ...

### 5.4 Future features

- Add different Map-Generation algorithms (Rooms/Indoor, Caves)
- Add Portals and automatic Level-Loading (maybe Loading-Screen or transition effect)
- Add Player GUI (Lifebar, Action-Bar, Skill/Stat Window)
- Add Pickups and Buffs (Gold, Items, Chests)
- Add Controller Support (with automatic check if Controller is available)
- Add a basic Options Menu and Save/Load functionality 
- Add Sound and Music
