#include<BluetoothA2DPSink.h>
//#include<driver/i2s.h>
BluetoothA2DPSink a2dp;

const i2s_pin_config_t i2s_pins={
  .bck_io_num=27,
  .ws_io_num=26,
  .data_out_num=25,
  //.data_in_num=I2S_PIN_NO_CHANGE
};

void setup()
{
  a2dp.set_pin_config(i2s_pins);
  a2dp.start("ESP32");
}

void loop()
{
  

}
