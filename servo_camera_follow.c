/*
 * This tracking function still has problems. It doesn't perfectly track the color
 * unless the color is close to the center of the camera. Otherwise there's nothing to
 * keep the servo from continuing to turn to its limit if the object is on one side or the
 * other.
 */

#include <kipr/botball.h>

#define GREEN 0
#define CAMERA_IMAGE_CENTER 80
#define SERVO_POSITION_CENTER 1000
#define SERVO_POSITION_LIMIT_BOTTOM 300
#define SERVO_POSITION_LIMIT_TOP 1700
#define SERVO_POSITION_ADJUST 50
#define SLEEP_MS 10

int main()
{
    camera_track(GREEN,3);
    return 0;
}

/*
 * camera_track takes the following arguments:
 *      channel     : a color channel
 *      servo_port  : a servo port
 */
void camera_track(channel, servo_port)
{
    enable_servos(); // Activate the servos.
    camera_open(); // Turn on the camera.
    set_servo_position(servo_port, SERVO_POSITION_CENTER); // Reset servo to center position.
    int servo_position = SERVO_POSITION_CENTER; // Variable used to keep state of the servo position as it adjusts.
   
    while(1) { // Infinite loop.
        camera_update(); // Refresh the image from the camera.
        if (get_object_count(channel) > 0) { // Am I seeing this color?
            if (get_object_center_x(channel, 0) > CAMERA_IMAGE_CENTER) { // If I'm seeing the color and it's on my right.
                if ((servo_position - SERVO_POSITION_ADJUST) >= SERVO_POSITION_LIMIT_BOTTOM) { // Make sure we don't go past the servo bottom limit.
                    servo_position -= SERVO_POSITION_ADJUST; // Decrement the servo position.
                    set_servo_position(servo_port, servo_pos); // Move the servo to the new position.
                }
                printf("Servo position = %d\n", servo_pos); // Print current servo position.
                msleep(SLEEP_MS); // Hang out for a little while.
            }
            if (get_object_center_x(channel, 0) < CAMERA_IMAGE_CENTER) { // If I'm seeing the color and it's on my right.
                if ((servo_position + SERVO_POSITION_ADJUST) <= SERVO_POSITION_LIMIT_TOP) { // Make sure we don't go past the servo top limit.
                    servo_pos += SERVO_POSITION_ADJUST; // Increment the servo position.
                    set_servo_position(servo_port, servo_pos); // Move the servo to the new position.
                }           
                printf("Servo position = %d\n", servo_pos); // Print current servo position.
                msleep(SLEEP_MS); // Hang out for a little while.}
            }
        }
        else { // The color was not detected.
            printf("Color not detected.\n");
        }
    }
 }