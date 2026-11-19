#pragma once

#include <array>
#include <memory>

#include <SFML/Graphics/Texture.hpp>

#include "include/buttonKind.h"
#include "include/config/assetTable.h"

class AssetCache {
private:
    struct Slot {
        std::unique_ptr<sf::Texture> tex;      // 空 = 没有/加载失败
        bool warned = false;
    };
    static void loadInto(Slot& slot, const AssetSpec& spec);

    std::array<Slot, kButtonKindCount> buttons_{};
    Slot bgLobby_{}, bgNull_{};
public:
    void loadAll();// 只在 Renderer::onEnter 调一次

    const sf::Texture* textureOf(ButtonKind kind) const;//临时支持按钮纹理
    const sf::Texture* textureBgLobby() const { return bgLobby_.tex.get(); }
    const sf::Texture* textureBgNull()  const { return bgNull_.tex.get(); }


};