#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../test.h"

#include "trilogy/error.h"
#include "trilogy/protocol.h"

static uint8_t error_packet[] = {
    0xff, 0x28, 0x04, 0x23, 0x34, 0x32, 0x30, 0x30, 0x30, 0x59, 0x6f, 0x75, 0x20, 0x68, 0x61, 0x76, 0x65, 0x20,
    0x61, 0x6e, 0x20, 0x65, 0x72, 0x72, 0x6f, 0x72, 0x20, 0x69, 0x6e, 0x20, 0x79, 0x6f, 0x75, 0x72, 0x20, 0x53,
    0x51, 0x4c, 0x20, 0x73, 0x79, 0x6e, 0x74, 0x61, 0x78, 0x3b, 0x20, 0x63, 0x68, 0x65, 0x63, 0x6b, 0x20, 0x74,
    0x68, 0x65, 0x20, 0x6d, 0x61, 0x6e, 0x75, 0x61, 0x6c, 0x20, 0x74, 0x68, 0x61, 0x74, 0x20, 0x63, 0x6f, 0x72,
    0x72, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x64, 0x73, 0x20, 0x74, 0x6f, 0x20, 0x79, 0x6f, 0x75, 0x72, 0x20, 0x4d,
    0x79, 0x53, 0x51, 0x4c, 0x20, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f,
    0x6e, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x74, 0x68, 0x65, 0x20, 0x72, 0x69, 0x67, 0x68, 0x74, 0x20, 0x73, 0x79,
    0x6e, 0x74, 0x61, 0x78, 0x20, 0x74, 0x6f, 0x20, 0x75, 0x73, 0x65, 0x20, 0x6e, 0x65, 0x61, 0x72, 0x20, 0x27,
    0x53, 0x45, 0x4c, 0x43, 0x54, 0x20, 0x31, 0x27, 0x20, 0x61, 0x74, 0x20, 0x6c, 0x69, 0x6e, 0x65, 0x20, 0x31};

TEST test_parse_error_packet()
{
    trilogy_err_packet_t packet;

    uint32_t flags = TRILOGY_CAPABILITIES_PROTOCOL_41;

    int err = trilogy_parse_err_packet(error_packet, sizeof(error_packet), flags, &packet);
    ASSERT_OK(err);

    ASSERT_EQ(1064, packet.error_code);
    ASSERT_EQ(0x23, packet.sql_state_marker[0]);

    const char expected_state[] = "42000";
    ASSERT_MEM_EQ(packet.sql_state, expected_state, sizeof(packet.sql_state));

    const char expected_message[] = "You have an error in your SQL syntax; check the manual that "
                                    "corresponds to your MySQL server version for the right syntax to use "
                                    "near 'SELCT 1' at line 1";
    ASSERT_MEM_EQ(packet.error_message, expected_message, packet.error_message_len);

    PASS();
}

TEST test_parse_error_packet_truncated()
{
    trilogy_err_packet_t packet;

    uint32_t flags = TRILOGY_CAPABILITIES_PROTOCOL_41;

    int err = trilogy_parse_err_packet(error_packet, sizeof(error_packet) - 154, flags, &packet);
    ASSERT_ERR(TRILOGY_TRUNCATED_PACKET, err);

    PASS();
}

int parse_error_packet_test()
{
    RUN_TEST(test_parse_error_packet);
    RUN_TEST(test_parse_error_packet_truncated);

    return 0;
}