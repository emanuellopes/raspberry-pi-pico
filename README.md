# raspberry-pi-pico

# 1. Pico SDK

- Download Pico SDK using git

```bash
git clone git@github.com:raspberrypi/pico-sdk.git --recurse-submodules
```

- Set the variable in .bashrc or .zshrc, it depends of if you are using zsh or bash
```bash
export PICO_SDK_PATH="$HOME/Documents/raspberry_pi_pico_w/pico-sdk"
```

For Mac OS install the following libraries to compile the program

```bash
brew install cmake
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```

# 2. Configure the visual studio
- Install the cmake tools extension https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools
- Install C/C++ extension https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools
- Install C/C++ Extension Pack extension https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack

# 3. Note
Open the each project in a separated vscode window to make sure vscode is grabbing the correct settings.