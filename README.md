# zosclib

![ZOSCLIB Logo](./img/zosc_logo.png) 

zosclib is a C++ library for working with the Open Sound Control (OSC) protocol. It provides tools for creating, sending, receiving, and processing OSC messages and bundles with a simple and intuitive API.

---

## Features 🚀

### ZoscMessage  
- Create and manage OSC messages with address patterns.  
- Supports multiple argument types:  
  - `int32_t`  
  - `float`  
  - `std::string`  
  - `std::vector<uint8_t>`  
  - `uint32_t`  
- Serialize and deserialize OSC-compliant byte streams.  

### ZoscBundle  
- Nest OSC messages and bundles hierarchically.  
- Use NTP-compatible TimeTags for precise scheduling.  
- Serialize and deserialize bundles into OSC formats.  

### ZoscReceiver  
- Listen for incoming OSC messages and bundles on a specified UDP port.  
- Use callback functions for customized message and bundle processing.  
- Operates asynchronously using the ASIO library.  

### ZoscSender  
- Send OSC messages and bundles over UDP.  
- Supports raw OSC-encoded data transmission.  
- Optimized for low-latency networking.  

---

## Usage 🔧 

### Requirements  
- C++17 or later  
- [ASIO](https://think-async.com/) library (via Boost)  

### Building Zosc  
1. Clone the repository:  
   ```bash
   git clone https://github.com/PedroZappa/zosclib.git
   cd zosclib
   ```
2. Build the library:
```bash
make
```
Include the zosclib headers in your project. Zosclib is a header-only library, so no additional build steps are required.

___

## Documentation 📖

The full documentation for **zosclib** is available online. You can view it by clicking the link below:

<a href="https://PedroZappa.github.io/zosclib/" target="_blank">View Documentation</a>

> Alternatively, you can also generate the documentation locally by running Doxygen and opening the `index.html` file in a browser.

---

## References 📖

- [Open Sound Control](https://opensoundcontrol.org/)
- [OSC Syntax](https://opensoundcontrol.stanford.edu/spec-1_0.html#introduction)
- [Open Sound Control Wikipedia](https://en.wikipedia.org/wiki/Open_Sound_Control)
- [Best Practices for OSC STANFORD](https://opensoundcontrol.stanford.edu/files/osc-best-practices-final.pdf)
- [mhroth/tinyosc](https://github.com/mhroth/tinyosc)

___

## License 🔏

This work is published under the terms of <a href="https://github.com/PedroZappa/zosclib/blob/main/LICENSE">The Unlicense</a>.
