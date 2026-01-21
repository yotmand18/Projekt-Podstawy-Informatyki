#pragma once

#include "lib.h"
#include "Settings.h"

class Platform;
class Enemy;

class Level {
public:
    explicit Level(const std::string& assetRoot);
    ~Level();

    // Core API
    bool loadFromFile(const std::string& filename);
    const std::vector<Platform*>& getPlatforms() const;
    const std::vector<Enemy*>& getEnemies() const;
    void render(sf::RenderTarget& target);
    void update(const sf::Vector2f& playerPos, const sf::View& view);
    float getWorldFloor() const { return worldFloor; }
    std::string lastTextureName;

    void setSettings(Settings* s) { settings = s; }

    // One-shot or streaming generation
    void generateRandomLevel(float prefillAheadDistance, int difficulty = 1); // backwards-compatible
    void startStreamingGeneration(float initialAheadDistance = 3000.f, int difficulty = 1);

    // Debug / introspection
    float getLastGeneratedX() const { return lastGeneratedX; }
    size_t getPlatformCount() const { return platforms.size(); }

private:
    Settings* settings = nullptr;

    // Helpers
    void LoadTextures();
    void spawnPlatformAtX(float baseRightX);
    void trySpawnEnemiesOnPlatform(Platform* p);
    void cullPlatformsBehind(float playerX);
    void ensurePlatformsAhead(float playerX);

    // Assets
    std::string assetRoot;
    std::unordered_map<std::string, sf::Texture*> textures;

    // World objects (raw pointers for compatibility with your codebase)
    std::vector<Platform*> platforms;
    std::vector<Enemy*> enemies;

    // Streaming state
    float lastGeneratedX = 0.f;            // rightmost X we've generated up to
    float spawnAheadDistance = 3000.f;    // keep this far ahead of player
    float despawnBehindDistance = 800.f;  // cull platforms that end this far behind player

    // RNG
    std::mt19937 rng;

    // Texture pool (flattened weighted names)
    std::vector<std::string> platformTexNames;

    // Generation tuning (tweakable)
    float minGap = 140.f;
    float maxGap = 380.f;
    float minWidth = 120.f;
    float maxWidth = 340.f;
    float minHeight = 40.f;
    float maxHeight = 180.f;

    // Maximum vertical change between consecutive platforms (so jumps remain reachable)
    float maxStepY = 110.f;

    // Absolute vertical bounds for platform top (so platforms don't fly off-screen)
    float minPlatformTop = 120.f; // don't place platforms higher than this (Y smaller = higher)
    float maxPlatformTopOffsetFromFloor = 160.f; // min distance above bottom to place top (so platforms don't touch floor)

    // World floor (platforms sit so that bottom ~= worldFloor)
    float worldFloor = 900.f; // default -- tune to your game's coordinate system

    // Last top Y used (helps keep vertical moves reasonable)
    float lastPlatformTopY = -1.f;

    // Debug
    bool debugPrint = false;
};
