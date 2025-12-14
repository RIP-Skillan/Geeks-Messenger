# Geeks Messenger 💬
> *Because `netcat` wasn't social enough.*

**Geeks Messenger** is a high-performance, room-based CLI chat application built with modern C++20, Boost.Asio, and Protocol Buffers. It's designed for developers who prefer the terminal over bloated GUI apps.

![License](https://img.shields.io/badge/license-MIT-blue.svg) ![C++](https://img.shields.io/badge/std-c%2B%2B20-blue) ![Build](https://img.shields.io/badge/build-passing-brightgreen)

## 🚀 Features
*   **Room-Based Chat**: Create dynamic rooms on the fly.
*   **Password Protection**: Keep your `sudo` secrets safe with password-locked rooms.
*   **Protocol Buffers**: Efficient binary serialization for minimal network overhead.
*   **Cross-Platform**: Runs on Linux and Windows (Docker support included).
*   **Auto-Cleanup**: Rooms self-destruct when the last user leaves. No memory leaks here!

## 🛠️ Tech Stack
*   **Language**: C++20
*   **Networking**: Boost.Asio (Async TCP)
*   **Serialization**: Google Protocol Buffers
*   **Build System**: CMake & CPack
*   **Deployment**: Docker & Docker Compose

## 📦 Installation

### Linux (Debian/Ubuntu)
Download the latest `.deb` from [Releases](https://github.com/RIP-Skillan/Geeks-Messenger/releases).
```bash
sudo apt install ./geeks-messenger-client-1.0.0-Linux.deb
```

### Linux (Fedora/RHEL)
Install directly from GitHub Releases:
```bash
sudo dnf install https://github.com/RIP-Skillan/Geeks-Messenger/releases/download/v1.0.0/geeks-messenger-1.0.0-1.fc42.x86_64.rpm
```

### Windows
Download the installer (`.exe`) from [Releases](https://github.com/RIP-Skillan/Geeks-Messenger/releases) and run it.

### Docker (Server)
Want to host your own?
```bash
git clone https://github.com/RIP-Skillan/Geeks-Messenger.git
cd Geeks-Messenger
docker compose up -d --build
```

## 💻 Usage
Connect to a server:
```bash
client_app <SERVER_IP> 12345
```

### Commands
| Command | Description |
| :--- | :--- |
| `/join <room> [password]` | Join a room (creates it if it doesn't exist). |
| `/leave` | Leave the current room. |
| `/quit` | Exit the application. |

**Example:**
```text
/join vim_users :wq!
> [System] rip has joined the room.
```

## 🤝 Contributing
Pull requests are welcome! Please make sure your code compiles with `-Wall -Wextra -Werror`. We don't like warnings.

## 📜 License
MIT
