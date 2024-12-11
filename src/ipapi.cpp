#include <iostream>
#include "ipapi.h"

namespace ipapi {

    size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
        size_t realsize = size * nmemb;
        IPInfo* ip_info = static_cast<IPInfo*>(userp);
        try {
            nlohmann::json json = nlohmann::json::parse(std::string(static_cast<char*>(contents), realsize));
            
            // Use .value() with a default to handle missing or null keys
            ip_info->ip = json.value("ip", "Unknown");

            // Handle 'isp' section
            ip_info->isp.asn = json.value("isp", nlohmann::json::object()).value("asn", "Unknown");
            ip_info->isp.organization = json.value("isp", nlohmann::json::object()).value("org", "Unknown");  // Corrected 'org' key
            ip_info->isp.name = json.value("isp", nlohmann::json::object()).value("isp", "Unknown");  // Corrected 'isp' key

            // Handle 'location' section
            ip_info->location.country = json.value("location", nlohmann::json::object()).value("country", "Unknown");
            ip_info->location.country_code = json.value("location", nlohmann::json::object()).value("country_code", "Unknown");
            ip_info->location.city = json.value("location", nlohmann::json::object()).value("city", "Unknown");
            ip_info->location.state = json.value("location", nlohmann::json::object()).value("state", "Unknown");
            ip_info->location.zipcode = json.value("location", nlohmann::json::object()).value("zipcode", "Unknown");
            ip_info->location.latitude = json.value("location", nlohmann::json::object()).value("latitude", 0.0);
            ip_info->location.longitude = json.value("location", nlohmann::json::object()).value("longitude", 0.0);
            ip_info->location.timezone = json.value("location", nlohmann::json::object()).value("timezone", "Unknown");
            ip_info->location.localtime = json.value("location", nlohmann::json::object()).value("localtime", "Unknown");

            // Handle 'risk' section
            ip_info->risk.is_mobile = json.value("risk", nlohmann::json::object()).value("is_mobile", false);
            ip_info->risk.is_vpn = json.value("risk", nlohmann::json::object()).value("is_vpn", false);
            ip_info->risk.is_tor = json.value("risk", nlohmann::json::object()).value("is_tor", false);
            ip_info->risk.is_proxy = json.value("risk", nlohmann::json::object()).value("is_proxy", false);

        } catch (const nlohmann::json::exception& e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;
        }

        return realsize;
    }

    size_t write_callback_bulk(void* contents, size_t size, size_t nmemb, void* userp) {
        size_t realsize = size * nmemb;
        std::vector<IPInfo>* ip_infos = static_cast<std::vector<IPInfo>*>(userp);
        try {
            nlohmann::json json = nlohmann::json::parse(std::string(static_cast<char*>(contents), realsize));
            for (const auto& item : json) {
                IPInfo ip_info;
                ip_info.ip = item.value("ip", "Unknown");

                // Handle 'isp' section
                ip_info.isp.asn = item.value("isp", nlohmann::json::object()).value("asn", "Unknown");
                ip_info.isp.organization = item.value("isp", nlohmann::json::object()).value("org", "Unknown");  // Corrected 'org' key
                ip_info.isp.name = item.value("isp", nlohmann::json::object()).value("isp", "Unknown");  // Corrected 'isp' key

                // Handle 'location' section
                ip_info.location.country = item.value("location", nlohmann::json::object()).value("country", "Unknown");
                ip_info.location.country_code = item.value("location", nlohmann::json::object()).value("country_code", "Unknown");
                ip_info.location.city = item.value("location", nlohmann::json::object()).value("city", "Unknown");
                ip_info.location.state = item.value("location", nlohmann::json::object()).value("state", "Unknown");
                ip_info.location.zipcode = item.value("location", nlohmann::json::object()).value("zipcode", "Unknown");
                ip_info.location.latitude = item.value("location", nlohmann::json::object()).value("latitude", 0.0);
                ip_info.location.longitude = item.value("location", nlohmann::json::object()).value("longitude", 0.0);
                ip_info.location.timezone = item.value("location", nlohmann::json::object()).value("timezone", "Unknown");
                ip_info.location.localtime = item.value("location", nlohmann::json::object()).value("localtime", "Unknown");

                // Handle 'risk' section
                ip_info.risk.is_mobile = item.value("risk", nlohmann::json::object()).value("is_mobile", false);
                ip_info.risk.is_vpn = item.value("risk", nlohmann::json::object()).value("is_vpn", false);
                ip_info.risk.is_tor = item.value("risk", nlohmann::json::object()).value("is_tor", false);
                ip_info.risk.is_proxy = item.value("risk", nlohmann::json::object()).value("is_proxy", false);

                ip_infos->push_back(ip_info);
            }
        } catch (const nlohmann::json::exception& e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;
        }

        return realsize;
    }

    IPInfo query_ip(const std::string& ip) {
        IPInfo ip_info;
        CURL* curl = curl_easy_init();
        if (curl) {
            std::string url = BASE_URL + ip;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ip_info);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "Failed to fetch IP information: " << curl_easy_strerror(res) << std::endl;
            }
            curl_easy_cleanup(curl);
        } else {
            std::cerr << "Failed to initialize cURL." << std::endl;
        }
        return ip_info;
    }

    std::vector<IPInfo> query_bulk(const std::vector<std::string>& ips) {
        std::vector<IPInfo> ip_infos;
        CURL* curl = curl_easy_init();
        if (curl) {
            std::string ip_list = std::accumulate(std::next(ips.begin()), ips.end(), ips[0], [](std::string a, std::string b) {
                return a + "," + b;
            });
            std::string url = BASE_URL + ip_list;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_bulk);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ip_infos);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "Failed to fetch IP information: " << curl_easy_strerror(res) << std::endl;
            }
            curl_easy_cleanup(curl);
        } else {
            std::cerr << "Failed to initialize cURL." << std::endl;
        }
        return ip_infos;
    }

    std::string query_own_ip() {
        std::string ip;
        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, BASE_URL.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ip);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "Failed to fetch IP information: " << curl_easy_strerror(res) << std::endl;
            }
            curl_easy_cleanup(curl);
        } else {
            std::cerr << "Failed to initialize cURL." << std::endl;
        }
        return ip;
    }
    
}
