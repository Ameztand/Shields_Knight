#include "assetCache.h"

#include <iostream>

void AssetCache::loadInto(Slot& slot, const AssetSpec& spec)
{
    if (!spec.path) return; // 为空，没有美术：静默，走纯色

    auto tex = std::make_unique<sf::Texture>();
    if (!tex->loadFromFile(spec.path) || tex->getSize().x == 0u) {
        slot.warned = true;
        std::cerr << "[asset] missing: " << spec.path << " -> solid fallback\n"; //cerr 的内容哪怕程序秒崩也能看到。
        return;
    }
    slot.tex = std::move(tex);
}

void AssetCache::loadAll()
{
    for (size_t i = 0; i < kButtonKindCount; ++i)
    {
        loadInto(buttons_[i], assetOf(static_cast<ButtonKind>(i)));
    }
    //后续统一进配置表
    loadInto(bgLobby_, kBackgroundLobby);
    loadInto(bgNull_,  kBackgroundNull);
}

const sf::Texture* AssetCache::textureOf(ButtonKind kind) const
{
    return buttons_[static_cast<size_t>(kind)].tex.get();
}

