# Socket_Programming
Socket client-server communication

---

## 🛠️ Build Instructions

### 1. Create Build Directory
```bash
mkdir -p build
````

### 2. Compile the Project

```bash
make
```

### 3. Clean Build Files

```bash
make clean
```

👉 All compiled binaries will be generated inside the `build/` directory (same level as `README.md`).

---

## ▶️ Run the Programs

Navigate to build directory:

```bash
cd build
```

### Start Server

```bash
./server
```

### Start Multi-Client Server

```bash
./server_multi_client
```

### Start Client

```bash
./client
```

---

## 🖼️ Socket Communication Flow

### 🔹 Single Client - Server Communication

![Single Client Flow](Socket.png)

---

### 🔹 Multiple Clients - Server Communication

![Multi Client Flow](Socket_multiple_client.png)

---

## 💻 VS Code Setup (with DevContainer)

This project is fully compatible with **VS Code Dev Containers (Ubuntu environment)**.

---

### 📦 DevContainer Configuration

Located in:

```
.devcontainer/devcontainer.json
```

Key Features:

* Uses Docker container
* Host network enabled (`--network=host`)
* Preinstalled extensions:

  * C/C++ support
  * Makefile tools
  * Python (optional utilities)

---

### ⚙️ VS Code Tasks Configuration

Located in:

```
.vscode/tasks.json
```

---

### 🚀 Available Tasks

| Task Name                        | Description                           |
| -------------------------------- | ------------------------------------- |
| Create a Build folder            | Creates `build/` directory            |
| Build C File                     | Compiles server & client using `make` |
| Clean File                       | Cleans build using `make clean`       |
| Run Program                      | Runs selected binary                  |
| Start Socket Server              | Runs `server`                         |
| Start Socket Multi-Client Server | Runs `server_multi_client`            |
| Start Socket Client              | Runs `client`                         |
| Remove a Build folder            | Deletes `build/`                      |

---

### ▶️ How to Run in VS Code

1. Open project in VS Code

2. Reopen in DevContainer

   ```
   Ctrl + Shift + P → Dev Containers: Reopen in Container
   ```

3. Run tasks:

   ```
   Ctrl + Shift + B
   ```

4. Select task:

   * Tasks: Run Task
   * Build C File
   * Start Socket Server / Client

---

### ⚡ Example Workflow in VS Code

1. **Create Build Folder**
2. **Build C File**
3. **Start Socket Server**
4. **Start Socket Client**

---

## 📌 Notes

* Ensure ports used in server/client match
* Server supports concurrent connections of multiple-clients
* Uses standard Linux socket APIs (`socket`, `bind`, `listen`, `accept`, etc.)

---

## 👨‍💻 Author

Developed for learning and demonstration of Linux socket programming.

