void enable_A9_interrupts();
void config_GIC();
void config_interrupt (int N, int CPU_target);
void config_PS2s();
void set_A9_IRQ_stack();
void PS2_ISR();

void plot_pixel(int x, int y, short int line_color);
void clear_screen();
void plot_string(int x, int y, char *text_ptr);
void VGA_text_clean();

volatile int pixel_buffer_start;
volatile char *character_buffer;

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
	
	char data[2];
	data[1] = '\0';
	data[0] = PS2_data;
	plot_string(70, 50, data);
	
	return;
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

	while (1) {}
	
    return 0;
}
