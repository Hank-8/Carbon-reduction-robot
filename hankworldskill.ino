#include <MatrixMC.h>
#include "MatrixMiniR4.h"

/*
MiniR4.PS2.read_gamepad(false, 0);輪尋數據
constrain();限制
*/

MatrixMC matrixMC;//定義擴充盒

float LX;
float LY;
float RX;
float M1,M2,M3,M4;

void setup()
{
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2);
  Serial.begin(115200);

  matrixMC.begin();

  LX = 0;
  LY = 0;
  RX = 0;


}

void loop()
{
  MiniR4.PS2.read_gamepad(false, 0);
  LY = map(MiniR4.PS2.Analog(PSS_LY), 0, 255, 90, (-90));
  LX = map(MiniR4.PS2.Analog(PSS_LX), 0, 255, 90, (-90));
  RX = map(MiniR4.PS2.Analog(PSS_RX), 0, 255, 90, (-90));

  M1 = constrain(((RX - LY) - LX), (-40), 40);
  M2 = constrain(((RX + LY) - LX), (-40), 40);
  M3 = constrain(((RX + LY) + LX), (-40), 40);
  M4 = constrain(((RX - LY) + LX), (-40), 40);

  matrixMC.loop();

  matrixMC.MC1.M1.setPower(M1);
  matrixMC.MC1.M2.setPower(M2);
  matrixMC.MC1.M3.setPower(M4);
  matrixMC.MC1.M4.setPower(M3);

  if(MiniR4.PS2.Button(PSB_PAD_UP))
  {
    MiniR4.RC1.setAngle(45);
  }
  if(MiniR4.PS2.Button(PSB_PAD_DOWN))
  {
    MiniR4.RC1.setAngle(140);
  }
  if(!(MiniR4.PS2.Button(PSB_PAD_UP)||MiniR4.PS2.Button(PSB_PAD_DOWN)))
  {
    MiniR4.RC1.setAngle(90);
  }
}