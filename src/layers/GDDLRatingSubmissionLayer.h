#ifndef GDDLINTEGRATION_GDDLRATINGSUBMISSIONLAYER_H
#define GDDLINTEGRATION_GDDLRATINGSUBMISSIONLAYER_H

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

/**
 * json schema
 * {
 *  "levelID":341613,
 *  "rating":2,
 *  "enjoyment":6,
 *  "refreshRate":60,
 *  "device":1, - 1 for pc, 2 for mobile
 *  "proof":"https://youtu.be/3-BUEoH9WBs",
 *  "progress":100,
 *  "attempts":403,
 *  "isSolo":true - default for both 1p and 2p levels
 *  "secondPlayerID": int | null
 *  }
 *
 *  headers:
 *  Cookie: gddl.sid.sig=<sid.sig>; gddl.sid=<sid>
 */

class GDDLRatingSubmissionLayer final : public FLAlertLayer {
    CCMenuItemSpriteExtra* m_closeBtn{};

    CCTextInputNode* ratingTextfield = nullptr;
    CCTextInputNode* enjoymentTextfield = nullptr;
    CCTextInputNode* fpsTextfield = nullptr;
    CCTextInputNode* proofTextfield = nullptr;
    CCTextInputNode* percentTextfield = nullptr;
    CCTextInputNode* attemptsTextfield = nullptr;
    CCTextInputNode* secondPlayerTextfield = nullptr;
    CCLabelBMFont* deviceLabel = nullptr;
    CCMenuItemToggler* soloCompletionToggler = nullptr;

    const inline static std::string submissionEndpoint = "https://gdladder.com/api/login";
    const inline static std::string userSearchEndpoint = "https://gdladder.com/api/user/search";

    int levelID = 0, attempts = 0;
    bool twoPlayer = false;

    bool init(GJGameLevel* level);
    void onClose(CCObject* sender);

public:
    static GDDLRatingSubmissionLayer* create(GJGameLevel* level);
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLRATINGSUBMISSIONLAYER_H
