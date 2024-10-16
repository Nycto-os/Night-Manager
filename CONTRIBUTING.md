# Contributing to Software Manager

We appreciate all contributions, whether they are bug fixes, UI improvements, or new features.

## How to Contribute

### 1. Fork the Repository

- Fork the repository to your own GitHub account by clicking the "Fork" button on the top-right of the project page.

### 2. Clone Your Fork

- Clone your forked repository to your local machine:
    ```bash
    git clone git@github.com:acmpesuecc/Night-Manager.git
    cd Night-Manager
    ```

### 3. Create a Branch for Your Contribution

- Create a new branch for your contribution:
    ```bash
    git checkout -b issue-description
    ```

### 4. Make Your Changes

- Make your changes to the code.
- Ensure that the code follows the existing code style.
- Address the specific issue you're working on. Here are some current open issues:

#### Current Issues

- **Issue 1**: Ensure terminal widget reflects actual package operations (install/uninstall).
- **Issue 2**: Remove unnecessary or redundant packages from the list.
- **Issue 3**: UI enhancements such as resizing, improving visibility, and responsiveness.
- **Issue 4**: Implement package installation/uninstallation logic using `pacman`.
- **Issue 5**: Add support for the Arch User Repository (AUR).
- **Issue 6**: Add a warning when attempting to download/install from the AUR (must be done after resolving terminal issues).
- **Issue 7**: Add package search functionality using `yay -Ss`.

### 5. Test Your Changes

- Make sure to test the functionality thoroughly:
  - Test installing and uninstalling packages.
  - Ensure the terminal output works as expected.
  - Make sure the UI behaves correctly and is responsive.

### 6. Commit Your Changes

- Commit your changes with a meaningful commit message:
    ```bash
    git add .
    git commit -m "Fix issue #X: Description of what you've fixed/added"
    ```

### 7. Push to Your Fork

- Push your changes to your forked repository:
    ```bash
    git push origin issue-description
    ```

### 8. Create a Pull Request

- Go to the original repository on GitHub and create a pull request. Please ensure your PR includes a description of what you did and references the issue it solves (if applicable).

### 9. Get Feedback
   Wait for a maintainer to review your pull request (PR) and provide feedback.

### 10. Gain Bounty Points :)
   If everything is approved, your issue will be closed, and you'll gain bounty points on the leaderboard!

---

## Code of Conduct

We expect contributors to be respectful to others, and help maintain a welcoming environment.

## Reporting Issues

- If you encounter bugs, feel free to open an issue on the GitHub Issues page. Be sure to include a clear description of the problem and steps to reproduce it.

---

Thank you for contributing to Night Manager!
