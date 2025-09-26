// Copyright 2025 XTX Markets Technologies Limited
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "Bincode.hpp"
#include "LogsDB.hpp"
#include "Msgs.hpp"
#include "MsgsGen.hpp"
#include "RegistryServer.hpp"
#include "Time.hpp"
#include <cstdint>


struct EntryReqIdx {
    LogIdx logIdx;
    uint8_t offset;
    bool operator ==(const EntryReqIdx&) const = default;
};


struct RegistryDBApplyEntryResult {
    LogIdx idx;
    std::vector<RegistryRespContainer> results;
};

template <>
struct std::hash<EntryReqIdx> {
    std::size_t operator()(const EntryReqIdx& k) const {
        return hash<uint64_t>()(k.logIdx.u64 ^ ((uint64_t)k.offset << 56));
    }
};

struct RegistryDBLogEntry {
public:

    TernTime entryTime;
    BincodeList<RegistryReqContainer> requests;

    static constexpr uint16_t STATIC_SIZE = sizeof(entryTime) + BincodeList<RegistryReqContainer>::STATIC_SIZE;

    RegistryDBLogEntry();
    size_t packedSize() const;

    void pack(BincodeBuf& buf) const;

    void unpack(BincodeBuf& buf);

    void clear();

    bool operator==(const RegistryDBLogEntry&rhs) const;

    static void requestsToLogEntries(std::vector<RegistryRequest>& requests,
        size_t maxLogEntrySize, TernTime entryTime, std::vector<LogsDBLogEntry>& out, std::vector<EntryReqIdx>& outIndices);
};


