#define LEFT_1 1
#define LEFT_2 2
#define LEFT_3 3
#define LEFT_4 4
#define LEFT_5 5

#define RIGHT_1 2
#define RIGHT_2 3
#define RIGHT_3 4
#define RIGHT_4 5
#define RIGHT_5 6

#define LEFT_UP '0'
#define LEFT_DOWN '1'
#define RIGHT_UP '2'
#define RIGHT_DOWN '3'
#define LEFT_CLICK '4'
#define RIGHT_CLICK '5'

enum {MOUSE_LEFT, MOUSE_RIGHT};

int INSTANT_CLICK_DELAY = 500;
unsigned long TRIGGER_PRESS_TIME = 800;

unsigned long left_entered_high_time;
int right_prev_state;

bool left_pressed = false;
bool right_pressed = false;

void mouse_press(int mouse) {
  if (mouse == MOUSE_LEFT) {
    left_pressed = true;
    Serial.write(LEFT_DOWN);
  } else {
    right_pressed = true;
    Serial.write(RIGHT_DOWN);
  }
}

void mouse_release(int mouse) {
  if (mouse == MOUSE_LEFT) {
    left_pressed = false;
    Serial.write(LEFT_UP);
  } else {
    right_pressed = false;
    Serial.write(RIGHT_UP);
  }
}

void mouse_click(int mouse) {
  if (mouse == MOUSE_LEFT) {
    left_pressed = false;
    Serial.write(LEFT_CLICK);
  } else {
    right_pressed = false;
    Serial.write(RIGHT_CLICK);
  }
}

bool mouse_is_pressed(int mouse) {
  return (mouse == MOUSE_LEFT ? left_pressed : right_pressed);
}

unsigned long mouse_press_action(unsigned long entered_high_time, int state, int mouse) {
    unsigned long cur_time = millis();
    bool is_pressed = mouse_is_pressed(mouse);

    if (state == HIGH) {
        if (entered_high_time == 0) {
            // first entrance
            entered_high_time = cur_time;
            if (is_pressed) {
                //Mouse.release(mouse);
                mouse_release(mouse);
            }
            //Mouse.click(mouse);
            mouse_click(mouse);
        }
        if (cur_time - entered_high_time >= TRIGGER_PRESS_TIME) {
            if (!is_pressed) {
                //Mouse.press(mouse);
                mouse_press(mouse);
            }
        }
    } else { // state == LOW
        if (is_pressed) {
            //Mouse.release(mouse);
            mouse_release(mouse);
        }
        entered_high_time = 0;
    }

    return entered_high_time;
}

int mouse_click_action(int prev_state, int state, int mouse) {
    if (state == HIGH) {
        if (prev_state == LOW) {
            if (mouse_is_pressed(mouse)) {
                //Mouse.release(mouse);
                mouse_release(mouse);
            }
            //Mouse.click(mouse);
            mouse_click(mouse);
            delay(INSTANT_CLICK_DELAY);
        }
    }

    return state;
}

void setup() {
  pinMode(RIGHT_1, INPUT);
  pinMode(RIGHT_2, INPUT);
  pinMode(RIGHT_3, INPUT);
  pinMode(RIGHT_4, INPUT);
  pinMode(RIGHT_5, INPUT);

  left_entered_high_time = millis();
  right_prev_state = LOW;
  
  Serial.begin(9600);
}

void loop() {
  bool new_left = 
    analogRead(LEFT_1) > 127 ||
    analogRead(LEFT_2) > 127 ||
    analogRead(LEFT_3) > 127 ||
    analogRead(LEFT_4) > 127 ||
    analogRead(LEFT_5) > 127;

  bool new_right = 
    digitalRead(RIGHT_1) ||
    digitalRead(RIGHT_2) ||
    digitalRead(RIGHT_3) ||
    digitalRead(RIGHT_4) ||
    digitalRead(RIGHT_5);

  // Handle left
  left_entered_high_time = mouse_press_action(left_entered_high_time, new_left, MOUSE_LEFT);

  //Handle right
  right_prev_state = mouse_click_action(right_prev_state, new_right, MOUSE_RIGHT);

  delay(1);
}
