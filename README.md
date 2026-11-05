# Interstellar 3D Simulation

A real-time, interactive 3D computer graphics simulation built in C++ and OpenGL/GLUT, inspired by the physics, cinematography, and multi-dimensional concepts of Christopher Nolan's film *Interstellar*. 

This project explores complex rendering techniques including matrix stack hierarchies, procedural generation, dynamic billboarding, and non-Euclidean environmental looping to recreate the *Endurance* spacecraft, the Gargantua black hole, and the 4th-dimensional Tesseract.

## 🌌 Project Phases & Features

### Phase 1: Deep Space & Gargantua
A classic 3rd-person space flight simulator featuring decoupled Newtonian-style physics and dynamic camera tracking.
* **Dual-Ship Architecture:** Fly the massive, sluggish *Endurance* mother-ship, or decouple and pilot the highly agile *Ranger* scout ship.
* **Rotational Steering:** Video-game style thrust and steering mechanics. The camera acts as a drone, allowing the player to orbit their view while flying.
* **Gargantua Black Hole:** A colossal, scaled background entity utilizing mathematical billboarding so the accretion disk and vertical halos perfectly track the player's camera angle at all times.
* **Procedural Galaxy:** The deep background consists of algorithmically generated spiral galaxies and scaled stars, mapped 20,000 units deep into the Z-buffer.

### Phase 2: The Tesseract
Flying into the Black Hole's event horizon triggers a seamless transition into a 4th-dimensional library.
* **Non-Euclidean Space:** The environment utilizes a mathematical "Pac-Man" wrap-around trick. Flying out of one bookshelf tunnel instantly teleports the ship to the opposite side of the room, creating an inescapable, infinite 3D labyrinth.
* **Photorealistic Architecture:** Built using a 7-node XYZ matrix that generates mathematically perfect cubic intersections.
* **"Streaks of Time" Rendering:** Utilizes UV Texture Clamping to sample a tiny 1% sliver of a bookshelf texture and algorithmically stretch it across 140-unit struts, perfectly mimicking the movie's extruded light effect.
* **Infinite Abyss:** Employs OpenGL Exponential Fog (`GL_EXP2`) to blend the outer bounds of the grid into pure blackness.

## 🎮 Controls

| Key | Action |
| :--- | :--- |
| **W / S** | Main Thrusters (Forward / Backward) |
| **A / D** | Rotational Steering (Yaw Left / Right) |
| **Spacebar** | Vertical Hover Thrust (Up) |
| **C** | Vertical Hover Thrust (Down) |
| **F** | Toggle Docking (Eject/Attach the *Ranger* from the *Endurance*) |
| **R** | Toggle *Endurance* Ring Spin Animation |
| **Mouse** | Orbit Camera (Look around the ship) |
| **ESC** | Exit Simulation |

## 🛠️ Technical Architecture

The project is structured using a modular Object-Oriented design to separate logic, rendering, and state management:

* `main.cpp`: Handles GLUT window management, input polling, and the state-machine transition between Phase 1 and Phase 2.
* `Spaceship.cpp/h`: Manages the physics engine, thrust vectors, collision limits, hierarchical rendering (`glPushMatrix`/`glPopMatrix`) of the nested ship modules, and the docking state.
* `Camera.cpp/h`: Calculates dynamic 3rd-person orbital math and relative angles for 2D billboarding.
* `BlackHole.cpp/h`: Renders the accretion disk using complex trigonometric vertex generation.
* `Environment.cpp/h`: Handles the algorithmic generation and additive blending (`GL_SRC_ALPHA`, `GL_ONE`) of the procedural background.
* `Tesseract.cpp/h`: Generates the infinite lattice, applying UV clamping and unlit texture mapping.
* `TextureLoader.h`: Utilizes `stb_image.h` to load external `.jpg` assets into OpenGL.

## 🚀 Build Instructions

### Prerequisites
* C++ Compiler (MSVC / GCC)
* OpenGL & GLUT (FreeGLUT recommended)
* `stb_image.h` (Included for texture loading)

### Setup
1. Clone the repository.
2. Ensure your linker settings include `opengl32.lib`, `glu32.lib`, and `glut32.lib` (or your OS equivalents).
3. Place `solar.jpg`, `metal.jpg`, `texture_1.jpg`, and `bookshelf.jpg` in the root execution directory.
4. Build and run.
