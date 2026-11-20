#include "battle.h"

#include <iostream>
#include <cmath>

#include "logic/contract/stateContext.h"
#include "logic/contract/renderDataPu.h"
#include "logic/contract/battleContext.h"

#include "logic/entity/entityStore.h"
#include "logic/button/button.h"
#include "logic/gameData.h"
#include "input/msgData.h"

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

void Battle::onEnter(StateContext& stateCtx)
{
}

void Battle::onExit(StateContext& stateCtx)
{
}

void Battle::tick(StateContext &stateCtx, BattleContext &battleCtx)
{
    //按钮判断
    for (const auto& btn : stateCtx.buttonList) {          // ← 用新路径
        if (!btn.isClicked) continue;
        switch (btn.status.kind) {
        case ButtonKind::puGameTestButton:
        {
            int temp = battleCtx.gameData.getCnt();
            double dt_ = stateCtx.dt;
            std::cout << "test , cnt = " << temp <<  ", now = " << dt_ << std::endl;
            temp++;
            battleCtx.gameData.setCnt(temp);

            break;
        }
        default: break;
        }
    }

    //测试方块判定
    for (const auto& ety : battleCtx.entityPool.commitEntitys()) {
        if (ety.kind == EntityKind::Null) continue;//跳过墓碑
        if (ety.handle.id == battleCtx.gameData.getID() )
        {
            float dx = 0.f;
            float dy = 0.f;
            //MsgData tempMsgData = stateCtx.msgData;// ← 这里是 const 引用，不能直接修改,后续改为提前拷贝一份？
            if (MsgData::held(stateCtx.msgData.W)) dy -= 1.f;
            if (MsgData::held(stateCtx.msgData.S)) dy += 1.f;
            if (MsgData::held(stateCtx.msgData.A)) dx -= 1.f;
            if (MsgData::held(stateCtx.msgData.D)) dx += 1.f;

            if (dx != 0.f || dy != 0.f) {
                const float len = std::sqrt(dx * dx + dy * dy);//??
                dx /= len;
                dy /= len;                           // 斜走不加速
            }

            Entity temp = ety;

            temp.vel = { dx * ety.speed, dy * ety.speed };
            temp.updateMove(stateCtx.dt);

            battleCtx.entityPool.setEntity(temp,temp.handle.id);
        }
    }

    //方块选择
    if (stateCtx.msgData.MouseL == KeySta::Falling && stateCtx.msgData.MousePos != Epos)
    {
        battleCtx.gameData.setID(0);// 先清空选中
        for (const auto& ety : battleCtx.entityPool.commitEntitys()) {
            if (ety.kind == EntityKind::Null) continue;//跳过墓碑
            //卡牌实际情况考虑遍历手牌数组
            //遍历方向就是优先选取的卡牌
            if (HitTest::contains(ety, stateCtx.msgData.MousePos))
            {
                battleCtx.gameData.setID(ety.handle.id);
                break;//有必要吗？
            }
        }
    }

    //检查拖动
    if (stateCtx.msgData.MouseL == KeySta::Falling && stateCtx.msgData.MousePos != Epos)
    {
        //gameData_.setCentre(msgData.MousePos);
        battleCtx.gameData.setDownLID(battleCtx.gameData.getID());
    }
    else if ((stateCtx.msgData.MouseL == KeySta::DownL || stateCtx.msgData.MouseL == KeySta::DownS) && stateCtx.msgData.MousePos != Epos)
    {
        //明天在做
        //if (gameData_.getID() == gameData_.getDownLID() && HitTest::contains(entityStore_.getEntity(gameData_.getDownLID(), msgData.MousePos))
        if (battleCtx.gameData.getID() == battleCtx.gameData.getDownLID())
        {
            //if(gameData_.getCentre() != Epos)gameData_.setCentre(msgData.MousePos);
            if (battleCtx.gameData.getCentre() != Epos)
            {
                Entity temp = battleCtx.entityPool.getEntity(battleCtx.gameData.getDownLID());
                temp.pos.x += stateCtx.msgData.MousePos.x - battleCtx.gameData.getCentre().x;
                temp.pos.y += stateCtx.msgData.MousePos.y - battleCtx.gameData.getCentre().y;
                battleCtx.entityPool.setEntity(temp, battleCtx.gameData.getDownLID());
            }
            battleCtx.gameData.setCentre(stateCtx.msgData.MousePos);
        }
    }
    else if (stateCtx.msgData.MouseL == KeySta::Rising || !(stateCtx.msgData.MousePos != Epos))
    {
        battleCtx.gameData.setCentre(Epos);
        battleCtx.gameData.setDownLID(0);
    }

    //检查悬停
    //没有处理鼠标出界问题？
    if (!MsgData::held(stateCtx.msgData.MouseL) && stateCtx.msgData.MousePos != Epos)
    {
        //gameData_.setHoverID(0);
        for (const auto& ety : battleCtx.entityPool.commitEntitys()) {
            if (ety.kind == EntityKind::Null) continue;//跳过墓碑
            //卡牌实际情况考虑遍历手牌数组
            //遍历方向就是优先选取的卡牌
            if (HitTest::contains(ety, stateCtx.msgData.MousePos))
            {
                //gameData_.setHoverID(ety.handle.id);
                battleCtx.entityPool.updateHover(ety.handle.id, stateCtx.dt);
                //break;//有必要吗？
            }
            else {
                battleCtx.entityPool.clearHover(ety.handle.id);
            }
        }
    }

    //检查注册和删除
    if (stateCtx.msgData.Alt == KeySta::Falling)
    {
        //int i = gameData_.getID();
        //gameData_.setID((3 - i) % 3);
        battleCtx.entityPool.deleteEntitys(battleCtx.gameData.getID());
        battleCtx.entityPool.registerEntitys(EntityKind::testEntity);
    }
}

void Battle::submitRender(RenderDataPu &out)
{
}
