#include <cstring>
#include <vector>

#include "CoAPMessage.h"
#include "Log.h"

static Logger& logger = Logger::get_instance();

COAPMessage::COAPMessage() 
    :m_version(1),
    m_type(Type::CON),
    m_code(Code::GET),
    m_message_id(0) {

}

bool COAPMessage::parse(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 4) {
        std::cerr << "Invalid CoAP message size" << std::endl;
        return false;
    }

    // 解析 CoAP 消息头
    m_version = (buffer[0] >> 6) & 0x03;
    if(m_version != 0b01) {
        logger.log(ERROR, "get the wrong version of message");
        return false;
    }

    m_type = static_cast<Type>((buffer[0] >> 4) & 0x03);
    m_token_length = buffer[0] & 0x0F;
    m_code = static_cast<Code>(buffer[1]);
    m_message_id = (buffer[2] << 8) | buffer[3];

    // 解析 Token
    if(m_token_length > 0) {
        if (buffer.size() < (size_t)(4 + m_token_length)) {
            logger.log(ERROR, "Invalid CoAP message: token length mismatch");
            return false;
        }
        m_token.assign(buffer.begin() + 4, buffer.begin() + 4 + m_token_length);
    }

    // 解析 Options
    size_t option_index = 4 + m_token_length;
    while(option_index < buffer.size()) {
        uint8_t option_delta = (buffer[option_index] >> 4) & 0x0F;
        uint8_t option_length = buffer[option_index] & 0x0F;

        if((option_delta == (uint8_t)15) & (option_index == (uint8_t)15)) {
            break;
        }

        if((option_delta == (uint8_t)15) | (option_index == (uint8_t)15)) {
            logger.log(ERROR, "get the wrong option message");
            return false;
        }

        if (option_delta == 13) {
            option_delta = buffer[option_index + 1] + 13;
            option_index += 1;
        } else if (option_delta == 14) {
            option_delta = (buffer[option_index + 1] << 8) + buffer[option_index + 2] + 269;
            option_index += 2;
        }

        if (option_length == 13) {
            option_length = buffer[option_index + 1] + 13;
            option_index += 1;
        } else if (option_length == 14) {
            option_length = (buffer[option_index + 1] << 8) + buffer[option_index + 2] + 269;
            option_index += 2;
        }

        std::vector<uint8_t> option_value(buffer.begin() + option_index + 1, buffer.begin() + option_index + 1 + option_length);
        m_options[option_delta] = option_value;

        option_index += 1 + option_length;
    }

    if(buffer.size() > option_index) {
        if(buffer[option_index] != 0xff) {
            logger.log(ERROR, "get the wrong message format");
            return false;
        }
    }

    // 解析 Payload
    if (option_index < buffer.size()) {
        m_payload.assign(buffer.begin() + option_index, buffer.end());
    }

    return true;
}

void COAPMessage::make(Type type, char* token, size_t token_length, 
            Code code, uint16_t message_id, uint8_t* data, ssize_t data_size) {
    m_version = 0x01;
    m_type = type;
    m_token_length = token_length;
    m_code = code;
    m_message_id = message_id;
    if(token_length > 0) 
        m_token.insert(m_token.end(), token, token + strlen(token));
    if(data_size > 0)
        m_payload.insert(m_payload.end(), data, data + data_size);
    m_payload_size = data_size;
}

char* COAPMessage::format() {
    int pointer = 0;
    m_length = 4 + m_token_length + 1 + m_payload_size;
    char* mesg = (char*)malloc(sizeof(uint8_t) * (4 + m_token_length + 1 + m_payload_size));
    mesg[pointer++] = (m_version << 6) | ((uint8_t)m_type << 4) | (m_token_length & 0x0f);
    mesg[pointer++] = (uint8_t)m_code;
    mesg[pointer++] = (m_message_id >> 8) & 0x0f;
    mesg[pointer++] = (m_message_id & 0x0f);
    if(m_token_length > 0) {
        for(int i = 0; i < m_token_length; i++) {
            mesg[pointer++] = m_token[i];
        }
    }
    mesg[pointer++] = 0xff;
    for(int i = 0; i < m_payload_size; i++) {
        mesg[pointer++] = m_payload[i];
    }
    return mesg;
}

void COAPMessage::print() {
    std::cout << "Version: " << static_cast<int>(m_version) << std::endl;
    std::cout << "Type: " << static_cast<int>(m_type) << std::endl;
    std::cout << "Token length: " << static_cast<int>(m_token_length) << std::endl;
    std::cout << "Code: " << static_cast<int>(m_code) << std::endl;
    std::cout << "Message ID: " << m_message_id << std::endl;
    std::cout << "Token: ";
    for (uint8_t byte : m_token) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    // 打印 Options
    std::cout << "Options: " << std::endl;
    for (const auto& option : m_options) {
        std::cout << "  Option Delta: " << static_cast<int>(option.first) << std::endl;
        std::cout << "  Option Value: ";
        for (uint8_t byte : option.second) {
            std::cout << std::hex << static_cast<int>(byte) << " ";
        }
        std::cout << std::endl;
    }

    // 打印 Payload
    std::cout << "Payload: ";
    for (uint8_t byte : m_payload) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;
}