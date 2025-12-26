# Bahria LMS Clone (OOP Project)

This project is a C++ application that provides a **Learning Management System (LMS)** for Bahria University. It features two modes: a robust **Console Application** and a modern **Web-Based Graphical User Interface (GUI)**.

## Features
- **Object-Oriented Design**: Built using Inheritance, Polymorphism, and Encapsulation.
- **Dual Interface**:
  1.  **Console Mode**: Text-based menu system.
  2.  **Web Mode**: Premium HTML5/CSS3 Dashboard running on a local C++ web server (Port 8080).
- **Roles**: Admin, Faculty, Student.

## How to Run

### Prerequisites
- CLion (or CMake + MinGW for Windows).
- A web browser (Chrome/Edge/Firefox).

### Steps
1.  **Build** the project in CLion.
2.  **Run** the executable `Bahria_LMS.exe`.
3.  You will be prompted to select a mode:
    ```
    Select Mode:
    1. Console Application
    2. Web Server GUI (http://localhost:8080)
    ```
4.  **For GUI**: Select Option `2`.
    - Open your browser and go to `http://localhost:8080`.
    - Login with the credentials below.

### Default Credentials
- **Admin**: ID: `admin`, Pass: `admin123`
- **Faculty**: ID: `emp1`, Pass: `pass`
- **Student**: ID: `01-1342`, Pass: `pass`

## Project Structure
- `LMS.h/.cpp`: Main System Controller.
- `WebServer.h/.cpp`: Custom HTTP Server implementation using `winsock2`.
- `www/`: Contains frontend resources (`index.html`, `style.css`, `app.js`).
- `User` Classes: `User`, `Student`, `Faculty`, `Admin`.

## OOP Implementation Details
- **Polymorphism**: The `User::showMenu()` is key. In Console mode, it prints text menus. In the future, it can be extended to generate role-specific JSON for the API.
- **Web Server**: The server is implemented from scratch using Sockets to demonstrate low-level system programming skills alongside high-level OOP design.
