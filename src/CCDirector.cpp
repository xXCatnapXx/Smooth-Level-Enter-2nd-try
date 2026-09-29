#include "CCDirector.hpp"
#include "CCTransitionPlayLayer.hpp"

void HookedCCDirector::onModify(auto& self) {
    geode::log::info("[SLE 2.2074 DEBUG] Installing CCDirector::replaceScene hook");
    (void)self.setHookPriorityPost("cocos2d::CCDirector::replaceScene", geode::Priority::Last);
}

bool HookedCCDirector::replaceScene(cocos2d::CCScene* scene) {
    // Diagnostic build for GD 2.2074. These messages tell us exactly which
    // assumption from the 2.2081 version stops matching at runtime.
    geode::log::info("[SLE 2.2074 DEBUG] replaceScene called; scene={}", static_cast<void*>(scene));

    auto transition = geode::cast::typeinfo_cast<cocos2d::CCTransitionFade*>(scene);
    if (!transition) {
        geode::log::info("[SLE 2.2074 DEBUG] scene is NOT CCTransitionFade -> passing through");
        return CCDirector::replaceScene(scene);
    }

    geode::log::info(
        "[SLE 2.2074 DEBUG] CCTransitionFade detected; inScene={}, outScene={}",
        static_cast<void*>(transition->m_pInScene),
        static_cast<void*>(transition->m_pOutScene)
    );

    if (!transition->m_pInScene) {
        geode::log::warn("[SLE 2.2074 DEBUG] transition has no inScene -> passing through");
        return CCDirector::replaceScene(scene);
    }

    auto playLayer = transition->m_pInScene->getChildByType<PlayLayer>(0);
    if (!playLayer) {
        geode::log::info("[SLE 2.2074 DEBUG] inScene has NO PlayLayer -> passing through");
        return CCDirector::replaceScene(scene);
    }

    geode::log::info(
        "[SLE 2.2074 DEBUG] PlayLayer FOUND at {} -> swapping to CCTransitionPlayLayer",
        static_cast<void*>(playLayer)
    );

    static void* vtable = []() -> void* {
        CCTransitionPlayLayer temp;
        // dtor releases both of these
        temp.m_pInScene = cocos2d::CCScene::create();
        temp.m_pOutScene = cocos2d::CCScene::create();
        return *(void**)&temp;
    }();

    // swap the vtables
    *(void**)scene = vtable;
    geode::log::info("[SLE 2.2074 DEBUG] vtable swapped; forwarding replaceScene");

    return CCDirector::replaceScene(scene);
}
