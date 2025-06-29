# 🔥 Roguelike Prototype – Unreal Engine 5

This is a personal game development prototype built in **Unreal Engine 5** using **Blueprints and C++**. It features a procedural dungeon generator and a melee combat system designed to be modular and extensible.

---

## 🎮 Features

### 🧱 Procedural Dungeon Generation
- Seed-based random dungeon layouts
- Rooms are modular and connected logically
- Automatic door and wall placement
- Uses a customized version of [pcg](https://www.pcg-random.org/) for fast, repeatable RNG

### ⚔️ Melee Combat System (Still in Early Stages)
- Realtime combat using animation-timed `BoxTrace` detection
- Impact effects and sounds are spawned relative to precise `Hit.Location`
- Supports multiple enemy types with unique reactions (ToDo)
- One-hit-per-attack window logic

### 🔫 Weapon System
- Modular `WeaponBase` class
- Handles timing, effects, and logic for future melee/ranged weapons
- Easily extendable for new weapon types

### 👾 Enemy & Character Behavior
- Enemies react to damage with animations and sound
- Dissolve effect on death
- Character death animation and dissolve (ToDo)
- Fully Blueprint-driven with backend C++ support for core systems

---

## 🛠️ Tech Stack

- **Unreal Engine 5**
- **C++** & **Blueprints**
- [pcg](https://www.pcg-random.org/) (RNG)
- Animation Notifies
- Line/Box Trace systems
- Modular component-based architecture

---

## 🎥 Demo

> _Watch a short gameplay showcase video here:_  
> [📺 Watch on YouTube](#) *(link your video here)*

---

## 🚧 Next Steps

- [ ] Combo attack chains  
- [ ] Enemy AI (patrol, chase, attack)  
- [ ] Health bars & UI  
- [ ] Projectile/ranged weapon support  
- [ ] Save/load system

---

## 🧠 What I’m Practicing

- Scalable architecture in UE5  
- Clean separation of logic between character, weapon, and environment  
- Animation syncing with gameplay logic  
- Procedural content generation  
- Visual feedback & polish (VFX, audio)

---

## 📬 Contact

Feel free to reach out on [E-Mail](bgrakca@gmail.com) 
Feedback and collaboration ideas are always welcome!

---

## 📄 License

This project is currently for **educational and portfolio use only**.  
Not intended for commercial distribution.

