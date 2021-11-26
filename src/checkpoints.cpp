// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2018-2021 The InfiniLooP developers
// Copyright (c) 2020-2021 The Zetacoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 500;

 namespace Checkpoints
 {
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0000,  uint256("0x000005cfd8a9c70423a97a448fc06a96de06d48f664f2eb5546c6d3817b7a348") ) // Genesis
        ( 175,   uint256("0x5521aa5c3f427a296e9eeb47e085b8915a65bf3498bc554e7fac4a080e6b130c") ) // End of pre-mining epoch
        ( 176,   uint256("0xdb38cacfda14205fa395652d2036a300a34324010206ebafd6f3f60f30844196") ) // Beginning of PoW + PoS epoch
        ( 1001,  uint256("0xbf072343bb3d1a715d4821e6532764b1822d548f42fc077b018d13f44ff25c8a") ) // Initial 12-hour period of sustained PoS
        ( 6001,  uint256("0x5541e26132973dec35b6b135e88a11a7c7f92fefd0365cf14cb3cff6f36692fe") ) // Blockchain no longer requires PoW for sustainability
    //  ( ABCDE, uint256("0xbf072343bb3d1a715d4821e6532764b1822d548f42fc077b018dx3y4zff25c8a") ) // July 1, 2022 formal decomissioning of old Zetacoin
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();

        if (nHeight <= pindexSync->nHeight)
            return false;
        return true;
    }
}
