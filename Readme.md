# ipapi

A C++ library to query IP addresses using the [ipquery.io](https://ipquery.io) API.

## Features

- Query details for a specific IP address
- Bulk query multiple IP addresses
- Fetch your own public IP address

## Installation

To use this library, include the `ipapi.h` and link the necessary dependencies for HTTP requests (e.g., libcurl).

### Prerequisites

- A C++17 or later compiler
- `libcurl` for making HTTP requests

Install `libcurl`:
```bash
# Ubuntu
sudo apt-get install libcurl4-openssl-dev

# macOS (using Homebrew)
brew install curl
```

Include the following files in your project:
- `ipapi.h`
- `ipapi.cpp`

## Usage

### Query a Specific IP Address

The `query_ip` function retrieves information about a specific IP address, including its ISP, location, and risk data.

```cpp
#include <iostream>
#include "ipapi.h"

int main() {
    auto ip_info = ipapi::query_ip("8.8.8.8");
    if (ip_info) {
        std::cout << *ip_info << std::endl;
    } else {
        std::cerr << "Failed to fetch IP information." << std::endl;
    }
    return 0;
}
```

#### Output Example
```plaintext
IPInfo {
    ip: "8.8.8.8",
    isp: { asn: "AS15169", org: "Google LLC", isp: "Google LLC" },
    location: {
        country: "United States",
        country_code: "US",
        city: "Mountain View",
        state: "California",
        zipcode: "94035",
        latitude: 37.386,
        longitude: -122.0838,
        timezone: "America/Los_Angeles",
        localtime: "2024-11-09T12:45:32"
    },
    risk: {
        is_mobile: false,
        is_vpn: false,
        is_tor: false,
        is_proxy: false,
        is_datacenter: true,
        risk_score: 0
    }
}
```

### Bulk Query Multiple IP Addresses

The `query_bulk` function allows you to query information for multiple IP addresses at once.

```cpp
#include <iostream>
#include "ipapi.h"

int main() {
    std::vector<std::string> ips = {"8.8.8.8", "1.1.1.1"};
    auto ip_infos = ipapi::query_bulk(ips);
    for (const auto& info : ip_infos) {
        std::cout << info << std::endl;
    }
    return 0;
}
```

#### Output Example
```plaintext
IPInfo {
    ip: "8.8.8.8",
    ...
}
IPInfo {
    ip: "1.1.1.1",
    ...
}
```

### Fetch Your Own Public IP Address

The `query_own_ip` function retrieves the public IP address of the current machine.

```cpp
#include <iostream>
#include "ipapi.h"

int main() {
    auto ip = ipapi::query_own_ip();
    if (ip) {
        std::cout << "Your IP Address: " << *ip << std::endl;
    } else {
        std::cerr << "Failed to fetch public IP address." << std::endl;
    }
    return 0;
}
```

#### Output Example
```plaintext
Your IP Address: 203.0.113.45
```

## API Documentation

### 1. `query_ip`

#### Signature
```cpp
std::optional<IPInfo> query_ip(const std::string& ip);
```

#### Description
Fetches detailed information about a specific IP address, including its ISP, location, and risk information.

#### Parameters
- `ip`: A `std::string` representing the IP address to query.

#### Returns
- `std::optional<IPInfo>` containing details about the IP address on success.
- `std::nullopt` if the network request fails.

---

### 2. `query_bulk`

#### Signature
```cpp
std::vector<IPInfo> query_bulk(const std::vector<std::string>& ips);
```

#### Description
Fetches information for multiple IP addresses at once. Useful for batch processing.

#### Parameters
- `ips`: A `std::vector<std::string>` containing the list of IP addresses to query.

#### Returns
- A `std::vector<IPInfo>` containing details for each IP address.

---

### 3. `query_own_ip`

#### Signature
```cpp
std::optional<std::string> query_own_ip();
```

#### Description
Fetches the public IP address of the current machine.

#### Returns
- `std::optional<std::string>` containing the public IP address on success.
- `std::nullopt` if the network request fails.

---

## Running Tests

To run tests for this library, include and execute the `tests.cpp` file:
```bash
# initialize the cmake build first
cmake -S . -B build

# Build and test
cmake --build build
cd build && ctest
```

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests on the [GitHub repository](https://github.com/rezwanahmedsami/ipapi-cpp).

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.