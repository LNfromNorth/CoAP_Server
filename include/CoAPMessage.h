#ifndef __COAP_SERVER_COAP_MESSAGE_
#define __COAP_SERVER_COAP_MESSAGE_

#include <iostream>
#include <vector>
#include <map>

class COAPMessage {
public:
    enum class Type: uint8_t {
        CON = 0,
        NON = 1,
        ACK = 2,
        RST = 3
    };

    enum class Code : uint8_t {
        EMPTY               = 0,
        // request
        GET                 = 1,
        POST                = 2,
        PUT                 = 3,
        DELETE              = 4,
        // response
        /* success */
        Created             = 0x41,
        Delete              = 0x42,
        Valid               = 0x43,
        Changed             = 0x44,
        Content             = 0x45,
        /* Client Error */
        Bad_Request         = 0x80,
        Unauthorized        = 0x81,
        Bad_Option          = 0x82,
        Forbidden           = 0x83,
        Not_Fount           = 0x84,
        Method_Not_Allowed  = 0x85,
        Not_Acceptable      = 0x86,
        Precondition_Failed = 0x8c,
        Unsupper_content_T  = 0x8f,
        /* Server Error */
        Internal_Server_Err = 0xa0,
        Not_Implemented     = 0xa1,
        Bad_Gateway         = 0xa2,
        Service_Unavailable = 0xa3,
        Gateway_Timeout     = 0xa4,
        Proxying_Not_Sup    = 0xa5
    };

    enum class Option: uint16_t {
        Uri_Host            = 3,
        ETag                = 4,
        If_Match            = 5,
        Observe             = 6,
        Uri_Port            = 7,
        Location_Path       = 8,
        Uri_Path            = 11,
        Content_Format      = 12,
        Max_Age             = 14,
        Uri_Query           = 15,
        Accept              = 17,
        Location_Query      = 20,
        Block1              = 23,
        Block2              = 27,
        Size1               = 28,
        Proxy_Uri           = 35,
        Proxy_Scheme        = 39,
        Size2               = 60
    };


    COAPMessage();

    bool parse(const std::vector<uint8_t>& buffer);
    char* format();

    void make(Type type, char* token, size_t token_length, 
            Code code, uint16_t message_id, uint8_t* data, ssize_t data_size);

    bool add_option(uint16_t delta, uint16_t length, uint8_t* option);

    void print();
    bool isCon();
    bool isACK();

    char* get_code2string() {
        if(m_type == Type::CON) return (char*)"CON";
        else if(m_type == Type::NON) return (char*)"NON";
        else if(m_type == Type::ACK) return (char*)"ACK";
        else return (char*)"RST";
    }
    int get_size() { return m_length;}
    uint8_t get_tokenl() { return m_token_length;}
    char* get_token() { return (char*)&m_token[0];}
    uint16_t get_msgid() { return m_message_id;}
    ssize_t get_data_size() { return m_payload_size;}
    char* get_data() {return (char*)&m_payload[0];}
private:
    uint8_t m_version;
    Type m_type;
    uint8_t m_token_length;
    Code m_code;
    uint16_t m_message_id;
    std::vector<uint8_t> m_token;
    std::map<uint16_t, std::vector<uint8_t>> m_options;
    ssize_t m_payload_size;
    std::vector<uint8_t> m_payload;
    ssize_t m_length;
};

#endif
