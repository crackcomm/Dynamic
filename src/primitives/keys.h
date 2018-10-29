// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DYNAMIC_PRIMITIVES_KEYS_H
#define DYNAMIC_PRIMITIVES_KEYS_H

#include "chain/params.h"
#include "keys/key.h"
#include "keys/pubkey.h"
#include "script/script.h"
#include "script/standard.h"
#include "support/allocators/zeroafterfree.h"
#include "util/base58-data.h"

#include <string>
#include <vector>

/** base58-encoded Dynamic addresses.
 * Public-key-hash-addresses have version 76 (or 140 testnet).
 * The data vector contains RIPEMD160(SHA256(pubkey)), where pubkey is the serialized public key.
 * Script-hash-addresses have version 16 (or 19 testnet).
 * The data vector contains RIPEMD160(SHA256(cscript)), where cscript is the serialized redemption script.
 */
class CDynamicAddress : public CBase58Data
{
public:
    bool Set(const CKeyID& id);
    bool Set(const CScriptID& id);
    bool Set(const CTxDestination& dest);
    bool IsValid() const;
    bool IsValid(const CChainParams& params) const;

    CDynamicAddress() {}
    CDynamicAddress(const CTxDestination& dest) { Set(dest); }
    CDynamicAddress(const std::string& strAddress) { SetString(strAddress); }
    CDynamicAddress(const char* pszAddress) { SetString(pszAddress); }

    CTxDestination Get() const;
    bool GetKeyID(CKeyID& keyID) const;
    bool GetIndexKey(uint160& hashBytes, int& type) const;
    bool IsScript() const;
};

/**
 * A base58-encoded secret key
 */
class CDynamicSecret : public CBase58Data
{
public:
    void SetKey(const CKey& vchSecret);
    CKey GetKey();
    bool IsValid() const;
    bool SetString(const char* pszSecret);
    bool SetString(const std::string& strSecret);

    CDynamicSecret(const CKey& vchSecret) { SetKey(vchSecret); }
    CDynamicSecret() {}
};

template <typename K, int Size, CChainParams::Base58Type Type>
class CDynamicExtKeyBase : public CBase58Data
{
public:
    void SetKey(const K& key)
    {
        unsigned char vch[Size];
        key.Encode(vch);
        SetData(Params().Base58Prefix(Type), vch, vch + Size);
    }

    K GetKey()
    {
        K ret;
        if (vchData.size() == Size) {
            //if base58 encouded data not holds a ext key, return a !IsValid() key
            ret.Decode(&vchData[0]);
        }
        return ret;
    }

    CDynamicExtKeyBase(const K& key)
    {
        SetKey(key);
    }

    CDynamicExtKeyBase(const std::string& strBase58c)
    {
        SetString(strBase58c.c_str(), Params().Base58Prefix(Type).size());
    }

    CDynamicExtKeyBase() {}
};

typedef CDynamicExtKeyBase<CExtKey, BIP32_EXTKEY_SIZE, CChainParams::EXT_SECRET_KEY> CDynamicExtKey;
typedef CDynamicExtKeyBase<CExtPubKey, BIP32_EXTKEY_SIZE, CChainParams::EXT_PUBLIC_KEY> CDynamicExtPubKey;

#endif // DYNAMIC_PRIMITIVES_KEYS_H