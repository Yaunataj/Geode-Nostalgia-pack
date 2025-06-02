#include <Geode/Geode.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <audio/include/SimpleAudioEngine.h>

using namespace geode::prelude;
using namespace CocosDenshion;

struct WindowsXPSounds {
    static void playStartupSound() {
        auto audio = SimpleAudioEngine::getInstance();
        audio->playEffect("startup.mp3", false, 1.0f, 1.0f, 1.0f);
    }

    static void playShutdownSound() {
        auto audio = SimpleAudioEngine::getInstance();
        audio->playEffect("shutdown.mp3", false, 1.0f, 1.0f, 1.0f);
    }
};

// Play startup sound when game launches
struct AppDelegateHook : Modify<AppDelegateHook, AppDelegate> {
    void applicationDidEnterBackground() {
        WindowsXPSounds::playShutdownSound();
        // Small delay to let sound play before closing
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        AppDelegate::applicationDidEnterBackground();
    }
};

// Play startup sound after loading finishes
struct MenuLayerHook : Modify<MenuLayerHook, MenuLayer> {
    bool init() {
        if (!MenuLayer::init())
            return false;
        
        // Small delay to avoid overlapping with other sounds
        Loader::get()->queueInMainThread([]() {
            WindowsXPSounds::playStartupSound();
        });
        
        return true;
    }
};