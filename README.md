
# Computer Systems Oribtal 
Welcome to code respository for the Space And Systems Satellite Club's REALOP 1 Flight Software! The code here will be run on our flight computer, the Orbital Platform, which will be responsible for controlling and monitoring the CubeSat's various subsystems. Details regarding the Orbital Platform are available on this github repository (FILL THIS WITH ELEC's REPO).

**NOTE**: Several images of the board or connectors aren't attached yet since the Orbital Platform is back in Davis. Wait a few moonths and the images will be here.

## REALOP 1

Lorem Ipsum for now...

## Getting Started
To get started with our code, you'll need to have a basic understanding of C programming and the tools we're using. You'll need to follow the instructions listed in the following sections:
- [Prerequisites & Requirements](#Prerequisites-&-Requirements)
- [EGit Tutorial](#EGit-Tutorial)
- [Developing on the board](#Developing-on-the-board)


## Prerequisites & Requirements

A list of software that must be installed before hand:

### [GNU ARM Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)
The Toolchain can be installed in any folder of your liking, as long as it the 

### [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html#get-software)

#### Installer
Navigate to [WEBSITE], scroll to the very bottom, and click `Get Latest` for whichever Operating System your computer runs. A license agreement form will pop up. Click `Accept`, and you'll be prompted to enter an email. Once that is done, check that mail's inbox. There should be an email by ST. Open it and click on the `Download now` button. You should be taken back to the same installation website, except this time the browser will prompt you to download the installer (or automatically download depending on the browser settings).

#### Installation
Follow this guide by ST : [Cube IDE Installation Guide](https://www.st.com/resource/en/user_manual/dm00603964-stm32cubeide-installation-guide-stmicroelectronics.pdf)
#### Starting CubeIDE
When you open CubeIDE, it'll prompty you to slect a workspace. To keep things seperate, it is recommended to make a folder somewhere on your computer just for the REALOP FSW. Then use that as the workspace. An example:
IMAGE
Upon clicking `Launch`, the IDE will open

#### EGit
Before setting up anything else, first get Git integration in the CubeIDE. Follow this tutorial : [EGit]([https://shadyelectronics.com/how-to-use-github-with-stm32cubeide/](https://eclipsesource.com/blogs/tutorials/egit-tutorial/) (follow the instructions under **Cloning Repositories** to clone this repository on your system).
**NOTE**: You'll need both a *Github account* and a *Github Personal Access Token* (the token gets used whenver you are prompted to enter your githuub user password). **Make sure to save the Access Token, it's only shown to you once**.

#### Setting up the project properties
The project properties must be configured so a `.bin` file is generated when the project is built. 
In order to do so you must right click the master folder in the workspace and select `properties` at the bottom of the list.
IMAGE
A new window should open and you must navigate to `C/C++ Builds->Settings->MCU Post build outputs`. Tick the boxe that says `Convert to binary file (-O binary).


## Developing on the board

### Physically connecting to the board
We use ST-LINK V2 to connect to the board. The ST-LINK USB Connector already has wires attached to it, with labels on each wire.
IMAGE
The Orbital Platform has a connector port on it's topside, with labels that correspond to the wires on the ST-LINK Connector. 
IMAGE
Connect the Port and the Connector to each other by matching the labels.
If the compuer is powered on, the Platform should start getting power, and start running whatever program is loaded on it. Since the `Heartbeat` LED blinks every second, it's blinking will be a clear indication of things working.
IMAGE


### Flashing the Platform
With ST-LINK V2, fashing can be done right through the CUBE IDE. Before flashing, make sure the Orbital Platform is connected. Then make sure the REALOP Project is open in the IDE. Flashing can be done by pressing the `Run` in the top pane of the IDE.
IMAGE
This will first rebuild the project
IMAGE
and then try connecting to the Platform
IMAGE
Once both are done, it will flash the program onto the Platform
NOTE: Once this is completed, the Platform will start running the code immediately.

### Debugging the Platform
In order to debug, make sure to have a breakpoint at some line in your code (ideally close to whichever component you think is failing). Then, make sure the Platform is physically connected. Press the `Debug` icon on the top pane. There will be a prompt to enter the `Debugger Layout`. Press `Yes`.
IMAGE
The debugger will open up
IMAGE
The project will be built and flashed onto the Platform, and the IDE will have the normal tools found on most debuggers (such as single stepping, stepping over, watching expressions, looking at registers, etc), and will stop execution once it hits a breakpoint).
NOTE: The Startup file might open whenver the program starts. This is normal behavior.


## Resources
- [EGit Tutorial](https://eclipsesource.com/blogs/tutorials/egit-tutorial/)
- [DoxyGen](https://www.doxygen.nl/manual/index.html)
- [API Doc](https://rbretmounet.github.io/CS-Ortibal/DoxyGen/html/index.html)

STM32L476 vendor documentation 
- [Webpage](https://www.st.com/en/microcontrollers-microprocessors/stm32l476zg.html) - software support and complete information
- [Datasheet](https://www.st.com/resource/en/datasheet/stm32l476zg.pdf) - overview of features and electrical specifications 
- [Reference manual](https://www.st.com/resource/en/reference_manual/rm0351-stm32l47xxx-stm32l48xxx-stm32l49xxx-and-stm32l4axxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) - details of every peripheral and subsystem 
- [Programming manual](https://www.st.com/resource/en/programming_manual/pm0214-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf) - details of microarchitecture, not necessary unless


## Contributing
Contributions are always welcome!
See `contributing.md` for ways to get started.
Please adhere to this project's `code of conduct`.

