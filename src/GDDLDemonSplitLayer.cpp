#include "GDDLDemonSplitLayer.h"

#include <Geode/Geode.hpp>

#include "RatingsManager.h"
#include "GDDLSearchLayer.h"

bool GDDLDemonSplitLayer::init() {
    if(!FLAlertLayer::init(75)) return false; // that magic number is actualy bg opacity btw

    CCPoint popupSize = {360.0f, 200.0f};
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    // background
    auto bg = CCScale9Sprite::create("GJ_square02.png", {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({ winSize.width / 2, winSize.height / 2 });
    bg->setID("gddl-demon-split-bg"_spr);
    m_mainLayer->addChild(bg, -1);
    // menu with the main layout
    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("button-menu"_spr);
    m_buttonMenu->setLayout(ColumnLayout::create()->setGap(5.0f)->setAxisReverse(true)->setAutoScale(true));
    m_mainLayer->addChild(m_buttonMenu, 10);
    // title
    auto titleContainer = CCMenu::create();
    titleContainer->setLayout(RowLayout::create());
    titleContainer->setContentSize({200.0f, 30.0f});
    auto title = CCLabelBMFont::create("GDDL Demon Split", "goldFont.fnt");
    title->setID("gddl-demon-split-title"_spr);
    titleContainer->addChild(title);
    // the (i) button
    auto iButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    iButtonSprite->setScale(0.5f);
    auto iButton = CCMenuItemSpriteExtra::create(iButtonSprite, this, menu_selector(GDDLDemonSplitLayer::onInfo));
    iButton->setScale(0.4f);
    iButton->setContentSize({10.0f, 10.0f});
    titleContainer->addChild(iButton);
    titleContainer->updateLayout();
    m_buttonMenu->addChild(titleContainer);
    m_buttonMenu->reorderChild(titleContainer, 0);

    // this is where the tiers go
    for (int row = 0; row < 5; row++) {
        auto rowNode = CCNode::create();
        rowNode->setLayout(RowLayout::create()->setGap(5.0f));
        rowNode->setContentSize({350.0f, 20.0f});
        for (int column = 0; column < 7; column++) {
            auto tierNode = createTierNode(row+1+column*5);
            rowNode->addChild(tierNode);
        }
        rowNode->updateLayout();
        m_buttonMenu->addChild(rowNode);
        m_buttonMenu->reorderChild(rowNode, row+1);
    }
    // ok button
    auto spr = ButtonSprite::create("OK");
    auto okButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GDDLDemonSplitLayer::onClose));
    okButton->setID("gddl-demon-split-ok"_spr);
    m_buttonMenu->addChild(okButton);
    m_buttonMenu->reorderChild(okButton, 10);

    m_buttonMenu->updateLayout();
    m_mainLayer->updateLayout();
    // fix the iButton :tm: placement (is it clear already that I never did frontend)
    iButton->setPosition({iButton->getPositionX(), 15.0f});
    return true;
}

void GDDLDemonSplitLayer::onClose(cocos2d::CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLDemonSplitLayer::onInfo(cocos2d::CCObject *sender) { // NOLINT(*-convert-member-functions-to-static)
    std::map<int, int> tierStats = RatingsManager::getTierStats();
    int total = 0;
    for (auto tierCountPair: tierStats) {
        total += tierCountPair.second;
    }
    const int unrated = tierStats[-1];
    const std::string message = "Not accounting for <cb>official levels</c>, <co>gauntlet levels</c> and <cy>weekly "
                                "demons</c>.\n <cg>Total:</c> " +
                                std::to_string(total) +
                                ", of which with <cr>unknown</c> tier: " + std::to_string(unrated);
    FLAlertLayer::create("GDDL Demon Split", message.c_str(), "OK")->show();
}

void GDDLDemonSplitLayer::onTierSearch(cocos2d::CCObject *sender) { // NOLINT(*-convert-member-functions-to-static)
    auto *senderNode = dynamic_cast<CCNode *>(sender);
    const std::string tierStr = senderNode->getID();
    const int tierNumber = std::stoi(tierStr.substr(12, tierStr.size()-10));
    GDDLSearchLayer::requestSearchFromDemonSplit(tierNumber);
    // the list should display itself hopefully
}

CCNode *GDDLDemonSplitLayer::createTierNode(int tier) {
    auto tierNode = CCMenu::create();
    tierNode->setLayout(RowLayout::create()->setGap(3.0f)->setAutoScale(true));
    tierNode->setContentSize({50.0f, 20.0f});
    // tier sprite
    std::string tierSpriteName = "tier_" + std::to_string(tier) + ".png";
    auto textureName = Mod::get()->expandSpriteName(tierSpriteName.c_str());
    auto tierSprite = CCSprite::create(textureName);
    tierSprite->setScale(0.05f);
    tierSprite->setContentSize({20.0f, 20.0f});
    auto tierButton = CCMenuItemSpriteExtra::create(tierSprite, this, menu_selector(GDDLDemonSplitLayer::onTierSearch));
    tierButton->setContentSize({20.0f, 20.0f});
    tierButton->setID("button-tier-"+std::to_string(tier));
    tierNode->addChild(tierButton);
    // tier count
    int count = RatingsManager::getTierStats()[tier];
    auto countLabel = CCLabelBMFont::create(std::to_string(count).c_str(), "chatFont.fnt");
    countLabel->setContentSize({30.0f, 20.0f});
    tierNode->addChild(countLabel);
    tierNode->updateLayout();
    // sprite size fix
    tierSprite->setScale(0.15f);
    tierButton->setPosition({14.0f, 11.5f});
    if(count >= 1000) {
        countLabel->setScale(0.8f);
    }
    return tierNode;
}

GDDLDemonSplitLayer *GDDLDemonSplitLayer::create() {
    if (const auto newLayer = new GDDLDemonSplitLayer(); newLayer != nullptr && newLayer->init()) {
        newLayer->autorelease();
        return newLayer;
    } else {
        delete newLayer;
        return nullptr;
    }
}

void GDDLDemonSplitLayer::show() {
    FLAlertLayer::show();
    cocos::handleTouchPriority(this);
}

