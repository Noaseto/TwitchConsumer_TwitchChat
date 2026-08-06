#include "mods/hook.hpp"
#include "mods/service.hpp"
#include "mods/svc/ui.h"
#include "mods/svc/hook.h"
#include "mods/svc/log.h"

#include <string>
#include <format>

#include "twitchStuff/twitchData.h"
#include "twitchLoaderService.h"

DEFINE_MOD();
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(UiService, svc_ui);
IMPORT_SERVICE(TwitchEventsService, svc_twitch_events);

extern "C" {

static ModResult handle_twitch_event(const TwitchEvent& event) {
    switch (event.type) {
        case TwitchEventType::ChatMessage:
            // atm data is json, needs to be formatted and payload retrieved
            char buf[512];
            snprintf(buf, sizeof(buf), "%p", event.data);
            svc_log->info(mod_ctx, std::format("{}", event.data).c_str());
            //svc_log->info(mod_ctx, event.data->c_str());
            break;
        case TwitchEventType::Follow:
        case TwitchEventType::Subscribe:
        case TwitchEventType::SubGift:
        case TwitchEventType::Cheer:
        case TwitchEventType::Unknown:
        case TwitchEventType::SessionWelcome:
            svc_log->debug(mod_ctx, std::format("Event {} not not handled", ".." ).c_str());
            break;
        case TwitchEventType::TwitchEventError:
        default:
            return MOD_ERROR;
    }
    return MOD_OK;
}

MOD_EXPORT ModResult mod_initialize(ModError*) {
    svc_log->info(mod_ctx, "TwitchChat mod initialized");
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    const TwitchEvent* events = nullptr;
    uint32_t eventCount = 0;

    // Call into TwitchEventsService
    ModResult result = svc_twitch_events->get_events(mod_ctx, &events, &eventCount);
    if (result != MOD_OK) {
        return result;
    }

    for (uint32_t eventIndex = 0; eventIndex < eventCount; ++eventIndex) {
        handle_twitch_event(events[eventIndex]);
    }

    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    return MOD_OK;
}
}
