
# Computer Systems Oribtal 
Welcome to the GitHub repository for the Space And Systems Satellite Club's Computer Systems Team! Our goal is to design and build a CubeSat that will be launched into space. This repository contains the code for our team's computer systems, which will be responsible for controlling and monitoring the CubeSat's various subsystems.

## Getting Started
To get started with our code, you'll need to have a basic understanding of programming and the tools we're using. We're primarily using C++ for our code. You'll also need to follow the instructions listed in the following sections:
- [Prerequisites & Requirements](#Prerequisites & Requirements)
- [EGit Tutorial](#EGit Tutorial)
- [Developing on the board](#Developing-on-the-board)




## Prerequisites & Requirements

A list of software that must be installed before hand:

### [GNU ARM Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)
The Toolchain can be installed in any folder of your liking, as long as it the 

### [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html#get-software)

#### EGit


## EGit Tutorial
### Create a personal access token
A personal access token is need for EGit authorization.
To start go to
```bash
https://github.com/settings/tokens
```
Once the webpage has loaded select`Generate new token-> Generate new token(classic)`

Under the "Select Scopes" check the boxes `repo` and `gist` and click the `Generate token` button located at the bottom. You will be given a randomly generated token **MAKE SURE TO SAVE THE KEY.**
### Install EGit
EGit is a marketplace plugin that allows git integration into the STM32CubeIDE.

To setup EGit in STM32CubeIDE

Navigate to 
```bash
  Help->Eclipse Marketplace
```

In the "find:" search bar type

```bash
  EGit - Git Integration for Eclipse
```

Hit install and restart STM32CubeIDE when complete.
### Cloning the repository
Right click on "Project Explorer" and navigate to
`Import->Git->Projects from Git->Clone URL`.
Enter in the "URL:" field
```bash
https://github.com/rbretmounet/CS-Ortibal.git
```
Enter in your github username under `user` and your github token under `password`. Click next until the project is imported.
### Setting up the project properties
The project properties must be configured so a `.bin` file is generated when the project is built. 
In order to do so you must right click `orbital_r1` and select `properties` at the bottom of the list. A new window should open and you must navigate to `C/C++ Builds->Settings->MCU Post build outputs`. Tick the boxe that says `Convert to binary file (-O binary).


## Developing on the board
### Physically connecting to the board
We use ST-LINK V2 to connect to the board. The ST-LINK USB Connector already has wires attached to it, with labels on each wire.
IMAGE
The Orbital Platform has a connector port on it's topside, with labels that correspond to the wires on the ST-LINK Connector. 
IMAGE
Connect those wires in the order:
```
Platform --  Wire
------------------
   3     --   Pwr
   G     --   Gnd
   C     --   Clk
   D     --   Dt
   R     --   Rst
```
If the compuer is powered on, the Platform should start getting power, and start running whatever program is loaded on it. Since the `Heartbeat` LED blinks every second, it's blinking will be a clear indication of things working.
IMAGE

#### USB Connector has no Wires
If, for some odd reason, the ST-LINK USB Connector doesn't have any wires connected to it already, new wires will be needed to connect the Connector to the Platform. The order for connecting the wires is:
```
Platform -- Connector
   3     --   3.3V
   G     --   GND
   C     --   SCLK
   D     --   SDATA
   R     --   RESET
```

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
In order to debug, make sure to have a breakpoint at some line in your code (ideally close to whichever thing you think is failing). Then, make sure the Platform is physically connected. Press the `Debug` icon on the top pane. There will be a prompt to enter the `Debugger Layout`. Press `Yes`.
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

