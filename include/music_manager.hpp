#pragma once
#include <vector>
#include "core.hpp"
#include <string>
#include "asset/asset.hpp"

class MusicManager {
public:
    static void Init(const std::string& musicDir);
    static void Update();
    static void Start();
    static void Stop();

private:
    static void PlayNext();

    inline static std::vector<AssetHandle> m_MusicHandles;
    inline static std::vector<int> m_ShuffledOrder;
    inline static int m_CurrentIndex = 0;
};