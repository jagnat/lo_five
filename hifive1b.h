#define BIT(x) (1 << x)

volatile struct __GPIO
{
    unsigned input_val;
    unsigned input_en;
    unsigned output_en;
    unsigned output_val;
    unsigned pull_up_enable;
    unsigned drive_strength;
    unsigned rise_ie;
    unsigned rise_ip;
    unsigned fall_ie;
    unsigned fall_ip;
    unsigned high_ie;
    unsigned high_ip;
    unsigned low_ie;
    unsigned low_ip;
    unsigned padding[2];
    unsigned out_xor;
} extern GPIO;
