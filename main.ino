/*
 * 2018년도 2학기 창의공학설계 미니프로젝트 2
 * 2018. 12. 07
 * 20조
*/

//핀과 관련된 상수
#define TRIGGER_PIN 12
#define ECHO_PIN 11

#define ENA 6
#define IN1 7
#define IN2 3
#define ENB 5
#define IN3 4
#define IN4 2

//바퀴를 나타내는 상수
#define L_WHEEL 0x01
#define R_WHEEL 0x10

//방향과 관련된 상수
#define STOP 0
#define FOWARD 1
#define BACK 2
#define LEFT 3
#define RIGHT 4

//센서 상태와 관련된 상수
#define SENSOR_THRESHOLD 200
#define SENSOR_L 0x001
#define SENSOR_R 0x010
#define SENSOR_F 0x100

//거리측정과 관련된 상수
#define DISTANCE_THRESHOLD 15
#define DISTANCE_COUNT_THRESHOLD 5

//속도와 관련된 상수
#define SPEED 150
#define TURNSPEED 150

//시간과 관련된 상수
#define TICK 75
#define TOCK 10
#define TURNTIME 200

int sensorStatus = 0;
int distance = 0;
int distanceCount = 0;

void wheel(int lr, int dir, int speed) {
	if (lr & L_WHEEL) {
		if (dir == STOP) {
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, LOW);
			analogWrite(ENA, 0);	
		}	
		if (dir == FOWARD) {
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, HIGH);
			analogWrite(ENA, speed);
		}
		if (dir == BACK) {
			digitalWrite(IN1, HIGH;
			digitalWrite(IN2, LOW);
			analogWrite(ENA, speed);
		}
	}
	if (lr & R_WHEEL) {
		if (dir == STOP) {
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, LOW);
			analogWrite(ENB, 0);	
		}	
		if (dir == FOWARD) {
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, HIGH);
			analogWrite(ENB, speed);
		}
		if (dir == BACK) {
			digitalWrite(IN3, HIGH;
			digitalWrite(IN4, LOW);
			analogWrite(ENB, speed);
		}
	}
}

void sense() {
	sensorStatus = ((analogRead(MODULE_L) > SENSOR_THRESHOLD) ? SENSOR_L : 0) | 
				((analogRead(MODULE_R) > SENSOR_THRESHOLD) ? SENSOR_R : 0) | 
				((analogRead(MODULE_F) > SENSOR_THRESHOLD) ? SENSOR_F : 0);
	
	digitalWrite(TRIGGER_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIGGER_PIN, LOW);
	distance = pulseIn(ECHO_PIN, HIGH)/58;	

	delay(10);
}

void roll(int dir, int speed, int time) {
	if (dir == FOWARD)
		wheel(L_WHEEL | R_WHEEL, FOWARD, speed);
	else if (dir == BACK)
		wheel(L_WHEEL | R_WHEEL, BACK, speed);
	else if (dir == RIGHT) {
		wheel(L_WHEEL, FOWARD, speed);
		wheel(R_WHEEL, BACK, speed);
	}
	else if (dir == LEFT) {
		wheel(L_WHEEL, BACK, speed);
		wheel(R_WHEEL, FOWARD, speed);
	}
	delay(time);
	wheel(L_WHEEL | R_WHEEL, stop, 0);
}

void turnRight() {

}

void turnLeft() {

}

void turnAround() {
	roll(RIGHT, TURNSPEED, TURNTIME);

	do {
		sense()
		roll(RIGHT, TURNSPEED, TICK);
	} while (sensorStatus != SENSOR_F);
}

void setup() {
	pinMode(TRIGGER_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);

	pinMode(ENA, OUTPUT);
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(ENB, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);

	wheel(L_WHEEL | R_WHEEL, STOP, 0);
}

void loop() {
	//감지 --> 판단 --> 움직임
	sense();

	//앞에 장애물이 있는지 체크하고
	if (distance < DISTANCE_THRESHOLD) {
		distanceCount++;
	} else {
		ditanceCount = 0;
	}
	//장애물이 있다면(THRESHOLD회 이상 있는 것으로 판단된 경우)뒤로 돈다.
	if (distanceCount < DISTANCE_COUNT_THRESHOLD) {
		distanceCount = 0;
		turnAround();
	
		return;
	}

	//사거리를 만난 경우
	if (sensorStatus & SENSOR_F && sensorStatus & SENSOR_R && sensorStatus & SENSOR_L) {
		turnRight();

		return;
	}

	//직진
	if (sensorStatus & SENSOR_L) {
		roll(RIGHT, SPEED, TICK);
		delay(TOCK);
	} else if (sensorStatus & SENSOR_R) {
		roll(LEFT, SPEED, TICK);
		delay(TOCK);
	} else {
		roll(FRONT, SPEED, TICK);
		delay(TOCK);
	}
	//모든 작업이 끝난 후 약간 멈추기
	delay(0);	
}
