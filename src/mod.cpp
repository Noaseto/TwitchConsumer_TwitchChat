#include "mods/hook.hpp"
#include "mods/service.hpp"
#include "mods/svc/ui.h"
#include "mods/svc/hook.h"
#include "mods/svc/log.h"

#include <cstdint>
#include <cstdio>

#include "twitchLoaderService.h"

DEFINE_MOD();
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(UiService, svc_ui);
IMPORT_SERVICE(TwitchEventsService, svc_twitch_events);

extern "C" {

static void handle_chat_event(const TwitchChatEvent& event) {
    char buf[512];
    snprintf(buf, sizeof(buf), "%s: %s", event.username, event.message);
    svc_log->info(mod_ctx, buf);
}

MOD_EXPORT ModResult mod_initialize(ModError*) {
    svc_log->info(mod_ctx, "TwitchChat mod initialized");
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    // thanks encounter :3
    // Create variables that get_events will write to
    const TwitchChatEvent* events = nullptr;
    uint32_t eventCount = 0;

    // Call into TwitchEventsService
    ModResult result = svc_twitch_events->get_events(mod_ctx, &events, &eventCount);
    if (result != MOD_OK) {
        return result;
    }

    // Iterate over all the events we see
    for (uint32_t eventIndex = 0; eventIndex < eventCount; ++eventIndex) {
        handle_chat_event(events[eventIndex]);
    }

    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    return MOD_OK;
}
}
