# HTTP Proxy Server

This project is a simple HTTP proxy server written in C. It was developed for a networking and cybersecurity assignment to demonstrate fundamental skills in TCP socket programming, HTTP parsing, and basic string manipulation in C.

## Features

- Accepts three command-line arguments: a listen port, a destination host, and a destination port
- Parses and forwards HTTP GET requests starting with `/forward/`
- Returns a proper `404` response for unsupported or malformed requests
- Demonstrates understanding of TCP socket communication and HTTP basics in C

## 🗂 Files

| File               | Description                                                  |
|--------------------|--------------------------------------------------------------|
| `httpproxy.c`       | Main source file for the HTTP proxy implementation           |
| `Makefile`          | Build instructions for compiling `httpproxy`                |
| `mongoose.c`, `mongoose.h` | Lightweight HTTP server library used by test service (provided) |
| `service-httpproxy` | Provided test server used to simulate destination behavior   |
| `README.md`         | You're reading it :)                                         |

> ⚠️ The `mongoose.*` files and `service-httpproxy` were provided for testing and are **not authored** by me. They are included for completeness and to support reproducibility of results.

## 🛠 Compilation

Use the provided `Makefile` to compile:

```bash
make
