# Lab Lockdown

A Windows first-person game built in C++17 and OpenGL. Survive waves of robots in a lab and use health and ammo pickups to stay in the fight.

Between waves, use the ten-second break to choose health (H) or ammo (J). The heavy drone takes double damage during its blue recovery. In wave two, hold F at the central relay for three seconds. Wave three requires arming the central relay and holding F at the northwest relay within 35 seconds. Damage and leaving range interrupt a hold. An expired link can be restarted. Clear the final drones and return to the starting area, then press F to extract. The results screen shows your final score and mission time.

Drone attacks are preceded by a rising tone and red glow. Damage is biased toward the side of the attacker. Wave two adds fast pursuers and wave three introduces a heavy drone. Look for green health packs and amber ammo packs. Press Enter on the results screen to retry.

In the pause menu, click buttons or drag the sensitivity and volume sliders. Click the damage effect toggle to change it. Arrow keys and Enter also work. Settings apply for the current run.

## Build and run

1. Install Visual Studio with Desktop development with C++, the MSVC v143 build tools and a Windows 10 or 11 SDK.
2. Open `FPS-Game.sln`, select **Release | x64** and build the solution.
3. Run from Visual Studio or open `x64/Release/FPS-Game.exe`. For command-line launches, start it from the executable's directory.

The build copies the runtime DLLs and resources to the output directory. The game requires an OpenGL 3.3-capable graphics driver.

## Controls

| Input | Action |
| --- | --- |
| Enter | Start |
| WASD | Run |
| Either Shift key | Walk while held |
| Mouse | Look |
| Space | Jump |
| Left mouse button | Fire or slash with the selected weapon |
| 1 / 2 / 3 | Gun / Knife / Empty hands |
| Mouse wheel | Cycle equipment |
| R | Reload |
| E | Toggle flashlight |
| Esc | Pause or resume during a mission |

Game source is in `ProjectFPS/src`, assets are in `ProjectFPS/resources`, and the bundled third-party dependencies are in `libraries`.
