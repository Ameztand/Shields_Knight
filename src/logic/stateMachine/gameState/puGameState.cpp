#include "puGameState.h"

#include "../../dataStore.h"
#include "../../buttonManager/button.h"
#include "../../buttonManager/buttonID.h"

#include <iostream>
#include <cmath>

namespace HitTest {
    // 底：点 + 中心 + 尺寸
    bool contains(const Vec2& p, const Vec2& center, float w, float h)
    {
        const float halfW = w * 0.5f;
        const float halfH = h * 0.5f;
        return p.x >= center.x - halfW && p.x < center.x + halfW
            && p.y >= center.y - halfH && p.y < center.y + halfH;
    }

    // 实体：用 Entity 自己的派生边界，别在这里重算一遍
    bool contains(const Entity& e, const Vec2& p)
    {
        return p.x >= e.left() && p.x < e.right()
            && p.y >= e.top() && p.y < e.bottom();
    }

    // 鼠标：设计坐标的 int → float 是精确的（int 转 float 不丢位），所以这里可以放心转
    bool contains(const Entity& e, const Position& p)
    {
        return contains(e, Vec2{ static_cast<float>(p.x), static_cast<float>(p.y) });
    }

} // namespace HitTest

namespace HandleInput2 {
    bool EscPop(Data& world, const MsgData& msgData)
    {
        bool temp = (msgData.Esc == KeySta::Falling);
        world.gameIntend.popSta = temp;
        return temp;
    }

    bool EscPuase(Data& world, const MsgData& msgData)
    {
        bool temp = (msgData.Esc == KeySta::Falling);
        world.gameIntend.pushPause = temp;
        return temp;
    }
}
//目前这个HandleInput是直接复制粘贴，考虑提炼utils？

//const std::vector<Entity>& PuGame::getEntitys() const
//{
//    return entityManager_.commitEntitys();
//}

void PuGame::onEnter(Data& data)
{
    gameData_.initGameData();//或许需要世界信息？
    entityManager_.initEntityManager();
    entityManager_.registerEntitys(EntityKind::testEntity);
    //临时搭建选择器
    gameData_.setID(1);
}

void PuGame::onExit(Data& data)
{
    //测试
    //entityManager_.deleteEntitys(EntityKind::testEntity);
}

void PuGame::onSuspend(Data& world)
{
    int temp = gameData_.getCnt();
    temp /= 2;
    gameData_.setCnt(temp);
    //printf("触发onSuspend，cnt减半，及cnt = %d\n", temp);//不知道为什么用不了，只能用endl强行刷新
    std::cout << "onSuspend: cnt = " << gameData_.getCnt() << std::endl;   // ← 换成 cout + endl
}

void PuGame::onResume(Data& world)
{
    int temp = gameData_.getCnt();
    temp += 1;
    gameData_.setCnt(temp);
    //printf("触发onResume，cnt加一，及cnt = %d\n", temp);
    std::cout << "onResume: cnt = " << gameData_.getCnt() << std::endl;
}

void PuGame::tick(StateContext& ctx)
{
    //退出
    if (HandleInput2::EscPuase(ctx.world, ctx.msgData))return;

    //游戏逻辑
    const auto& msgData = ctx.msgData;//一次性获取

    //按钮判断
    for (const auto& btn : ctx.buttonList) {          // ← 用新路径
        if (!btn.isClicked) continue;
        switch (btn.status.ID) {
        case ButtonID::puGameTestButton:
        {
            int temp = gameData_.getCnt();
            std::cout << "test , cnt = " << temp <<  ", now = " << ctx.world.getNow()/1000 << std::endl;
            temp++;
            gameData_.setCnt(temp);

            break;
        }
        default: break;
        }
    }

    //测试方块判定
    for (const auto& ety : entityManager_.commitEntitys()) {
        if (ety.kind == EntityKind::Null) continue;//跳过墓碑
        if (ety.handle.id == gameData_.getID() )
        {
            float dx = 0.f;
            float dy = 0.f;
            if (MsgData::held(msgData.W)) dy -= 1.f;
            if (MsgData::held(msgData.S)) dy += 1.f;
            if (MsgData::held(msgData.A)) dx -= 1.f;
            if (MsgData::held(msgData.D)) dx += 1.f;

            if (dx != 0.f || dy != 0.f) {
                const float len = std::sqrt(dx * dx + dy * dy);//??
                dx /= len;
                dy /= len;                           // 斜走不加速
            }

            Entity temp = ety;

            temp.vel = { dx * ety.speed, dy * ety.speed };
            temp.updateMove(ctx.world.getDeltaSec());

            entityManager_.setEntity(temp,temp.handle.id);
        }
    }

    //方块选择
    if (msgData.MouseL == KeySta::Falling && msgData.MousePos != Epos)
    {
        gameData_.setID(0);// 先清空选中
        for (const auto& ety : entityManager_.commitEntitys()) {
            if (ety.kind == EntityKind::Null) continue;//跳过墓碑
            //卡牌实际情况考虑遍历手牌数组
            //遍历方向就是优先选取的卡牌
            if (HitTest::contains(ety, msgData.MousePos))
            {
                gameData_.setID(ety.handle.id);
                break;//有必要吗？
            }
        }
    }

    //检查拖动
    if (msgData.MouseL == KeySta::Falling && msgData.MousePos != Epos)
    {
        //gameData_.setCentre(msgData.MousePos);
        gameData_.setDownLID(gameData_.getID());
    }
    else if ((msgData.MouseL == KeySta::DownL || msgData.MouseL == KeySta::DownS) && msgData.MousePos != Epos)
    {
        //明天在做
        //if (gameData_.getID() == gameData_.getDownLID() && HitTest::contains(entityManager_.getEntity(gameData_.getDownLID(), msgData.MousePos))
        if (gameData_.getID() == gameData_.getDownLID())
        {
            //if(gameData_.getCentre() != Epos)gameData_.setCentre(msgData.MousePos);
            if (gameData_.getCentre() != Epos)
            {
                Entity temp = entityManager_.getEntity(gameData_.getDownLID());
                temp.pos.x += msgData.MousePos.x - gameData_.getCentre().x;
                temp.pos.y += msgData.MousePos.y - gameData_.getCentre().y;
                entityManager_.setEntity(temp, gameData_.getDownLID());
            }
            gameData_.setCentre(msgData.MousePos);
        }
    }
    else if (msgData.MouseL == KeySta::Rising || !(msgData.MousePos != Epos))
    {
        gameData_.setCentre(Epos);
        gameData_.setDownLID(0);
    }

    //检查悬停
    //没有处理鼠标出界问题？
    if (!MsgData::held(msgData.MouseL) && msgData.MousePos != Epos)
    {
        //gameData_.setHoverID(0);
        for (const auto& ety : entityManager_.commitEntitys()) {
            if (ety.kind == EntityKind::Null) continue;//跳过墓碑
            //卡牌实际情况考虑遍历手牌数组
            //遍历方向就是优先选取的卡牌
            if (HitTest::contains(ety, msgData.MousePos))
            {
                //gameData_.setHoverID(ety.handle.id);
                entityManager_.updateHover(ety.handle.id, ctx.world.getDeltaSec());
                //break;//有必要吗？
            }
            else {
                entityManager_.clearHover(ety.handle.id);
            }
        }
    }

    //检查注册和删除
    if (msgData.Alt == KeySta::Falling)
    {
        //int i = gameData_.getID();
        //gameData_.setID((3 - i) % 3);
        entityManager_.deleteEntitys(gameData_.getID());
        entityManager_.registerEntitys(EntityKind::testEntity);
    }
}

void PuGame::submitRender(RenderDataEntity& out)
{
    //还没跳过墓碑
    out.entitys_ = entityManager_.commitEntitys();
    out.selectedID_ = gameData_.getID();
    //out.selectedHoverID_ = gameData_.getHoverID();
    //if (gameData_.getHoverID() != 0) out.loogTimeHover_ = entityManager_.getEntity(gameData_.getHoverID()).isLongHover;
}

GameStateKind PuGame::getID() const
{
    return ID;
}
