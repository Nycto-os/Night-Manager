# Night Manager

A GTK-based Software Manager that provides a graphical interface for managing packages on an Arch-based system. It integrates with `pacman` and will eventually include AUR support to install and remove packages directly from the application, with a built-in terminal for transparency.

## Features

- Browse available packages using `pacman`.
- View detailed information about each package.
- Install and uninstall packages.
- Integrated terminal to show package management logs.
- Future support for AUR with warning prompt.
- Basic search functionality (WIP).

## Getting Started

### Prerequisites

- GTK 3
- VTE Terminal
- Arch Linux or any Arch-based distribution (for testing, feel free to use a vm, or request help on the same)
- `pacman` and optionally `yay` for AUR support

### Installation

1. Clone the repository:
    ```bash
    git clone git@github.com:acmpesuecc/Night-Manager.git
    cd Night-Manager
    ```

2. Install dependencies (for linux):
    ```bash

    sudo pacman -S gtk3 vte3 base-devel
    ```

3. Compile and run the software:
    ```bash
    gcc -o software-manager software_manager.c $(pkg-config --cflags --libs gtk+-3.0 vte-2.91)
    ./software-manager
    ```

### Usage

- Select a package from the list to view its details.
- Use the `Install` button to install the selected package.
- Use the `Uninstall` button to remove an installed package.
- Toggle the built-in terminal to see the `pacman` logs during installation and uninstallation.
  
### Roadmap

#### Issues
- [ ] **Issue 1**: Terminal output should reflect actual package management operations.
- [ ] **Issue 2**: Remove unneeded or redundant packages from the list.
- [ ] **Issue 3**: UI Enhancements for better user experience.
- [ ] **Issue 4**: Implement actual functionality to install/uninstall packages.
- [ ] **Issue 5**: Include support for installing packages from AUR.
- [ ] **Issue 6**: Show a warning when installing from AUR.
- [ ] **Issue 7**: Add package search functionality (e.g., via `yay -Ss`).

### Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on how to contribute to this project.

---

## License

This project is licensed under the GNU General Public License V3 - see the [LICENSE](LICENSE) file for details.
