#include <cstring>
#include <vector>

#include "CoAPMessage.h"
#include "Log.h"

static Logger& logger = Logger::get_instance();

COAPMessage::COAPMessage() 
    :m_version(1),
    m_type(Type::CON),
    m_code(Code::GET),
    m_message_id(0x1111) {

}

bool COAPMessage::parse(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 4) {
        logger.log(ERROR, "Invalid CoAP message size");
        return false;
    }

    // 解析 CoAP 消息头
    m_version = (buffer[0] >> 6) & 0x03;
    if (m_version != 0b01) {
        logger.log(ERROR, "get the wrong version of message");
        return false;
    }

    m_type = static_cast<Type>((buffer[0] >> 4) & 0x03);
    m_token_length = buffer[0] & 0x0F;
    m_code = static_cast<Code>(buffer[1]);
    m_message_id = (buffer[2] << 8) | buffer[3];

    // 解析 Token
    if (m_token_length > 0) {
        if (buffer.size() < (size_t)(4 + m_token_length)) {
            logger.log(ERROR, "Invalid CoAP message: token length mismatch");
            return false;
        }
        m_token.assign(buffer.begin() + 4, buffer.begin() + 4 + m_token_length);
    }

    // 解析 Options
    size_t option_index = 4 + m_token_length;
    uint16_t running_delta = 0;

    while (option_index < buffer.size()) {
        uint8_t option_header = buffer[option_index];
        uint16_t option_delta = (option_header >> 4) & 0x0F;
        uint16_t option_length = option_header & 0x0F;

        if ((option_delta == 15) && (option_length == 15)) {
            break;
        }

        option_index++;

        if (option_delta == 13) {
            option_delta = 13 + buffer[option_index];
            option_index++;
        } else if (option_delta == 14) {
            option_delta = 269 + (buffer[option_index] << 8) + buffer[option_index + 1];
            option_index += 2;
        }

        if (option_length == 13) {
            option_length = 13 + buffer[option_index];
            option_index++;
        } else if (option_length == 14) {
            option_length = 269 + (buffer[option_index] << 8) + buffer[option_index + 1];
            option_index += 2;
        }

        running_delta += option_delta;

        std::vector<uint8_t> option_value(buffer.begin() + option_index, buffer.begin() + option_index + option_length);
        option_index += option_length;

        // 处理 Uri-Path 选项
        if (running_delta == 11) { // Uri-Path 的 Delta 值为 11
            std::string path_segment(option_value.begin(), option_value.end());
            m_uri_path += "/" + path_segment;
        }

        // 存储所有选项，无论是否为 Uri-Path
        m_options[running_delta] = option_value;
    }

    // 处理 Payload
    if (option_index < buffer.size() && buffer[option_index] == 0xFF) {
        option_index++;
        m_payload_size = buffer.size() - option_index;
        if (option_index < buffer.size()) {
            m_payload.assign(buffer.begin() + option_index, buffer.end());
        }
    } else {
        m_payload_size = 0;
    }

    return true;
}

// bool COAPMessage::parse(const std::vector<uint8_t>& buffer) {
//     if (buffer.size() < 4) {
//         logger.log(ERROR, "Invalid CoAP message size");
//         return false;
//     }

//     // 解析 CoAP 消息头
//     m_version = (buffer[0] >> 6) & 0x03;
//     if(m_version != 0b01) {
//         logger.log(ERROR, "get the wrong version of message");
//         return false;
//     }

//     m_type = static_cast<Type>((buffer[0] >> 4) & 0x03);
//     m_token_length = buffer[0] & 0x0F;
//     m_code = static_cast<Code>(buffer[1]);
//     m_message_id = (buffer[2] << 8) | buffer[3];

//     // 解析 Token
//     if(m_token_length > 0) {
//         if (buffer.size() < (size_t)(4 + m_token_length)) {
//             logger.log(ERROR, "Invalid CoAP message: token length mismatch");
//             return false;
//         }
//         m_token.assign(buffer.begin() + 4, buffer.begin() + 4 + m_token_length);
//     }

//     // 解析 Options
//     size_t option_index = 4 + m_token_length;
//     while(option_index < buffer.size()) {
//         uint16_t option_delta = ((buffer[option_index] >> 4) & 0x0F);
//         uint16_t option_length = buffer[option_index] & 0x0F;

//         if((option_delta == 15) && (option_length == 15)) {
//             break;
//         }

//         if((option_delta == 15) || (option_length == 15)) {
//             logger.log(ERROR, "get the wrong option message");
//             return false;
//         }

//         if ((uint16_t)option_delta == 13) {
//             option_delta = (buffer[option_index + 1] + 13);
//             option_index += 1;
//         } else if (option_delta == 14) {
//             option_delta = (buffer[option_index + 1] << 8) + buffer[option_index + 2] + 269;
//             option_index += 2;
//         }

//         if (option_length == 13) {
//             option_length = buffer[option_index + 1] + 13;
//             option_index += 1;
//         } else if (option_length == 14) {
//             option_length = (buffer[option_index + 1] << 8) + buffer[option_index + 2] + 269;
//             option_index += 2;
//         }

//         std::vector<uint8_t> option_value(buffer.begin() + option_index + 1, buffer.begin() + option_index + 1 + option_length);
//         m_options[option_delta] = option_value;

//         option_index += 1 + option_length;
//     }

//     if(buffer.size() > option_index) {
//         if(buffer[option_index] != 0xff) {
//             logger.log(ERROR, "get the wrong message format");
//             return false;
//         }
//     }

//     option_index++;

//     // 解析 Payload
//     m_payload_size = buffer.size() - option_index;
//     if (option_index < buffer.size()) {
//         m_payload.assign(buffer.begin() + option_index, buffer.end());
//     }

//     return true;
// }

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
    int option_size = 0;
    for(const auto& option: m_options) {
        int delta = static_cast<int>(option.first);
        int length = option.second.size();
        option_size += 1;
        // delta length
        if(delta > 269) option_size += 2;
        else if(delta > 13) option_size += 1;
        else option_size = option_size;
        // length length
        if(length > 269) option_size += 2;
        else if(length > 13) option_size += 1;
        else option_size = option_size;
        option_size += option.second.size();
    }
    if(m_payload_size != 0)
        m_length = 4 + m_token_length + 1 + m_payload_size + option_size;
    else
        m_length = 4 + m_token_length + option_size;
    char* mesg = (char*)malloc(sizeof(uint8_t) * (m_length));
    mesg[pointer++] = (m_version << 6) | ((uint8_t)m_type << 4) | (m_token_length & 0x0f);
    mesg[pointer++] = (uint8_t)m_code;
    mesg[pointer++] = (m_message_id >> 8) & 0xff;
    mesg[pointer++] = (m_message_id & 0xff);
    if(m_token_length > 0) {
        for(int i = 0; i < m_token_length; i++) {
            mesg[pointer++] = m_token[i];
        }
    }
    for(const auto& option: m_options) {
        uint16_t delta = static_cast<int>(option.first);
        uint16_t delta_hout = ((delta - 269) >> 8);
        uint16_t delta_lout = (delta - 269);
        uint16_t length = option.second.size();
        uint16_t length_hout = ((length - 269) >> 8);
        uint16_t length_lout = (length - 269);
        int head_pointer = pointer++;
        // delta length
        if(delta > 269) {
            mesg[head_pointer] = (0x0e << 4) & 0xf0;
            mesg[pointer++] = (delta_hout) & 0xff;
            mesg[pointer++] = (delta_lout) & 0xff;
        } else if(delta > 13) {
            mesg[head_pointer] = (0x0d << 4) & 0xf0;
            mesg[pointer++] = (delta - 13) & 0xff;
        } else {
            mesg[head_pointer] = (delta << 4) & 0xf0;
        }
        // length length
        if(length > 269) {
            mesg[head_pointer] |= (0x0e) & 0x0f;
            mesg[pointer++] = (length_hout) & 0xff;
            mesg[pointer++] = (length_lout) & 0xff;
        } else if(length > 13) {
            mesg[head_pointer] |= (0x0d) & 0x0f;
            mesg[pointer++] = (length - 13) & 0xff;
        } else {
            mesg[head_pointer] = mesg[head_pointer] | (length & 0x0f);
        }
        for(int i = 0; i < (int)option.second.size(); i++) {
            mesg[pointer++] = option.second[i];
        }
    }
    if(m_payload_size != 0) {
        mesg[pointer++] = 0xff;
        for(int i = 0; i < m_payload_size; i++) {
            mesg[pointer++] = m_payload[i];
        }
    }
    return mesg;
}

bool COAPMessage::add_option(uint16_t delta, uint16_t length, uint8_t* option) {
    std::vector<uint8_t> option_v;
    option_v.insert(option_v.end(), option, option + length);
    m_options[delta] = option_v;
    return 1;
}

bool COAPMessage::isCon() {
    if(m_type == Type::CON) {
        return true;
    } else {
        return false;
    }
}

bool COAPMessage::isACK() {
    if(m_type == Type::ACK) {
        return true;
    } else {
        return false;
    }
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
        std::cout << "  Option Delta: " << (int)static_cast<int>(option.first) << std::endl;
        std::cout << "  Option Value: ";
        for (uint8_t byte : option.second) {
            std::cout << (char)byte;
            // std::cout << std::hex << static_cast<int>(byte) << " ";
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