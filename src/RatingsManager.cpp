#include "RatingsManager.h"

#include <Geode/utils/web.hpp>

using namespace geode::prelude;
using json = nlohmann::json;

std::map<int, GDDLRating> RatingsManager::demonMap;
std::vector<int> RatingsManager::tierColors = {
    0x000000,
    0x420696,
    0x420696,
    0x420696,
    0x420696,
    0x420696,
    0x420696
};

GDDLRating RatingsManager::parseJson(std::string response) {
    json levelData = json::parse(response);
    return GDDLRating(levelData);
}

std::string RatingsManager::requestRating(int id) {
    // it's a simple request so I don't think that async is needed here
    // hi it's me from the future - this is actually quite noticeable, I need to redo this lmao
    std::string requestURL = "https://gdladder.com/api/level?levelID=" + std::to_string(id);
    auto res = web::fetch(requestURL);
    if (!res) {
        return "";
    }
    return res.value();
}

cocos2d::ccColor3B RatingsManager::convertToColor(int hexColor) {
    int r = (hexColor >> (8*0)) & 0xff;
    int g = (hexColor >> (8*1)) & 0xff;
    int b = (hexColor >> (8*2)) & 0xff;
    return cocos2d::ccColor3B(r, g, b);
}

int RatingsManager::getDemonTier(int id) {
    if (!demonMap.contains(id)) {
        std::string response = requestRating(id);
        if (response.empty()) return -1;
        GDDLRating rating = parseJson(response);
        demonMap[id] = rating;
    }
    return demonMap[id].roundedRating;
}

cocos2d::ccColor3B RatingsManager::getTierColor(int tier) {
    if(tier > tierColors.size() || tier < 0) {
        return cocos2d::ccColor3B(255, 255, 255);
    }
    int hexColor = tierColors[tier];
    return convertToColor(hexColor);
}
