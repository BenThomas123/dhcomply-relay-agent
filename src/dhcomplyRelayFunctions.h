#ifndef DHCOMPLYFUNCTIONS_H
#define DHCOMPLYFUNCTIONS_H

#include "dhcomplyStandardLibrary.h"


// message type constants
/* =========================================== */
#define SOLICIT_MESSAGE_TYPE               1
#define ADVERTISE_MESSAGE_TYPE             2
#define REQUEST_MESSAGE_TYPE               3
#define CONFIRM_MESSAGE_TYPE               4
#define RENEW_MESSAGE_TYPE                 5
#define REBIND_MESSAGE_TYPE                6
#define REPLY_MESSAGE_TYPE                 7
#define RELEASE_MESSAGE_TYPE               8
#define DECLINE_MESSAGE_TYPE               9
#define RECONFIGURE_MESSAGE_TYPE          10
#define INFORMATION_REQUEST_MESSAGE_TYPE  11
#define RELAY_FORWARD_MESSAGE_TYPE        12
#define RELAY_REPLY_MESSAGE_TYPE          13
/* =========================================== */

// option code constants
/* ========================================== */
#define CLIENT_ID_OPTION_CODE              1
#define SERVER_ID_OPTION_CODE              2
#define IA_NA_OPTION_CODE                  3
#define IA_ADDR_OPTION_CODE                5
#define ORO_OPTION_CODE                    6
#define PREFERENCE_OPTION_CODE             7
#define ELAPSED_TIME_OPTION_CODE           8
#define RELAY_MSG_OPTION_CODE              9
#define AUTH_OPTION_CODE                  11
#define UNICAST_OPTION_CODE               12
#define STATUS_CODE_OPTION_CODE           13
#define RAPID_COMMIT_OPTION_CODE          14
#define USER_CLASS_OPTION_CODE            15
#define VENDOR_CLASS_OPTION_CODE          16
#define VENDOR_OPTS_OPTION_CODE           17
#define INTERFACE_ID_OPTION_CODE          18
#define RECONF_MSG_OPTION_CODE            19
#define RECONF_ACCEPT_OPTION_CODE         20
#define DNS_SERVERS_OPTION_CODE           23
#define DOMAIN_SEARCH_LIST_OPTION_CODE    24
#define IA_PD_OPTION_CODE                 25
#define IAPREFIX_OPTION_CODE              26
#define INFORMATION_REFRESH_OPTION_CODE   32
#define FQDN_OPTION_CODE                  39
#define PD_EXCLUDE_OPTION_CODE            67
#define SOL_MAX_RT_OPTION_CODE            82
#define INF_MAX_RT_OPTION_CODE            83
/* ========================================== */

// port number constants
/* ============================================ */
#define DHCP_RELAY_PORT                  547
/*============================================= */

// address constants
/* ================================================== */
#define ALL_DHCP_RELAY_AGENTS_AND_SERVERS "ff02::1:2"
#define ALL_DHCP_SERVERS                  "ff05::1:3"
/* ================================================== */

// status code constants
/* ================================================== */
#define UNSPECFAIL_STATUS_CODE            1
#define NOADDRAVAIL_STATUS_CODE           2
#define NOBINDING_STATUS_CODE             3
#define NOTONLINK_STATUS_CODE             4
#define USEMULTICAST_STATUS_CODE          5
#define NOPREFIXAVAIL_STATUS_CODE         6
/* ================================================== */

/*
    Miscellaneous constants
*/
/* ================================== */
#define MILLISECONDS_IN_SECONDS 1000
#define MAX_PACKET_SIZE 2000
/* ================================== */


/* ============================================= */
#define ONE_BYTE_SHIFT 8
#define TWO_BYTE_SHIFT 16
#define THREE_BYTE_SHIFT 24
#define FOUR_BYTE_SHIFT 32

#define ONE_BYTE_MASK 0xFF
#define TWO_BYTE_MASK 0xFFFF
#define THREE_BYTE_MASK 0xFFFFFF
#define FOUR_BYTE_MASK 0xFFFFFFFF

#define HEXTETS_IN_IPV6_ADDRESS 16
#define START_POINT_IN_READING_ADDRESS 15
/* ============================================== */

typedef struct duid_ll {
    uint16_t duid_type;
    uint16_t hw_type;
    uint8_t *mac;
} duid_ll_t;

typedef struct {
    uint16_t option_code;
    uint16_t option_length;
    union {
        struct client_id {
            duid_ll_t duid;
        } client_id_t;
        struct server_id {
            duid_ll_t duid;
        } server_id_t;
        struct ia_address {
            uint128_t ipv6_address;
            uint64_t preferred_lifetime;
            uint64_t valid_lifetime;
            union dhcpv6_option *ia_address_options;
        } ia_address_t;
        struct ia_na {
            uint32_t iaid;
            uint32_t t1;
            uint32_t t2;
            struct ia_address *addresses;
        } ia_na_t;
        struct option_request {
            uint8_t *option_request;
        } option_request_t;
        struct preference {
            uint8_t preference_value;
        } preference_t;
        struct elapsed_time {
            uint16_t elapsed_time_value;
        } elapsed_time_t;
        struct relay  {
            uint32_t relay_value;
        } relay_t;
        struct authentication {
            uint8_t protocol;
            uint8_t algorithm;
            uint8_t RDM;
            uint64_t replay_detection;
            uint128_t authentication_information;
        } authentication_t;
        struct unicast {
            uint128_t address;
        } unicast_t;
        struct status_code {
            uint16_t status_code;
        } status_code_t;
        struct user_class_option {
            uint128_t user_class_data;
        } user_class_option_t;
        struct vendor_class_option {
            uint32_t enterprise_number;
            uint128_t vendor_class_data;
        } vendor_class_option_t;
        struct vendor_specific_option {
            uint32_t enterprise_number;
            uint128_t vendor_option_data;
        } vendor_specific_option_t;
        struct interface_id {
            uint128_t interface_id_value;
        } interface_id_t;
        struct reconfigure_message {
            uint8_t msg_type;
        } reconfigure_message_t;
        struct ia_prefix {
            uint128_t ipv6_prefix;
            uint64_t preferred_lifetime;
            uint64_t valid_lifetime;
            uint8_t prefix_length;
        } ia_prefix_t;
        struct ia_pd {
            uint32_t iaid;
            uint32_t t1;
            uint32_t t2;
            struct ia_prefix_t *prefixes;
        } ia_pd_t;
        struct information_refresh_time {
            uint32_t information_refresh_time;
        } information_refresh_time_t;
        struct dns_recursive_name_server {
            uint8_t *dns_servers;
        } dns_recursive_name_server_t;
        struct domain_search_list {
            char *search_list;
        } domain_search_list_t;
        struct SOL_MAX_RT {
            uint32_t SOL_MAX_RT_value;
        } SOL_MAX_RT_t;
        struct INF_MAX_RT {
            uint32_t INF_MAX_RT_value;
        } INF_MAX_RT_t;
    };
} dhcpv6_option_t;

typedef struct dhcpv6_message {
    uint8_t message_type;
    uint32_t transaction_id;
    dhcpv6_option_t *option_list;
    uint8_t option_count;
    bool valid;
} dhcpv6_message_t;

typedef struct config {
    uint8_t reconfigure;
    bool rapid_commit;
    uint8_t *oro_list;
    uint8_t oro_list_length;
    bool na;
    bool pd;
} config_t;

#endif
