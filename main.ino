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

#define MODULE_L A2
#define MODULE_F A1
#define MODULE_R A0

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
#define DISTANCE_THRESHOLD 8
#define DISTANCE_COUNT_THRESHOLD 3

//속도와 관련된 상수
#define SPEED 100
#define TURNSPEED 200

//시간과 관련된 상수
#define TICK 75
#define TOCK 50

#define TURNTIME 100//75

#define GO_STRAIGHT_BEFORE_TURN 3 


int sensorStatus = 0;
int distance = 0;
int distanceCount = 0;

void wheel(int lr, int dir, int speed) {
	if (lr & R_WHEEL) {
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
			digitalWrite(IN1, HIGH);
			digitalWrite(IN2, LOW);
			analogWrite(ENA, speed);
		}
	}
	if (lr & L_WHEEL) {
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
			digitalWrite(IN3, HIGH);
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

	wheel(L_WHEEL | R_WHEEL, STOP, 0);
}

//직진하지만 길이 끝인 경우 dir 방향으로 움직인다.
void goStraight(int dir) {
	while(1) {
		sense();
		roll(FOWARD, SPEED, TICK);
		delay(TOCK);

		if (sensorStatus == SENSOR_F)
			break;
		if ((sensorStatus & SENSOR_F) == 0) {
			if (dir == RIGHT)
				turnRight();
			else if (dir == LEFT)
				turnLeft();
			break;
		}
	}
}

void turnRight() {
	for (int i = 0; i < GO_STRAIGHT_BEFORE_TURN; i++) {
		roll(FOWARD, SPEED, TICK);
		delay(TOCK);
	}

	while(1) {
		sense();
		roll(RIGHT, TURNSPEED, TURNTIME);
		delay(TOCK);

		if (sensorStatus == SENSOR_F)
			break;
	}
}

void turnLeft() {
	for (int i = 0; i < GO_STRAIGHT_BEFORE_TURN; i++) {
		roll(FOWARD, SPEED, TICK);
		delay(TOCK);
	}

	int previous = sensorStatus;

	while(1) {
		sense();
		roll(LEFT, TURNSPEED, TURNTIME);
		delay(TOCK);

		if (!(previous & SENSOR_F) && (sensorStatus & SENSOR_F)) {
			break;
		}
		previous = sensorStatus;
	}
}

//오른쪽으로 돌다가 길을 발견하면 그 길로 간다 (가장 왼쪽으로 가는 방법)
void turnAround() {
	while(1) {
		sense();
		roll(RIGHT, TURNSPEED, TICK);
		delay(TOCK);

		if (sensorStatus == SENSOR_F)
			break;
	}
}

void setup() {
	Serial.begin(9600);

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
		distanceCount = 0;
	}
	//장애물이 있다면(THRESHOLD회 이상 있는 것으로 판단된 경우)뒤로 돈다.
	if (distanceCount >= DISTANCE_COUNT_THRESHOLD) {
		distanceCount = 0;
		turnAround();
		return;
	}

	//사거리를 만난 경우
	if (sensorStatus == (SENSOR_F | SENSOR_R | SENSOR_L)) {
		turnLeft();
		return;
	} 
	//오른쪽 갈림길만 있는 경우
	else if (sensorStatus == (SENSOR_R | SENSOR_F)) {
		goStraight(RIGHT);
		return;
	} 
	//왼쪽 갈림길만 있는 경우
	else if (sensorStatus == (SENSOR_L | SENSOR_F)) {
		goStraight(LEFT);
		turnLeft();
		return;
	}

	//직진
	if (sensorStatus & SENSOR_L) {
		roll(LEFT, TURNSPEED, TICK);
		delay(TOCK);
	} else if (sensorStatus & SENSOR_R) {
		roll(RIGHT, TURNSPEED, TICK);
		delay(TOCK);
	} else if (sensorStatus & SENSOR_F) {
		roll(FOWARD, SPEED, TICK);
		delay(TOCK);
	}

	//길을 이탈한 경우
	if (sensorStatus == 0) {
		roll(FOWARD, SPEED, TICK);
		delay(TOCK);
	}
}
