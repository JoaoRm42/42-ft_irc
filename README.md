# 42 Project FT_IRC

Welcome to the FT_IRC project! This project is part of the 42 curriculum and focuses on building a functional IRC (Internet Relay Chat) server in C++. The goal is to understand network programming, socket communication, and the IRC protocol. This README will guide you through the project details, setup instructions, and additional resources.

## Table of Contents
- [Introduction](#introduction)
- [Project Objectives](#project-objectives)
- [Requirements](#requirements)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [Contributing](#contributing)
- [Acknowledgments](#acknowledgments)
- [Grade](#grade)

## Introduction

FT_IRC is a system programming project that aims to implement an IRC server compliant with RFC 2812. Through this project, students will gain hands-on experience with network communication, socket programming, and managing multiple client connections.

## Project Objectives

- Implement a multi-client IRC server.
- Manage user connections and handle basic IRC commands.
- Enable channel creation and management.
- Implement private messaging between users.
- Ensure server stability and handle edge cases gracefully.

## Requirements

- Proficiency in C++.
- Understanding of network programming concepts.
- Familiarity with the IRC protocol (RFC 2812).

## Project Structure

Here's a brief overview of the project structure:

```
.
├── Bot
│   └── Bot.cpp
├── channel
│   ├── channel.cpp
│   └── channel.hpp
├── client
│   ├── Client.cpp
│   └── Client.hpp
├── server
│   ├── channelOperators
│   │   ├── inviteCommand.cpp
│   │   ├── joinCommand.cpp
│   │   ├── kickCommand.cpp
│   │   ├── modeCommand.cpp
│   │   ├── partCommand.cpp
│   │   ├── quitCommand.cpp
│   │   └── topicCommand.cpp
│   ├── getChannel.cpp
│   ├── getHostname.cpp
│   ├── server.cpp
│   ├── server.hpp
│   └── welcomeMessage.cpp
├── utils
│   ├── utils.cpp
│   └── utils.hpp
├── Makefile
├── libs.hpp
└── README.md
```

## Installation
1. **Clone the Repository:**
    ```bash
    git clone https://github.com/JoaoRm42/42-ft_irc
    cd 42-ft_irc
    ```

2. **Build the Project:**
    ```bash
    make
    ```
This will create the necessary executable file (e.g., `ircserv`).

## Usage

To run the `ircserv` program, use the following command:

```bash
./irc_server <port> <password>
```
Replace <port> with the desired port number and <password> with the server password.

### Hexchat

1. **Connecting with Hexchat**
Install Hexchat (if not already installed):
```bash
sudo apt-get install hexchat
```
or download from Hexchat's official website.



2. **Open Hexchat and follow these steps:**

Click on "Add" to create a new network.

![Screenshot from 2024-07-29 10-53-51](https://github.com/user-attachments/assets/39f733a2-a325-474c-ac3b-da7d4a19403c)


Name the network and press Enter.

![Screenshot from 2024-07-29 11-02-08](https://github.com/user-attachments/assets/5da317d3-4aa7-4c5d-ac4e-f3f537a04d34)


Select the newly created network and click on "Edit."

![Screenshot from 2024-07-29 11-05-23](https://github.com/user-attachments/assets/460b9bf1-dd82-4c7a-b353-0d920ba1ba6f)


In the "Servers for" section, add localhost/<port>, replacing <port> with the port number your server is listening on.
In the "Password" section add the correct password that you gave the server and leave the boxes of "Accept invalid SSL certificates" & "Use global user information" checked. The other boxes leave them unchecked!

![Screenshot from 2024-07-29 11-07-50](https://github.com/user-attachments/assets/cac6b02b-cb01-4ecc-82cc-b02ff74113c2)


Click "Close" and then "Connect."

3. **Join a Channel:**

Use the command /join #<channel_name> to join or create a channel.

### netcat (nc)

1. **Connecting with netcat**
To connect using netcat, use the following command:
```bash
nc localhost <port>
```
Replace <port> with the port number your server is listening on.

## Features

- Multi-client support: Handle multiple connections simultaneously.
- Channel management: Create, join, and manage channels.
- Private messaging: Send private messages between users.
- Basic commands: Implement essential IRC commands like JOIN, PART, PRIVMSG, and QUIT.
- Bot integration: Includes a simple bot that can automate tasks and respond to user commands.
- File transfer: Support for file transfers between users.

## Contributing
Contributions are welcome! If you have any suggestions or improvements, feel free to create an issue or submit a pull request. Please ensure that your contributions adhere to the coding standards and guidelines of the project.

## Acknowledgments
- A special thanks to my partners [ccosta-c](https://github.com/ccosta-c) && [macastanm](https://github.com/macastanm) for making this project a success.
- The 42 Network for providing the inspiration and resources for this project.
- The developers of https://modern.ircdocs.horse for their comprehensive guide.
- The creators of the IRC protocol documentation (RFC 2812).
- All contributors who have helped improve this project.

## Grade
- Grade: 125/100

---

Happy coding!
