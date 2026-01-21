#include "Level.h"
#include "Platform.h"
#include "Enemy.h"

#include <iostream>
#include <chrono>
#include <algorithm>

// ----------------------------
// Constructor / destructor
// ----------------------------
Level::Level(const std::string& root)
    : assetRoot(root)
{
    debugPrint = false; // set true to see spawn/cull logs
    rng.seed(static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    ));
    LoadTextures();
}

Level::~Level() {
    for (auto* p : platforms) delete p;
    for (auto* e : enemies) delete e;
    for (auto& pr : textures) delete pr.second;
}

// ----------------------------
// LoadTextures
// ----------------------------
void Level::LoadTextures() {
    for (auto& pr : textures) delete pr.second;
    textures.clear();
    platformTexNames.clear();

    auto loadTex = [this](const std::string& name, const std::string& folder) -> sf::Texture* {
        sf::Texture* tex = new sf::Texture();
        std::string path = assetRoot + "textures/" + folder + "/" + name + ".png";
        if (!tex->loadFromFile(path)) {
            delete tex;
            return nullptr;
        }
        tex->setSmooth(false);
        return tex;
    };

    // Weighted platform textures (ground more common)
    std::vector<std::pair<std::string, int>> platformWeights = {
        {"ground_1", 5}, {"ground_2", 4}, {"ground_3", 4},
        {"rock1", 1}, {"rock2", 1},
        {"trunk_1", 1}, {"trunk_2", 1}, {"trunk_3", 1}, {"trunk_4", 1}
    };

    for (auto &pw : platformWeights) {
        sf::Texture* t = loadTex(pw.first, "platforms");
        if (t) {
            textures[pw.first] = t;
            for (int i = 0; i < pw.second; ++i) platformTexNames.push_back(pw.first);
            if (debugPrint) std::cout << "[Level] loaded platform texture: " << pw.first << "\n";
        } else {
            if (debugPrint) std::cout << "[Level] texture not found: " << pw.first << "\n";
        }
    }

    // Enemy textures
    std::vector<std::string> enemyTextures = {"drowner", "ghoul", "wraith", "griffin"};
    for (auto &n : enemyTextures) {
        sf::Texture* t = loadTex(n, "enemies");
        if (t) {
            textures[n] = t;
            if (debugPrint) std::cout << "[Level] loaded enemy texture: " << n << "\n";
        } else {
            if (debugPrint) std::cout << "[Level] enemy texture not found: " << n << "\n";
        }
    }

    if (debugPrint) std::cout << "[Level] textures loaded=" << textures.size()
                               << " pool=" << platformTexNames.size() << "\n";
}

// ----------------------------
// Start streaming generation
// ----------------------------
void Level::startStreamingGeneration(float initialAheadDistance, int difficulty) {
    for (auto* p : platforms) delete p;
    platforms.clear();
    for (auto* e : enemies) delete e;
    enemies.clear();

    spawnAheadDistance = initialAheadDistance;
    lastGeneratedX = 0.f;

    // place an initial start platform
    float startWidth = std::max(800.f, minWidth * 4.f);
    float startHeight = std::min(maxHeight, 160.f);
    float startTop = worldFloor - startHeight;
    platforms.push_back(new Platform(0.f, startTop, startWidth, startHeight,
                                     textures.count("ground_1") ? textures["ground_1"] : nullptr));
    lastGeneratedX = 0.f + startWidth;
    lastPlatformTopY = startTop;

    // prefill - spawn until we reach spawnAheadDistance
    while (lastGeneratedX < spawnAheadDistance) {
        spawnPlatformAtX(lastGeneratedX);
    }

    if (debugPrint) std::cout << "[Level] prefilled up to X=" << lastGeneratedX
                               << " platforms=" << platforms.size() << "\n";
}

// Backwards compat
void Level::generateRandomLevel(float prefillAheadDistance, int difficulty) {
    startStreamingGeneration(prefillAheadDistance, difficulty);
}

// ----------------------------
// spawnPlatformAtX
// ----------------------------
void Level::spawnPlatformAtX(float baseRightX) {
    std::uniform_real_distribution<float> gapDist(minGap, maxGap);
    std::uniform_real_distribution<float> widthDist(minWidth, maxWidth);
    std::uniform_real_distribution<float> heightDist(minHeight, maxHeight);

    float gap = gapDist(rng);
    float width = widthDist(rng);
    float height = heightDist(rng);

    float preferredTop;
    if (lastPlatformTopY < 0.f) {
        preferredTop = worldFloor - height;
    } else {
        std::uniform_real_distribution<float> stepDist(-maxStepY, maxStepY);
        preferredTop = lastPlatformTopY + stepDist(rng);
    }

    float maxTop = worldFloor - minHeight - maxPlatformTopOffsetFromFloor;
    float minTop = minPlatformTop;
    if (preferredTop < minTop) preferredTop = minTop;
    if (preferredTop > maxTop) preferredTop = maxTop;

    // SWAMP LEVEL: slightly lower platforms
    float swampOffset = 20.f;
    preferredTop += swampOffset;

    float x = baseRightX + gap;
    float y = preferredTop;

    // -----------------------------
    // CHOOSE TEXTURE WITH CLUSTERING
    // -----------------------------
    sf::Texture* tex = nullptr;
    std::string texName = "none";

    if (!platformTexNames.empty()) {
        // 25% chance to repeat previous texture
        std::uniform_int_distribution<int> repeatChance(0, 99);
        if (!lastTextureName.empty() && repeatChance(rng) < 25) {
            texName = lastTextureName;
        } else {
            std::uniform_int_distribution<size_t> idx(0, platformTexNames.size() - 1);
            texName = platformTexNames[idx(rng)];
        }

        auto it = textures.find(texName);
        if (it != textures.end()) tex = it->second;
        lastTextureName = texName;
    }

    Platform* p = new Platform(x, y, width, height, tex);
    platforms.push_back(p);

    lastPlatformTopY = y;
    lastGeneratedX = x + width;

    if (debugPrint) {
        auto b = p->getGlobalBounds();
        std::cout << "[Level] spawn idx=" << (platforms.size()-1)
                  << " x=" << b.left << " y=" << b.top
                  << " w=" << b.width << " h=" << b.height
                  << " tex=" << texName << " nextGenX=" << lastGeneratedX << "\n";
    }

    trySpawnEnemiesOnPlatform(p);
}

// ----------------------------
// trySpawnEnemiesOnPlatform (fixed)
// ----------------------------
void Level::trySpawnEnemiesOnPlatform(Platform* p) {
    std::uniform_int_distribution<int> chance(0, 99);
    if (chance(rng) >= 16) return; // 16% chance to spawn

    // Collect available enemy textures
    std::vector<std::string> avail;
    for (auto name : {"drowner", "ghoul", "wraith", "griffin"}) {
        if (textures.count(name)) avail.push_back(name);
    }
    if (avail.empty()) return;

    std::uniform_int_distribution<size_t> idx(0, avail.size() - 1);
    std::string chosen = avail[idx(rng)];
    sf::Texture* tex = textures[chosen];

    sf::FloatRect b = p->getGlobalBounds();
    float ex = b.left + b.width * 0.5f;
    float ey = b.top - 48.f; // slightly above platform

    ENEMY_TYPE type = (chosen == "drowner") ? DROWNER :
                      (chosen == "ghoul")   ? GHOUL :
                      (chosen == "wraith")  ? WRAITH : GRIFFIN;

    // Get difficulty multiplier from Settings
    float multiplier = 1.f;
    if (settings) multiplier = settings->getDifficultyMultiplier();

    // Spawn enemy with multiplier applied via constructor
    Enemy* e = new Enemy(ex, ey, type, tex, b.left, b.left + b.width, multiplier);
    enemies.push_back(e);

    if (debugPrint) 
        std::cout << "[Level] spawned enemy " << chosen << " at " << ex << "," << ey << "\n";
}


// ----------------------------
// ensurePlatformsAhead
// ----------------------------
void Level::ensurePlatformsAhead(float playerX) {
    while (lastGeneratedX < playerX + spawnAheadDistance) {
        spawnPlatformAtX(lastGeneratedX);
        static int guard = 0;
        if (++guard > 5000) {
            if (debugPrint) std::cerr << "[Level] ensurePlatformsAhead guard triggered\n";
            break;
        }
    }
}

// ----------------------------
// cullPlatformsBehind
// ----------------------------
void Level::cullPlatformsBehind(float playerX) {
    float cutoff = playerX - despawnBehindDistance;
    size_t kept = 0;
    for (size_t i = 0; i < platforms.size(); ++i) {
        sf::FloatRect b = platforms[i]->getGlobalBounds();
        float right = b.left + b.width;
        if (right < cutoff) {
            if (debugPrint) std::cout << "[Level] cull idx=" << i << " right=" << right << "\n";
            delete platforms[i];
            platforms[i] = nullptr;
        } else {
            if (i != kept) platforms[kept] = platforms[i];
            ++kept;
        }
    }
    if (kept < platforms.size()) platforms.erase(platforms.begin() + kept, platforms.end());

    // Cull enemies similarly
    size_t keptE = 0;
    for (size_t i = 0; i < enemies.size(); ++i) {
        sf::FloatRect eb = enemies[i]->getGlobalBounds();
        float right = eb.left + eb.width;
        if (right < cutoff) {
            delete enemies[i];
            enemies[i] = nullptr;
        } else {
            if (i != keptE) enemies[keptE] = enemies[i];
            ++keptE;
        }
    }
    if (keptE < enemies.size()) enemies.erase(enemies.begin() + keptE, enemies.end());
}

// ----------------------------
// update
// ----------------------------
void Level::update(const sf::Vector2f& playerPos, const sf::View& view) {
    float playerX = playerPos.x;
    ensurePlatformsAhead(playerX);
    cullPlatformsBehind(playerX);

    float updateMargin = 500.f;
    sf::FloatRect updateBounds(
        view.getCenter().x - view.getSize().x / 2.f - updateMargin,
        view.getCenter().y - view.getSize().y / 2.f - updateMargin,
        view.getSize().x + updateMargin * 2.f,
        view.getSize().y + updateMargin * 2.f
    );

    for (auto* enemy : enemies) {
        if (!enemy) continue;
        if (enemy->getGlobalBounds().intersects(updateBounds)) {
            enemy->updatePlayerPosition(playerPos);
            enemy->update();
        }
    }
}

// ----------------------------
// render
// ----------------------------
void Level::render(sf::RenderTarget& target) {
    for (auto* p : platforms) if (p) p->render(target);
    for (auto* e : enemies) if (e) e->render(target);
}

// ----------------------------
// accessors
// ----------------------------
const std::vector<Platform*>& Level::getPlatforms() const { return platforms; }
const std::vector<Enemy*>& Level::getEnemies() const { return enemies; }
