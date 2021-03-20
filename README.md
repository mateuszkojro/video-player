# VIDEO PLAYER

This is a project for computer graphics class 

# How to build for development

1. Clone using
```bash
git clone --recurse-submodules https://github.com/mateuszkojro/video_player
```
3. Create a `build` folder then configure using CMake 
```bash
   cd build && cmake ..
```
3. Make using your favourite build tool for example
```bash
make
```
4. There is a issue with CMake config so you need to copy `./build/SDL/SDL2d.dll` the `./build/` folder.
