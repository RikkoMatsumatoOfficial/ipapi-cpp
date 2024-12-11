#ifndef IPAPI_H
#define IPAPI_H
#include <string>
#include <vector>
#include <curl/curl.h>
#include <json.hpp>
#include <version.h>

namespace ipapi {

    /// The base URL for the ipquery.io API.
    const std::string BASE_URL = "https://api.ipquery.io/";

    /// Represents information about an ISP (Internet Service Provider).
    struct IspInfo {
        /// The Autonomous System Number (ASN) of the ISP.
        std::string asn;
        /// The organization associated with the ISP.
        std::string organization;
        /// The name of the ISP.
        std::string name;
    };

    /// Represents information about the geographical location of an IP address.
    struct LocationInfo {
        /// The country name.
        std::string country;

        /// The ISO country code.
        std::string country_code;

        /// The city name.
        std::string city;

        /// The state or region.
        std::string state;

        /// The postal or ZIP code.
        std::string zipcode;

        /// The latitude of the location.
        double latitude;

        /// The longitude of the location.
        double longitude;

        /// The timezone of the location.
        std::string timezone;

        /// The local time in the specified timezone.
        std::string localtime;
    };

    /// Represents information about potential risks associated with an IP address.
    struct RiskInfo {
        /// Indicates if the IP is associated with a mobile network.
        bool is_mobile;
        /// Indicates if the IP is using a VPN.
        bool is_vpn;
        /// Indicates if the IP is part of the Tor network.
        bool is_tor;
        /// Indicates if the IP is using a proxy.
        bool is_proxy;
        /// Indicates if the IP is associated with a data center.
        bool is_datacenter;
        /// A score indicating the risk level (0-100).
        unsigned char risk_score;
    };

    /// Represents the full set of information returned by the API for an IP address.
    struct IPInfo {
        /// The queried IP address.
        std::string ip;
        /// Information about the ISP.
        IspInfo isp;
        /// Information about the location.
        LocationInfo location;
        /// Information about the risk level.
        RiskInfo risk;
    };

    /// Callback function for writing the response data from the API request.
    ///
    /// # Arguments
    ///
    /// * `contents` - A pointer to the response data.
    /// * `size` - The size of each element in the data.
    /// * `nmemb` - The number of elements in the data.
    /// * `userp` - A pointer to the user data.
    ///
    /// # Returns
    ///
    /// Returns the total size of the data written.
    size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);

    // callback function for bulk requests, where will receive a vector of IPInfo
    size_t write_callback_bulk(void* contents, size_t size, size_t nmemb, void* userp);

    /// Fetches the IP information for a given IP address.
    ///
    /// # Arguments
    ///
    /// * `ip` - A string representing the IP address to query.
    ///
    /// # Returns
    ///
    /// Returns an `IPInfo` struct containing the information about the IP address.
    ///
    /// # Example
    ///
    /// ```cpp
    /// #include <iostream>
    /// #include "ipapi.h"
    ///
    /// int main() {
    ///     auto ip_info = ipapi::query_ip("8.8.8.8");
    ///     std::cout << "IP: " << ip_info.ip << std::endl;
    ///     std::cout << "ISP: " << ip_info.isp.name << std::endl;
    ///     std::cout << "Country: " << ip_info.location.country << std::endl;
    ///     return 0;
    /// }
    /// ```
    ///
    /// # Errors
    ///
    /// Returns an error if the network request fails or the response cannot be deserialized.
    IPInfo query_ip(const std::string& ip);

    /*
        /// Fetches information for multiple IP addresses.
        ///
        /// # Arguments
        ///
        /// * `ips` - A slice of string slices representing the list of IP addresses to query.
        ///
        /// # Example
        ///
        /// ```rust
        /// use ipapi::query_bulk;
        /// use tokio;
        ///
        /// #[tokio::main]
        /// async fn main() {
        ///     let ip_infos = query_bulk(&["8.8.8.8", "1.1.1.1"]).await.unwrap();
        ///     println!("{:?}", ip_infos);
        /// }
        /// ```
        ///
        /// # Errors
        ///
        /// Returns an error if the network request fails or the response cannot be deserialized.
        pub async fn query_bulk(ips: &[&str]) -> Result<Vec<IPInfo>, Error> {
            let ip_list = ips.join(",");
            let url = format!("{}{}", BASE_URL, ip_list);
            let response = reqwest::get(&url).await?.json::<Vec<IPInfo>>().await?;
            Ok(response)
        }
    */
   
    /// Fetches information for multiple IP addresses.
    ///
    /// # Arguments
    ///
    /// * `ips` - A vector of strings representing the list of IP addresses to query.
    ///
    /// # Returns
    ///
    /// Returns a vector of `IPInfo` structs containing the information about the IP addresses.
    ///
    /// # Example
    ///
    /// ```cpp
    /// #include <iostream>
    /// #include "ipapi.h"
    ///
    /// int main() {
    ///     std::vector<std::string> ips = {"
    ///         "8.8.8.8",
    ///         "1.1.1.1"
    ///     };
    ///     auto ip_infos = ipapi::query_bulk(ips);
    ///     for (const auto& ip_info : ip_infos) {
    ///         std::cout << "IP: " << ip_info.ip << std::endl;
    ///         std::cout << "ISP: " << ip_info.isp.name << std::endl;
    ///         std::cout << "Country: " << ip_info.location.country << std::endl;
    ///     }
    ///     return 0;
    /// }
    /// ```
    ///
    /// # Errors
    ///
    /// Returns an error if the network request fails or the response cannot be deserialized.
    std::vector<IPInfo> query_bulk(const std::vector<std::string>& ips);

    /*
        /// Fetches the IP address of the current machine.
        ///
        /// # Example
        ///
        /// ```rust
        /// use ipapi::query_own_ip;
        /// use tokio;
        ///
        /// #[tokio::main]
        /// async fn main() {
        ///     let ip = query_own_ip().await.unwrap();
        ///     println!("Your IP Address: {}", ip);
        /// }
        /// ```
        ///
        /// # Errors
        ///
        /// Returns an error if the network request fails.
        pub async fn query_own_ip() -> Result<String, Error> {
            let response = reqwest::get(BASE_URL).await?.text().await?;
            Ok(response)
        }
    */

    /// Fetches the IP address of the current machine.
    ///
    /// # Returns
    ///
    /// Returns a string representing the IP address of the current machine.
    ///
    /// # Example
    ///
    /// ```cpp
    /// #include <iostream>
    /// #include "ipapi.h"
    ///
    /// int main() {
    ///     auto ip = ipapi::query_own_ip();
    ///     std::cout << "Your IP Address: " << ip << std::endl;
    ///     return 0;
    /// }
    /// ```
    ///
    /// # Errors
    ///
    /// Returns an error if the network request fails.
    std::string query_own_ip();
    



}

#endif // IPAPI_H
