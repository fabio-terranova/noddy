# Noddy

WORK IN PROGRESS...

Node-based DSP application written in C++.

<img width="1454" height="1104" alt="image" src="https://github.com/user-attachments/assets/c0e83cb6-fbdc-4128-8e0f-c306e5b5b42a" />

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
