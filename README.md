
# Computer Systems Oribtal 



## Appendix




## Prerequisites
A list of software that must be installed before hand:

STM32CubeIDE:
```bash
https://www.st.com/en/development-tools/stm32cubeide.html#get-software
```
STM32 ST-LINK Utility:
```bash
https://www.st.com/en/development-tools/stsw-link004.html
```

## Contributing

Contributions are always welcome!

See `contributing.md` for ways to get started.

Please adhere to this project's `code of conduct`.


## Running Locally
### Create a personal access token
Go to
```bash
https://github.com/settings/tokens
```
Select 
```bash
Generate new token-> Generate new token(classic)
```
Under the "Select Scopes" check the boxes
```bash
repo
gist
```
### Install EGit
Setup EGit in STM32CubeIDE

Navigate to 
```bash
  Help->Eclipse Marketplace
```

In the "find:" search bar type

```bash
  EGit - Git Integration for Eclipse
```

Hit install and restart STM32CubeIDE when complete.
### Cloneing the repository
Right click on "Project Explorer" and navigate to
`Import->Git->Projects from Git->Clone URL`.
Enter in the "URL:" field
```bash
https://github.com/rbretmounet/CS-Ortibal.git
```
Enter in your github username under `user` and your github token under `password`. Click next until the project is imported.
### Setting up the project properties
The project properties must be configured so a `.bin` file is generated when the project is built. 
In order to so you must right click `orbital_r1` and select `properties` at the bottom of the list. A new window should open and you must navigate to `C/C++ Builds->Settings->MCU Post build outputs`. Tick the boxe that says `Convert to binary file (-O binary).


## Developing on the board
### Connecting the board
Connect the board to your computer's USB port and open up `STM32 ST-LINK Utility.exe`.
Navigate to `Target -> Connect` the device memory should popluate with addresses if successful.
### Flashing the board
Navigate to  `Target->Program`. The application with ask your for a file that ends in `.bin,.srec, or .hex`. If `STM32CubeIDE` is correctly configured [See how to configure](#Setting-up-the-project-properties). These files are generated in the projects `Debug` folder. 
Navigate to the project directory `~/orbital_r1/Debug/` and select the file that ends in the correct file format.