void enable_A9_interrupts();
void config_GIC();
void config_interrupt (int N, int CPU_target);
void config_PS2s();
void set_A9_IRQ_stack();
void PS2_ISR();

void plot_pixel(int x, int y, short int line_color);
void clear_screen();
void clear_screen_w();
void plot_string(int x, int y, char *text_ptr);
void clear_characters();

volatile int pixel_buffer_start;
volatile char *character_buffer;
int where_you_are_x, where_you_are_y;
int seen_flag = 0;

// converts PS/2 input data to ASCII for printing 
int ps2_to_ascii (int ps2) {
      if (ps2 == 0x801C) return 0x61; // a
      else if (ps2 == 0x8032) return 0x62; // b
      else if (ps2 == 0x8021) return 0x63; // c
      else if (ps2 == 0x8023) return 0x64; // d
      else if (ps2 == 0x8024) return 0x65; // e
      else if (ps2 == 0x802B) return 0x66; // f
      else if (ps2 == 0x8034) return 0x67; // g
      else if (ps2 == 0x8033) return 0x68; // h
      else if (ps2 == 0x8043) return 0x69; // i
      else if (ps2 == 0x803B) return 0x6A; // j
      else if (ps2 == 0x8042) return 0x6B; // k
      else if (ps2 == 0x804B) return 0x6C; // l
      else if (ps2 == 0x803A) return 0x6D; // m
      else if (ps2 == 0x8031) return 0x6E; // n
      else if (ps2 == 0x8044) return 0x6F; // o
      else if (ps2 == 0x804D) return 0x70; // p
      else if (ps2 == 0x8015) return 0x71; // q
      else if (ps2 == 0x802D) return 0x72; // r
      else if (ps2 == 0x801B) return 0x73; // s
      else if (ps2 == 0x802C) return 0x74; // t
      else if (ps2 == 0x803C) return 0x75; // u
      else if (ps2 == 0x802A) return 0x76; // v
      else if (ps2 == 0x801D) return 0x77; // w
      else if (ps2 == 0x8022) return 0x78; // x
      else if (ps2 == 0x8035) return 0x79; // y
      else if (ps2 == 0x801A) return 0x7A; // z
      else if (ps2 == 0x801C) return 0x41; // A
      else if (ps2 == 0x8032) return 0x42; // B
      else if (ps2 == 0x8021) return 0x43; // C
      else if (ps2 == 0x8023) return 0x44; // D
      else if (ps2 == 0x8024) return 0x45; // E
      else if (ps2 == 0x802B) return 0x46; // F
      else if (ps2 == 0x8034) return 0x47; // G
      else if (ps2 == 0x8033) return 0x48; // H
      else if (ps2 == 0x8043) return 0x49; // I
      else if (ps2 == 0x803B) return 0x4A; // J
      else if (ps2 == 0x8042) return 0x4B; // K
      else if (ps2 == 0x804B) return 0x4C; // L
      else if (ps2 == 0x803A) return 0x4D; // M
      else if (ps2 == 0x8031) return 0x4E; // N
      else if (ps2 == 0x8044) return 0x4F; // O
      else if (ps2 == 0x804D) return 0x50; // P
      else if (ps2 == 0x8015) return 0x51; // Q
      else if (ps2 == 0x802D) return 0x52; // R
      else if (ps2 == 0x801B) return 0x53; // S
      else if (ps2 == 0x802C) return 0x54; // T
      else if (ps2 == 0x803C) return 0x55; // U
      else if (ps2 == 0x802A) return 0x56; // V
      else if (ps2 == 0x801D) return 0x57; // W
      else if (ps2 == 0x8022) return 0x58; // X
      else if (ps2 == 0x8035) return 0x59; // Y
      else if (ps2 == 0x801A) return 0x5A; // Z
      else if (ps2 == 0x8016) return 0x21; // !
      else if (ps2 == 0x8052) return 0x22; // 
      else if (ps2 == 0x8026) return 0x23; // #
      else if (ps2 == 0x8025) return 0x24; // $
      else if (ps2 == 0x802E) return 0x25; // %
      else if (ps2 == 0x803D) return 0x26; // &              
      else if (ps2 == 0x8046) return 0x28; // (
      else if (ps2 == 0x8045) return 0x29; // )
      else if (ps2 == 0x803E) return 0x2A; // *
      else if (ps2 == 0x8055) return 0x2B; // +
      else if (ps2 == 0x804C) return 0x3A; // :
      else if (ps2 == 0x8041) return 0x3C; // <
      else if (ps2 == 0x8049) return 0x3E; // >
      else if (ps2 == 0x804A) return 0x3F; // ?
      else if (ps2 == 0x801E) return 0x40; // @
      else if (ps2 == 0x8036) return 0x5E; // ^
      else if (ps2 == 0x804E) return 0x5F; // _
      else if (ps2 == 0x8054) return 0x7B; // {
      else if (ps2 == 0x805D) return 0x7C; // |
      else if (ps2 == 0x805B) return 0x7D; // }
      else if (ps2 == 0x800E) return 0x7E; // ~
      else if (ps2 == 0x8045) return 0x30; // 0
      else if (ps2 == 0x8016) return 0x31; // 1
      else if (ps2 == 0x801E) return 0x32; // 2
      else if (ps2 == 0x8026) return 0x33; // 3
      else if (ps2 == 0x8025) return 0x34; // 4
      else if (ps2 == 0x802E) return 0x35; // 5
      else if (ps2 == 0x8036) return 0x36; // 6
      else if (ps2 == 0x803D) return 0x37; // 7
      else if (ps2 == 0x803E) return 0x38; // 8
      else if (ps2 == 0x8046) return 0x39; // 9
      else if (ps2 == 0x8052) return 0x27; // '
      else if (ps2 == 0x8041) return 0x2C; // ,
      else if (ps2 == 0x804E) return 0x2D; // -
      else if (ps2 == 0x8049) return 0x2E; // .
      else if (ps2 == 0x804A) return 0x2F; // /
      else if (ps2 == 0x804C) return 0x3B; // ;
      else if (ps2 == 0x8055) return 0x3D; // =
      else if (ps2 == 0x8054) return 0x5B; // [
      else if (ps2 == 0x805D) return 0x5C; 
      else if (ps2 == 0x805B) return 0x5D; // ]
      else if (ps2 == 0x800E) return 0x60; // `
      else if (ps2 == 0x8029) return 0x20; // space
      else if (ps2 == 0x8066) return 0x08; // backspace 
      else if (ps2 == 0x800D) return 0x09; // tab 
      else if (ps2 == 0x805A) return 0x0D; // enter 
      else if (ps2 == 0x8076) return 0x1B; // escape
	  else return 0;
}


void enable_A9_interrupts() {
	int status = 0b01010011;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}

void config_GIC() {
	config_interrupt(79, 1);
	*((int *) 0xFFFEC104) = 0xFFFF;
	*((int *) 0xFFFEC100) = 1;
	*((int *) 0xFFFED000) = 1;
}

void config_interrupt (int N, int CPU_target) {
	int reg_offset, index, value, address;

	reg_offset = (N >> 3) & 0xFFFFFFFC;
	index = N & 0x1F;
	value = 0x1 << index;
	address = 0xFFFED100 + reg_offset;

	*(int *)address |= value;

	reg_offset = (N & 0xFFFFFFFC);
	index = N & 0x3;
	address = 0xFFFED800 + reg_offset + index;

	*(char *)address = (char) CPU_target;
}

void config_PS2s() {
	volatile int* PS2_ptr_interrupt = (int*)0xFF200104;
	*(PS2_ptr_interrupt) = 0x1;
} 

void PS2_ISR() {
	volatile int * PS2_ptr = (int *) 0xFF200100;
    int PS2_data;
    PS2_data = *(PS2_ptr);
	
	if (ps2_to_ascii(PS2_data) == 0) {
		seen_flag = 1;
		return;
	}
	
	if (seen_flag == 0) {
		char data[2];
		data[0] = ps2_to_ascii(PS2_data);
		data[1] = '\0';
		
		// BACKSPACE FUNCTIONALITY
		if (ps2_to_ascii(PS2_data) == 0x08) {
			if (where_you_are_x > 0) {
				where_you_are_x--;
			}
			else {
				if (where_you_are_y > 0) {
					where_you_are_x = 79;
					where_you_are_y--;
				}
			}
			
			int offset = (where_you_are_y << 7) + where_you_are_x;
            *(character_buffer + offset) = 0;
			return;
		}
		//******************//
		
		// NORMAL CHARACTERS
		plot_string(where_you_are_x, where_you_are_y, data);
		if (where_you_are_x < 79) {
			where_you_are_x++;
		} else {
			where_you_are_x = 0;
			where_you_are_y++;
		}

		if (where_you_are_y == 60) {
			where_you_are_y = 0;
		}
		//****************//
	} else
		seen_flag = 0;
}

void __attribute__ ((interrupt)) __cs3_isr_irq () {
	int interrupt_ID = *((int *) 0xFFFEC10C);
	if (interrupt_ID == 79) {
		PS2_ISR();
	} else
		while (1);

	*((int *) 0xFFFEC110) = interrupt_ID;
}

void set_A9_IRQ_stack() {
	int stack, mode;
	stack = 0xFFFFFFFF - 7;

	mode = 0b11010010;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
	asm("mov sp, %[ps]" : : [ps] "r" (stack));
	mode = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
}

void plot_pixel(int x, int y, short int line_color) {
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void clear_screen() {
    for (int x = 0; x < 320; x++) {
        for (int y = 0; y < 240; y++) {
            plot_pixel(x, y, 0x0000);
        }
    }
}

void clear_screen_w() {
    for (int x = 0; x < 320; x++) {
        for (int y = 0; y < 240; y++) {
            plot_pixel(x, y, 0xFFFF);
        }
    }
}

void plot_string(int x, int y, char *text_ptr) {
    int offset = (y << 7) + x;
    	while (*(text_ptr)) {
        	*(character_buffer + offset) = *(text_ptr);
        	text_ptr++;
        	offset++;
    	}
}


void clear_characters() {
    for(int y = 0; y < 60; y++){
        for(int x = 0; x < 80; x++){
            int offset = (y << 7) + x;
            *(character_buffer + offset) = 0;
            offset++;
        }
    }
}

int main(void) {
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    pixel_buffer_start = *pixel_ctrl_ptr;
	character_buffer = (char *) 0xC9000000;
	
    clear_screen();
	clear_characters();

	set_A9_IRQ_stack();
	config_GIC();
	config_PS2s();
	enable_A9_interrupts();
	
	where_you_are_x = 0;
	where_you_are_y = 0;
	
	while (1) {}
	
    return 0;
}
