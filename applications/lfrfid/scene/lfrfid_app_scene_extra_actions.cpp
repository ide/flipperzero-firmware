#include "lfrfid_app_scene_extra_actions.h"

typedef enum {
    SubmenuASK,
    SubmenuPSK,
} SubmenuIndex;

void LfRfidAppSceneExtraActions::on_enter(LfRfidApp* app, bool need_restore) {
    auto submenu = app->view_controller.get<SubmenuVM>();

    submenu->add_item("Read ASK (Animal, Ordinary Card)", SubmenuASK, submenu_callback, app);
    submenu->add_item("Read PSK (Indala)", SubmenuPSK, submenu_callback, app);

    if(need_restore) {
        submenu->set_selected_item(submenu_item_selected);
    }

    app->view_controller.switch_to<SubmenuVM>();

    // clear key
    string_reset(app->file_name);
    app->protocol_id = PROTOCOL_NO;
}

bool LfRfidAppSceneExtraActions::on_event(LfRfidApp* app, LfRfidApp::Event* event) {
    bool consumed = false;

    if(event->type == LfRfidApp::EventType::MenuSelected) {
        submenu_item_selected = event->payload.signed_int;
        switch(event->payload.signed_int) {
        case SubmenuASK:
            app->read_type = LFRFIDWorkerReadTypeASKOnly;
            break;
        case SubmenuPSK:
            app->read_type = LFRFIDWorkerReadTypePSKOnly;
            break;
        }
        app->scene_controller.switch_to_next_scene(LfRfidApp::SceneType::Read);
        consumed = true;
    }

    return consumed;
}

void LfRfidAppSceneExtraActions::on_exit(LfRfidApp* app) {
    app->view_controller.get<SubmenuVM>()->clean();
}

void LfRfidAppSceneExtraActions::submenu_callback(void* context, uint32_t index) {
    LfRfidApp* app = static_cast<LfRfidApp*>(context);
    LfRfidApp::Event event;

    event.type = LfRfidApp::EventType::MenuSelected;
    event.payload.signed_int = index;

    app->view_controller.send_event(&event);
}