#include <Mouse.h>

#define LEFT_UP '0'
#define LEFT_DOWN '1'
#define RIGHT_UP '2'
#define RIGHT_DOWN '3'
#define LEFT_CLICK '4'
#define RIGHT_CLICK '5'

void setup() {
  Serial1.begin(9600);

  Mouse.begin();
}

void loop() {
  if (!Serial1.available()) {
    delay(1);
    return;
  }

  char control_char = Serial1.read();

  switch (control_char) {
    case LEFT_UP:
      Mouse.release(MOUSE_LEFT);
      break;
    case LEFT_DOWN:
      Mouse.press(MOUSE_LEFT);
      break;
    case LEFT_CLICK:
      Mouse.click(MOUSE_LEFT);
      break;
    case RIGHT_UP:
      Mouse.release(MOUSE_RIGHT);
      break;
    case RIGHT_DOWN:
      Mouse.press(MOUSE_RIGHT);
      break;
    case RIGHT_CLICK:
      Mouse.click(MOUSE_RIGHT);
      break;
    default:
      delay(1);
      break;
  }
}
