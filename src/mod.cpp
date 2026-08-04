#include "mods/hook.hpp"
#include "mods/service.hpp"
#include "mods/svc/ui.h"
#include "mods/svc/hook.h"
#include "mods/svc/log.h"

#include <cstdint>
#include <cstdio>

#include "twitchStuff/twitchData.h"
#include "twitchLoaderService.h"

DEFINE_MOD();
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(UiService, svc_ui);
IMPORT_SERVICE(TwitchEventsService, svc_twitch_events);

extern "C" {

static void handle_chat_event(const TwitchEvent& event) {
    char buf[512];
    snprintf(buf, sizeof(buf), "%s", event.data.c_str());
    svc_log->info(mod_ctx, buf);
}
//
// UiDialogDesc ui_dialog_desc = UI_DIALOG_DESC_INIT;
// UiDialogHandle ui_dialog_handle = UI_SCOPE_OVERLAY;
// UiWindowDesc ui_window_desc = UI_WINDOW_DESC_INIT;
// UiWindowHandle ui_window_handle;

MOD_EXPORT ModResult mod_initialize(ModError*) {
    // ui_dialog_desc.title="Twitch chat";
    // ui_dialog_desc.body_rml="ouais ya du contenue wouhou<span> pouet \n ah <\span> wow<\br>le sauit de lighnet";
    // ui_dialog_desc.action_count=1;
    // UiDialogAction ui_dialog_action = {"un label", [](ModContext*, UiDialogHandle, void*){}, nullptr, true};
    //
    // ui_dialog_desc.actions={&ui_dialog_action};
    // ui_dialog_desc.on_dismiss=[](ModContext*, UiDialogHandle, void*){};
    // ui_dialog_desc.user_data=nullptr;
    // svc_ui->dialog_push(mod_ctx, &ui_dialog_desc, &ui_dialog_handle);

    UiTabDesc tabs[1]={UI_TAB_DESC_INIT};
    // ui_window_desc.tabs = tabs;
    // ui_window_desc.tab_count=1;

    svc_log->info(mod_ctx, "TwitchChat mod initialized");
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    // thanks encounter :3
    // Create variables that get_events will write to
    const TwitchEvent* events = nullptr;
    uint32_t eventCount = 0;

    // Call into TwitchEventsService
    ModResult result = svc_twitch_events->get_events(mod_ctx, &events, &eventCount);
    if (result != MOD_OK) {
        return result;
    }

    // Iterate over all the events we see
    for (uint32_t eventIndex = 0; eventIndex < eventCount; ++eventIndex) {
        handle_chat_event(events[eventIndex]);
        // ui_dialog_desc.body_rml=events[eventIndex].data.c_str();

    }

    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    return MOD_OK;
}
}
