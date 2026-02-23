#include "pch.hpp"
#include "music_manager.hpp"
#include "scuffcraft.hpp"
#include "audio/audio_engine.hpp"
#include <filesystem>
#include <random>

void MusicManager::Init(const std::string &musicDir)
{
    for (const auto &entry : std::filesystem::directory_iterator(musicDir))
    {
        if (entry.is_regular_file())
        {
            std::string path = entry.path().string();
            m_MusicHandles.push_back(Scuffcraft::Get().GetAssetManager().LoadAsset(path, AssetType::Audio));
        }
    }

    LOG("Initialized MusicManager with %zu tracks", m_MusicHandles.size());
}

void MusicManager::Start()
{
    if (m_MusicHandles.empty())
        return;

    m_ShuffledOrder.resize(m_MusicHandles.size());
    std::iota(m_ShuffledOrder.begin(), m_ShuffledOrder.end(), 0);
    std::shuffle(m_ShuffledOrder.begin(), m_ShuffledOrder.end(), std::mt19937{std::random_device{}()});

    m_CurrentIndex = 0;
    PlayNext();
}

void MusicManager::Stop()
{
    AudioEngine::StopMusic();
}

void MusicManager::Update()
{
    if (AudioEngine::GetMusicState() == AudioAPI::SourceState::Stopped)
    {
        PlayNext();
    }
}

void MusicManager::PlayNext()
{
    if (m_MusicHandles.empty())
        return;

    AssetHandle handle = m_MusicHandles[m_ShuffledOrder[m_CurrentIndex]];
    Ref<Audio> music = Scuffcraft::Get().GetAssetManager().GetAsset<Audio>(handle);
    AudioEngine::PlayMusic(music);

    m_CurrentIndex = (m_CurrentIndex + 1) % m_MusicHandles.size();
}