#include <px4_platform_common/module.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/time.h>
#include <uORB/topics/vehicle_command.h>
#include <uORB/uORB.h>

extern "C" __EXPORT int autostart_takeoff_main(int argc, char *argv[]);

int autostart_takeoff_main(int argc, char *argv[])
{
    PX4_INFO("Autostart Takeoff module started");

    // Esperar unos segundos tras el arranque
    px4_usleep(5 * 1e6); // 5 segundos

    // Crear publisher para vehicle_command
    orb_advert_t cmd_pub = orb_advertise(ORB_ID(vehicle_command), nullptr);
    struct vehicle_command_s cmd = {};

    cmd.timestamp = hrt_absolute_time();
    cmd.param1 = 1; // Arm
    cmd.command = vehicle_command_s::VEHICLE_CMD_COMPONENT_ARM_DISARM;
    cmd.target_system = 1;
    cmd.target_component = 1;
    cmd.source_system = 1;
    cmd.source_component = 1;
    orb_publish(ORB_ID(vehicle_command), cmd_pub, &cmd);

    PX4_INFO("Drone armed, waiting before takeoff...");
    px4_usleep(3 * 1e6);

    // Takeoff command
    cmd = {};
    cmd.timestamp = hrt_absolute_time();
    cmd.command = vehicle_command_s::VEHICLE_CMD_NAV_TAKEOFF;
    cmd.param7 = 5.0f; // altitude
    cmd.target_system = 1;
    cmd.target_component = 1;
    cmd.source_system = 1;
    cmd.source_component = 1;
    orb_publish(ORB_ID(vehicle_command), cmd_pub, &cmd);

    PX4_INFO("Takeoff command sent, hovering...");
    px4_usleep(10 * 1e6);

    // Land command
    cmd = {};
    cmd.timestamp = hrt_absolute_time();
    cmd.command = vehicle_command_s::VEHICLE_CMD_NAV_LAND;
    cmd.target_system = 1;
    cmd.target_component = 1;
    cmd.source_system = 1;
    cmd.source_component = 1;
    orb_publish(ORB_ID(vehicle_command), cmd_pub, &cmd);

    PX4_INFO("Landing command sent. Done!");

    return 0;
}
