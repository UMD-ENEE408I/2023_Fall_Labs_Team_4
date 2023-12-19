#include "sd_state_machine.hpp"

Backup_State& Backup_State::instance() {
    static Backup_State __instance;
    return __instance;
}

/**
 * Remain in stop state for the set duration and then transition to rest
*/
Abstract_State<Input, Output>& Backup_State::get_next_state(const Input& input) {
    if((input.gyro.timestamp - this->entry_time >= BACK_STATE_LEN_MILLIS)) {
        return Wander_State::instance();
    }
    else {
        return Backup_State::instance();
    }
    
}

/**
 * turn on brake when entering the state
*/
void Backup_State::entry_behavior(const Input& input, Output& output) {
    //output.screen.clearDisplay();
    //output.screen.drawString(0, 6, "State: Back");
    
    this->entry_time = input.gyro.timestamp;

    output.left_motor.set_pwm(-140);
    output.right_motor.set_pwm(-110);
}

void Backup_State::do_behavior(const Input& input, Output& output) {
    // do nothing
}

/**
 * turn of the brake when leaving the state
*/
void Backup_State::exit_behavior(const Input& input, Output& output) {
    output.left_motor.stop();
    output.right_motor.stop();
}