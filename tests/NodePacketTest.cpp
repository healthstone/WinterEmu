#include <catch2/catch_all.hpp>
#include <memory>
#include "packet/NodePacket.hpp"
#include "packet/NodeData.hpp"
#include "packet/WoWPacket.hpp"

TEST_CASE("NodePacket serialize/deserialize roundtrip with WoWPacket read", "[NodePacket]") {
    // Создаём NodeData
    NodeData nodeData;
    nodeData.write_uint8(7);           // NodeID
    nodeData.write_uint32_be(42);      // ClusterID

    // Создаём WoWPacket (например, с каким-то payload)
    WoWPacket wowPacket;
    wowPacket.write_uint32_le(123456); // example payload

    NodeOpcodes opcode = NodeOpcodes::NULL_PACKET;

    // Создаём NodePacket с opcode, NodeData и WoWPacket
    NodePacket nodePacket(opcode, nodeData, wowPacket);

    // Сериализуем в вектор байт
    std::vector<uint8_t> serialized = nodePacket.build_packet();

    // Создаём пустой NodePacket для десериализации
    NodePacket deserializedPacket;
    deserializedPacket.deserialize(serialized);

    // Проверяем opcode
    REQUIRE(deserializedPacket.opcode() == opcode);

    // Проверяем содержимое NodeData
    if (deserializedPacket.node_data()) {
        NodeData localCopy = deserializedPacket.node_data().value();
        REQUIRE(localCopy.serialize() == nodeData.serialize());
    }

    // Проверяем, что WoWPacket есть
    REQUIRE(deserializedPacket.wow_packet().has_value());
    REQUIRE(deserializedPacket.wow_packet().value().serialize() == wowPacket.serialize());

    uint32_t wowPayloadValue = 0;
    // Читаем uint32_t из десериализованного WoWPacket через локальную копию
    if (deserializedPacket.wow_packet()) {
        WoWPacket localCopy = deserializedPacket.wow_packet().value();
        wowPayloadValue = localCopy.read_uint32_le();
    }

    REQUIRE(wowPayloadValue == 123456);
    std::cout << "✅ 'NodePacket serialize/deserialize roundtrip with WoWPacket read' passed!\n";
}
