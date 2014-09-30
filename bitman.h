

#define set_bit(dst, bit) (dst |= _BV(bit))
#define set_high(dst, bit) set_bit(dst, bit)
#define clear_bit(dst, bit) (dst &= ~ _BV(bit))
#define set_low(dst, bit) clear_bit(dst, bit)
#define check_bit(dst, bit) (dst & _BV(bit))
#define toggle_bit(dst, bit) (dst ^= _BV(bit))
#define concat_bytes(high, low) (((high) << 8) | (low))
#define test_bit(src, num) ((src) && _BV(num))

#define set_as_input(ddr, pin) clear_bit(ddr, pin)
#define set_as_output(ddr, pin) set_bit(ddr, pin)