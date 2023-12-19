#include "sd_state_machine.hpp"

Rest_State& Rest_State::instance() {
    static Rest_State __instance;
    return __instance;
}
        
Abstract_State<Input, Output>& Rest_State::get_next_state(const Input& input) {
    if((input.receive_packet.next_state == SPIN_STATE_CODE) && ((input.gyro.timestamp -input.receive_packet.timestamp) < 1000)) {
        return Spin_State::instance();
    }
    else if (input.receive_packet.next_state == WALK_STATE_CODE && ((input.gyro.timestamp - this->entry_time) > 1300)) {
        return Walk_State::instance();
    }
    else {
        return Rest_State::instance();
    }
}

void Rest_State::entry_behavior(const Input& input, Output& output) {
    //output.screen.clearDisplay();
    this->entry_time = input.gyro.timestamp;
}
void Rest_State::do_behavior(const Input& input, Output& output) {
    //output.screen.drawString(0, 6, "State: Rest");
}