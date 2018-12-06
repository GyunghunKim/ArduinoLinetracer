/*
 * 2018년도 2학기 창의공학설계 미니프로젝트 2
 * 2018. 12. 07
 * 김경훈, 김범석
*/

#define ENA 6
#define IN1 7
#define IN2 3
#define ENB 5
#define IN3 4
#define IN4 2

#define L_WHEEL 0x01
#define R_WHEEL 0x10

#define STOP 0
#define FOWARD 1
#define BACK 2

#define SENSOR_THRESHOLD 200
#define SENSOR_L 0x001
#define SENSOR_R 0x010
#define SENSOR_F 0x100

int sensorStatus = 0;

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
}

int turnRight() {

}

int turnLeft() {

}

int turnBack() {

}

void setup() {
	pinMode(ENA, OUTPUT);
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(ENB, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
}

void loop() {
	//감지 --> 판단 --> 움직임
	sense();
	delay(10);

	

	//모든 작업이 끝난 후 약간 멈추기
	wheel(L_WHEEL | R_WHEEL, STOP, 0);
	delay(10);	
}
