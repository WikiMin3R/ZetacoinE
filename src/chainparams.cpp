// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2012-2018 Zetacoin developers
// Copyright (c) 2018-2021 EWMCI,LLC.
// Copyright (c) 2018-2021 InfiniLooP developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x66;
        pchMessageStart[1] = 0x46;
        pchMessageStart[2] = 0x20;
        pchMessageStart[3] = 0x04;
        vAlertPubKey = ParseHex("04c252b2a1a302e1662cb1b2d5eac900bbe932549ff34d815e3423d7127b5df91ce3bf854a3b45fa7bdde271ccf0fd39647e88edf0cdbe7e40e9a3f89cf6acec88");
        nDefaultPort = 22011;
        nRPCPort = 22014;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        //
        //CBlock(hash=000001faef25dec4fbcf906e6242621df2c183bf232f263d0ba5b101911e4563, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=12630d16a97f24b287c8c2594dda5fb98c9e6c70fc61d44191931ea2aa08dc90, nTime=1393221600, nBits=1e0fffff, nNonce=164482, vtx=1, vchBlockSig=)
        //  Coinbase(hash=12630d16a9, nTime=1393221600, ver=1, vin.size=1, vout.size=1, nLockTime=0)
        //    CTxIn(COutPoint(0000000000, 4294967295), coinbase 00012a24323020466562203230313420426974636f696e2041544d7320636f6d6520746f20555341)
        //    CTxOut(empty)
        //  vMerkleTree: 12630d16a9
        const char* pszTimestamp = "Zetacoin enters sustainable, energy-efficient, PoS future as SpaceX launches 53 Starlink internet satellites and lands rocket in foggy night!";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, 1636837314, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1636837314;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 1967387;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000005cfd8a9c70423a97a448fc06a96de06d48f664f2eb5546c6d3817b7a348"));
        assert(genesis.hashMerkleRoot == uint256("0xa715a22657b85ad56eb1913702dac8dc73f33738518d710725e3fc6634e7512d"));

        // List of SeedNodes - The latter three will be replaced as we transition the SPV network away from Legacy Zetacoin
		vSeeds.push_back(CDNSSeedData("zeta-seed-a.zetacoin.info", "207.180.252.200"));
	        vSeeds.push_back(CDNSSeedData("zeta-seed-b.zetacoin.biz", "176.57.189.38"));
	//	vSeeds.push_back(CDNSSeedData("secondary-zet.transcenders.ws", "207.180.208.187"));        
	//	vSeeds.push_back(CDNSSeedData("tertiary-zet.transcenders.ws", "95.111.247.164"));
	//    	vSeeds.push_back(CDNSSeedData("zet.electrumx.transcenders.name", "62.171.161.79"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 20);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 85);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, 153);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nLastPOWBlock = 9999999999999999;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xab;
        pchMessageStart[1] = 0xc3;
        pchMessageStart[2] = 0xd6;
        pchMessageStart[3] = 0xe9;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        vAlertPubKey = ParseHex("0458f16fadf9e6c5806b9031243a8f3e0640b94c31b1b9d4b891934bdc8d5c3e8f60d0ff9fb7c16a8d8ec327d9cd8824f05144034ba713980a93cdb7bc2702c39b");
        nDefaultPort = 22012;
        nRPCPort = 22015;
        strDataDir = "testnet";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 18714;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x0000e18c3a5fc8ee0d9ed848fb8a51ef88cb32025aa8b4097ce1a62534397ee0"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 83);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nLastPOWBlock = 0x7fffffff;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;

//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xc2;
        pchMessageStart[3] = 0xd2;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1636837314;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 1;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 22013;
        strDataDir = "regtest";
        assert(hashGenesisBlock == uint256("0x60d605a055653c39e1bbc86ebb5a84a615b88f4070e36c4d6e8265f6ca98e57e"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
