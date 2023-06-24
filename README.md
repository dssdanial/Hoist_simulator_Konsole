# Hoist_simulator_Konsole
An interactive simulator of a hoist using two shell windows and keys with different aims which allow the user to activate the hoist.

## ARP-Assignment2

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#Introduction">Introduction</a></li>
    <li><a href="#Installation_and_Running">Installation_and_Running</a></li>
    <li><a href="#Compiling">Compiling</a></li>
    <li><a href="#Members">Members</a></li>
  </ol>
</details>

## Introduction

Base project structure for the first *Advanced and Robot Programming* (ARP) assignment.
The project provides the basic functionalities for the **Command** and **Inspection processes**, both of which are implemented through the *ncurses library* as simple GUIs. In particular, the repository is organized as follows:
- The `src` folder contains the source code for the Command, Inspection, and Master processes.
- The `include` folder contains all the data structures and methods used within the ncurses framework to build the two GUIs. Unless you want to expand the graphical capabilities of the UIs (which requires understanding how ncurses works), you can ignore the content of this folder, as it already provides you with all the necessary functionalities.
- The `bin` folder is where the executable files are expected to be after compilation
![image](https://github.com/dssdanial/Hoist_simulator_Konsole/assets/32397445/4cbfeb6d-31a0-46b9-a3b6-4966c1247f11)

**The simulator comprises five essential processes:**

Command Console: This process is responsible for reading the six commands input by the user through keyboard keys. It facilitates the control and management of the hoist's movements along the x and z axes.

Inspection Console: The inspection console receives positional data from the motors while the hoist is in motion. It displays this information on the screen, allowing users to monitor the hoist's position in real time. Additionally, the inspection console handles the S and R buttons, providing emergency stop and reset functionalities.

**Motor X:** Simulating motion along the x-axis, this process receives commands from the command console and provides real-time positional feedback, including simulated errors. It plays a vital role in ensuring accurate and controlled movement of the hoist along the x-axis.

**Motor Z:** Similar to the motor X process, this component simulates motion along the z-axis. It receives commands, sends positional updates, and contributes to the overall movement and positioning of the hoist.

**Watchdog:** The watchdog process periodically checks the status of the previous four processes. If it detects that no computation, motion, or input/output has occurred for a certain duration (e.g., 60 seconds), it triggers a reset, emulating the functionality of the R button. This helps maintain the system's integrity and ensures proper operation.

Throughout this project, our aim is to create an immersive and interactive simulator that accurately replicates the behavior of a hoist with 2 d.o.f. The combination of the command console, inspection console, motors, and watchdog process enables users to control the hoist's movements, monitor its position, and ensure safe operation within the defined boundaries.

## Installation_and_Running
### ncurses installation
To install the ncurses library, simply open a terminal and type the following command:
```console
sudo apt-get install libncurses-dev
```

### Compiling
Once the installation is done, clone this repository to your desired location:
<pre><code>git clone https://github.com/dssdanial/Hoist_simulator_Konsole.git </code></pre>
After you clone the repository, run this command:
<pre><code> sh run.sh </code></pre>


## Members
| Github-page | Email |
|------------------|------------------|
| [Danial Sabzevari](https://github.com/dssdanial)  | s5217544@studenti.unige.it |
| [Parinaz Ramezanpour](https://github.com/ParinazRmp)  | s5214640@studenti.unige.it |
