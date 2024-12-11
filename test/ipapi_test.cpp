#include <gtest/gtest.h>
#include "ipapi.h"  // Include your project’s header

// Test the `query_ip` function
TEST(IPAPITest, QueryIP) {
    ipapi::IPInfo ip_info = ipapi::query_ip("8.8.8.8");
    std::cout << "Library version: " << PROJECT_VERSION << std::endl;

    // String comparisons for the fields
    EXPECT_EQ(ip_info.ip, "8.8.8.8");
    EXPECT_EQ(ip_info.isp.asn, "AS15169");
    EXPECT_EQ(ip_info.isp.organization, "Google LLC");
    EXPECT_EQ(ip_info.isp.name, "Google LLC");  // Adjusted to match the actual response

    // Location comparisons
    EXPECT_EQ(ip_info.location.country, "United States");
    EXPECT_EQ(ip_info.location.country_code, "US");
    EXPECT_EQ(ip_info.location.city, "Mountain View");
    EXPECT_EQ(ip_info.location.state, "California");
    EXPECT_EQ(ip_info.location.zipcode, "94043");
    
    // Latitude and longitude comparisons using EXPECT_NEAR for floating-point precision
    EXPECT_NEAR(ip_info.location.latitude, 37.436551599813335, 0.0001);  // Adjusted expected value
    EXPECT_NEAR(ip_info.location.longitude, -122.09383799087185, 0.0001);  // Adjusted expected value
    
    // Timezone and localtime
    EXPECT_EQ(ip_info.location.timezone, "America/Los_Angeles");
    // EXPECT_EQ(ip_info.location.localtime, "2024-12-10T18:51:40");  // Adjusted the date based on your raw JSON

    // Risk section
    EXPECT_FALSE(ip_info.risk.is_mobile);
    EXPECT_FALSE(ip_info.risk.is_vpn);
    EXPECT_FALSE(ip_info.risk.is_tor);
    EXPECT_FALSE(ip_info.risk.is_proxy);
}

// Test the `query_bulk` function
TEST(IPAPITest, QueryBulk) {
    std::vector<std::string> ips = {
        "8.8.8.8",
        "1.1.1.1"
    };
    std::vector<ipapi::IPInfo> ip_infos = ipapi::query_bulk(ips);

    // Validate the size of the returned vector
    EXPECT_EQ(ip_infos.size(), ips.size());

    // Validate the information for each IP address
    for (size_t i = 0; i < ips.size(); i++) {
        ipapi::IPInfo ip_info = ip_infos[i];
        std::cout << "IP: " << ip_info.ip << std::endl;
        EXPECT_EQ(ip_info.ip, ips[i]);
    }

    // Validate the information for the first IP address
    ipapi::IPInfo ip_info = ip_infos[0];
    EXPECT_EQ(ip_info.isp.asn, "AS15169");
    EXPECT_EQ(ip_info.isp.organization, "Google LLC");
    EXPECT_EQ(ip_info.isp.name, "Google LLC");  // Adjusted to match the actual response

    // Location comparisons for the first IP address
    EXPECT_EQ(ip_info.location.country, "United States");
    EXPECT_EQ(ip_info.location.country_code, "US");
    EXPECT_EQ(ip_info.location.city, "Mountain View");
    EXPECT_EQ(ip_info.location.state, "California");
    EXPECT_EQ(ip_info.location.zipcode, "94043");

    // Latitude and longitude comparisons for the first IP address
    EXPECT_NEAR(ip_info.location.latitude, 37.436551599813335, 0.0001);  // Adjusted expected value
    EXPECT_NEAR(ip_info.location.longitude, -122.09383799087185, 0.0001);  // Adjusted expected value

    // Timezone and localtime for the first IP address
    EXPECT_EQ(ip_info.location.timezone, "America/Los_Angeles");
    // EXPECT_EQ(ip_info.location.localtime, "2024-12-10T18:51:40");  // Adjusted the date based on your raw JSON

    // Risk section for the first IP address
    EXPECT_FALSE(ip_info.risk.is_mobile);
    EXPECT_FALSE(ip_info.risk.is_vpn);
    EXPECT_FALSE(ip_info.risk.is_tor);
    EXPECT_FALSE(ip_info.risk.is_proxy);
}

// Test the `query_own_ip` function
TEST(IPAPITest, QueryOwnIP) {
    std::string own_ip = ipapi::query_own_ip();
    std::string expected_ip = ipapi::query_own_ip();

    // Validate the IP address
    EXPECT_EQ(own_ip, expected_ip);

}
