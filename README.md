# Noddy

WORK IN PROGRESS...

Node-based DSP application written in C++.

<img width="1472" height="1114" alt="image" src="https://github.com/user-attachments/assets/0015cb2e-313e-4d0c-9cc4-40ae3331ed31" />

## Building

This project uses CMake. To build:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Project Structure

```
├── app/          # Main application code
├── core/         # Core library
├── external/     # Third-party dependencies
└── tests/        # Test files
```

## Usage

Run the application from the build directory:

```bash
./bin/noddy_gui
```

## License

See [LICENSE](LICENSE) for details.
