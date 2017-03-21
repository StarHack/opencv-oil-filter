# OpenCV Oil Filter + Text Masking
#### Powered By MuetzenJunge (m.b.H)

[![N|Solid](https://github.com/kpwn/yalu102/raw/master/yalu102/Assets.xcassets/AppIcon.appiconset/AppIcon60x60@3x.png?raw=true)](https://nodesource.com/products/nsolid)

# Description
This code snipped demonstrates how to use OpenCV to generate an oil filter effect. It will perform the steps described below:

  - Mask a text to exclude from oil filter
  - Apply oil filter on background image
  - Combine text matching the mask and filtered background image

# Preparation on macOS

The steps below describe installation of the **opencv** library and **pkg-config** using the package manager [homebrew]

```sh
$ brew tap homebrew/science
$ brew install opencv
$ brew install pkg-config
```

# Compiling on macOS

We're going to use **LLVM** for compilation. Alternatively, you might as well use **g++** or a compiler of your liking.

```sh
$ llvm-g++ $(pkg-config --cflags --libs opencv) main.cpp -o main
```

# Running

```sh
$ ./main
```

[homebrew]: <https://brew.sh>