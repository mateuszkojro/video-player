# Setup

Project is CMake based so the only think that is neded to change is directory containing Qt libraries

You need to setup the path to Qt and OpenCV

```CMake
# For Qt
set(CMAKE_PREFIX_PATH "C://Qt//5.15.2//mingw81_64//")
# For opencv
set(OpenCV_DIR "C:\\opencv\\install")
```

This program contains Lua 5.3.6

<https://www.lua.org/>

Cmake for Lua sourced from

<https://gist.github.com/squeek502/0706332efb18edd0601a4074762b0b9a>

## Engine

- [ ] pause / resume
- [ ] skip 10s next / prev
- [ ] save to file
- [ ] 

## UI

- [ ] pause, resume, skip UI
- [ ] Additional settings for effects
- [ ] unify UI so that input can be redirected at any time 

