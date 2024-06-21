# Beremiz - Reimplementade in C

Beremiz is a stack-based programming language that I created. It was initially created in Lua. This repository hosts a work-in-progress C reimplementation of the Beremiz language. The original Lua version remains the definitive and most up-to-date version, available at https://github.com/AdaiasMagdiel/beremiz.

## Table of Contents

- [Purpose](#purpose)
- [Getting Started](#getting-started)
  - [Installation](#installation)
  - [Building the Project](#building-the-project)
  - [Running the Project](#running-the-project)
- [License](#license)
- [Contributing](#contributing)
- [Additional Notes](#additional-notes)

## Purpose

This C reimplementation serves primarily as a proof-of-concept (POC) to explore the language's functionality in a different programming environment while I enhancing my C programming skills. While the Lua version remains the primary focus, the C reimplementation holds potential to become the definitive version in the future.

## Getting Started

This section equips you with the knowledge to set up, build, and run this implementation.

### Installation

1. **Clone the Repository:**

   Begin by cloning the Beremiz C repository from GitHub using the following command:

   ```bash
   git clone https://github.com/AdaiasMagdiel/beremiz-c
   ```

2. **Navigate to Project Directory:**

   Once the cloning process is complete, navigate to the newly created project directory:

   ```bash
   cd beremiz-c
   ```

### Building the Project

The project utilizes a `Makefile` to simplify the build process across different operating systems. To create the Beremiz interpreter executable, run the following command in the project directory:

   ```bash
   make
   ```

This command will compile the source code and generate the `beremiz` executable file.

### Running the Project

There are two ways to run the Beremiz interpreter:

1. **Executing a Beremiz Script:**

   To execute a Beremiz script file, provide the filename (including the `.brz` extension) as an argument when running the `beremiz` executable:

   ```bash
   ./beremiz <filename>.brz
   ```

   Replace `<filename>.brz` with the actual name of your Beremiz script.

2. **Interactive Mode (REPL):**

   To launch the Beremiz interpreter in interactive mode (REPL), simply run the `beremiz` executable without any arguments:

   ```bash
   ./beremiz
   ```

   This will start the REPL, allowing you to enter Beremiz code line by line for immediate execution and evaluation.

## License

This project is licensed under the MIT License. Refer to the [`LICENSE`](LICENSE) file for more details.

## Contributing

Contributions are welcome! Feel free to create pull requests with improvements or bug fixes.

## Additional Notes

- This project is still under development, and the C reimplementation may not fully replicate all features of the Lua version.
